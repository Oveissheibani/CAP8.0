"""System-resource introspection + parallelism advisor.

No Tk, no I/O beyond subprocess/proc reads.  Used by the parallelism
panel so the user picks a safe `--jobs` without guessing.
"""
from __future__ import annotations

import os
import platform
import subprocess
from dataclasses import dataclass

PYTHIA_MEM_GB_PER_PROC = 2.0      # generous upper bound at 200k events
RESERVED_OS_GB         = 4.0      # leave this much for the OS / apps


@dataclass(frozen=True)
class Advice:
    free_gb: float | None
    total_gb: float | None
    p_cores: int
    jobs: int
    note: str


def free_memory_gb() -> float | None:
    try:
        if platform.system() == "Darwin":
            out = subprocess.check_output(["vm_stat"], text=True, timeout=2)
            page = 4096
            free = inactive = 0
            for ln in out.splitlines():
                if "page size of" in ln:
                    try:
                        page = int(ln.rsplit("of ", 1)[1].split()[0])
                    except Exception:
                        page = 4096
                elif "Pages free" in ln:
                    free = int(ln.split(":")[1].strip().rstrip("."))
                elif "Pages inactive" in ln:
                    inactive = int(ln.split(":")[1].strip().rstrip("."))
            return (free + inactive) * page / (1024 ** 3)
        if platform.system() == "Linux":
            with open("/proc/meminfo") as fh:
                for ln in fh:
                    if ln.startswith("MemAvailable:"):
                        return int(ln.split()[1]) / (1024 ** 2)
    except Exception:
        return None
    return None


def total_memory_gb() -> float | None:
    try:
        if platform.system() == "Darwin":
            out = subprocess.check_output(
                ["sysctl", "-n", "hw.memsize"], text=True, timeout=2).strip()
            return int(out) / (1024 ** 3)
        if platform.system() == "Linux":
            with open("/proc/meminfo") as fh:
                for ln in fh:
                    if ln.startswith("MemTotal:"):
                        return int(ln.split()[1]) / (1024 ** 2)
    except Exception:
        return None
    return None


def p_cores() -> int:
    if platform.system() == "Darwin":
        for key in ("hw.perflevel0.physicalcpu", "hw.physicalcpu"):
            try:
                out = subprocess.check_output(
                    ["sysctl", "-n", key], text=True, timeout=2).strip()
                return int(out)
            except Exception:
                continue
    return os.cpu_count() or 1


def advise(rungs: int = 4) -> Advice:
    """Pick a safe `--jobs` for the given ladder size.  Returns Advice
    with terse fields the GUI can render however it wants."""
    free  = free_memory_gb()
    total = total_memory_gb()
    cores = p_cores()
    if free is None:
        return Advice(None, total, cores, 1,
                      "RAM unknown — defaulting to sequential.")
    usable = max(0.0, free - RESERVED_OS_GB)
    by_ram = int(usable // PYTHIA_MEM_GB_PER_PROC)
    jobs   = max(1, min(by_ram, cores, max(1, rungs)))
    note = (f"free={free:.1f} GB · usable={usable:.1f} GB · "
            f"{by_ram} fit at {PYTHIA_MEM_GB_PER_PROC:.0f} GB/proc · "
            f"cores={cores} · rungs={rungs}")
    return Advice(free, total, cores, jobs, note)
