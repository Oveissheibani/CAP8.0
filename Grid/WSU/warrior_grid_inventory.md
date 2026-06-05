# Wayne State "warrior" — HEP Software Inventory

Audit date: 2026-06-05 · Host: `mdt54` · User: `hx4574`
HOME: `/wsu/home/hx/hx45/hx4574` · OS: CentOS 7.9 · FS: Panasas (`panfs`)
**Toolchain for everything below: GCC 7.3.0** at `/opt/ohpc/pub/compiler/gcc/7.3.0`

> Note: the system default `/usr/bin/cc` is GCC 4.8.5. Any rebuild must force the
> gnu7 compilers (`export CC=$(which gcc) CXX=$(which g++) FC=$(which gfortran)`).

---

## 1. Master inventory

| Component | Version | Install prefix (use this) | Source/build tree | Notes |
|---|---|---|---|---|
| **Herwig** | 7.1.0 | `/wsu/.../Herwig/install` | `/wsu/.../Herwig/Herwig-7.1.0` | binary `install/bin/Herwig` |
| **ThePEG** | 2.1.0 | `/wsu/.../PEG/install` | `/wsu/.../PEG/ThePEG-2.1.0` | `libThePEG.so.25` |
| **Pythia 8** | 8.317 | `/wsu/.../pythia/PYTHIA8/install/pythia` | `.../sources/pythia8317` | built **with HepMC3 + LHAPDF6** |
| **EPOS 4** | 4.0.x (your build) | `/wsu/.../EPOS4/install/epos4` | `.../EPOS4/sources` | binary `bin/epos`, `bin/Xepos` |
| **HepMC2** | 2.06.09 | `/wsu/.../Herwig/HepMC2-install` | `.../HepMC2-build` | `libHepMC.so.4`; also a 2.06.10 source tree present |
| **HepMC3** | 3.02.07 | `/wsu/.../EPOS4/install/hepmc3` | `.../EPOS4/sources/HepMC3` | has rootIO; **also** a 2nd copy at `Herwig/HepMC3-install` |
| **LHAPDF** | 6.5.1 | `/wsu/.../LHAPDF/install` | `.../LHAPDF/LHAPDF-6.5.1` | this is the one ThePEG/Herwig use |
| **FastJet** | 3.4.0 | `/wsu/el7/gnu7/fastjet/3.4.0` (module) | — | not in HOME; load `fastjet/3.4.0` |
| **GSL** | 2.5 | `/wsu/el7/gnu7/gsl/2.5` (module) | — | load `gsl/2.5` (ignore the python 2.4 one on PATH) |
| **Boost** | 1.62.0 (Herwig) / 1.67.0 (ThePEG) | `/wsu/.../Herwig/install` headers / `/wsu/el7/gnu7/boost/1.67.0` | `.../Herwig/boost_1_62_0` | ThePEG used the 1.67 module; Herwig used self-built 1.62 |
| **ROOT** | 6.28.10 | `/wsu/el7/gnu7/root/6.28.10` (module) | — | not in HOME; HepMC3 rootIO links against it |

System modules also present but **not** your custom builds: `epos/4.0.0`, `pythia/8.303`.

(`/wsu/...` = `/wsu/home/hx/hx45/hx4574/...` throughout.)

---

## 2. HepMC linkage — the key relationships

```
EPOS4   ────────────────────────────►  HepMC3 3.02.07  (EPOS4/install/hepmc3)
Pythia 8.317  (libpythia8hepmc3.so) ──►  HepMC3
Herwig 7.1.0 ──► ThePEG 2.1.0 ──────►  HepMC2 2.06.09  (Herwig/HepMC2-install)   ◄── ONLY HepMC2
```

- Herwig does **not** link HepMC itself; its HepMC writing goes through ThePEG.
- ThePEG 2.1.0 here links **HepMC2 only**, so all current Herwig output is HepMC2.
- You already built a HepMC3 3.02.07 at `Herwig/HepMC3-install`, but nothing links to it —
  it looks like a half-finished attempt to move Herwig onto HepMC3.

---

## 3. Exact configure commands recovered from `config.log`

**HepMC2 2.06.09**
```bash
./configure --prefix=/wsu/.../Herwig/HepMC2-install \
            --with-momentum=GEV --with-length=MM
```

**LHAPDF 6.5.1**
```bash
./configure --prefix=/wsu/.../LHAPDF/install
```

**ThePEG 2.1.0**  ← decides Herwig's HepMC version
```bash
./configure --prefix=/wsu/.../PEG/install \
            --with-fastjet=/wsu/el7/gnu7/fastjet/3.4.0 \
            --with-gsl=/wsu/el7/gnu7/gsl/2.5 \
            --with-lhapdf=/wsu/.../LHAPDF/install \
            --with-boost=/wsu/el7/gnu7/boost/1.67.0 \
            --with-hepmc=/wsu/.../Herwig/HepMC2-install
```

**Herwig 7.1.0**
```bash
./configure --prefix=/wsu/.../Herwig/install \
            --with-thepeg=/wsu/.../PEG/install \
            --with-fastjet=/wsu/el7/gnu7/fastjet/3.4.0 \
            --with-gsl=/wsu/el7/gnu7/gsl/2.5 \
            --with-boost=/wsu/.../Herwig/install
# (--with-lhapdf and --with-hepmc were passed but IGNORED — inherited via ThePEG)
```

**EPOS4 / HepMC3 (CMake, GCC 7.3.0)** — install prefixes
`EPOS4/install/epos4` and `EPOS4/install/hepmc3`.

---

## 4. What a future install/build agent needs to know

### Build-time environment (rebuilding anything)
```bash
module load gnu7/7.3.0 cmake/3.21.1 root/6.28.10 fastjet/3.4.0 gsl/2.5
# system cc is GCC 4.8.5 — force gnu7:
export CC=$(which gcc) CXX=$(which g++) FC=$(which gfortran)
# ROOT here requires C++14:  -std=c++14
```
Build chain order for the Herwig stack:
`LHAPDF → (HepMC) → ThePEG → Herwig`. ThePEG needs LHAPDF + FastJet + GSL + Boost + HepMC;
Herwig needs ThePEG + the rest.

### Runtime environment (just *running* the existing Herwig)
```bash
H=/wsu/home/hx/hx45/hx4574
export PATH=$H/Herwig/install/bin:$PATH
export LD_LIBRARY_PATH=$H/Herwig/install/lib/Herwig:$H/PEG/install/lib/ThePEG:\
$H/LHAPDF/install/lib:$H/Herwig/HepMC2-install/lib:$LD_LIBRARY_PATH
module load gnu7/7.3.0 fastjet/3.4.0 gsl/2.5 root/6.28.10
```

### Cluster facts
- Partition: `mdtp`. Submit with `sbatch`. SLURM header that works:
  `--partition=mdtp --nodes=1 --ntasks=1 --time=...`.
- No `--with-hepmc`/`--with-lhapdf` on Herwig's own configure — set them on **ThePEG**.

---

## 5. Getting HepMC3 out of Herwig (the actual goal)

A single ThePEG build links **one** HepMC major version. So:

1. **Herwig 7.1.0 / ThePEG 2.1.0 (2017) is too old for clean HepMC3.** Reliable HepMC3
   support landed in later ThePEG (2.2.x+). Don't fight 2.1.0.
2. **Recommended:** install a current matched **Herwig 7.3.x + ThePEG 2.3.x** in a *new*
   prefix and point ThePEG at the HepMC3 you already have
   (`EPOS4/install/hepmc3`, v3.02.07) plus the existing LHAPDF/FastJet/GSL.
   This leaves the working HepMC2 Herwig untouched.
3. **"Both formats":** you generally won't get one build emitting both. Either
   (a) keep the HepMC2 build *and* add a HepMC3 build side by side, or
   (b) go HepMC3-only and convert to HepMC2 when needed (HepMC3 ships readers/writers).
4. Heads-up: the Herwig-side `density_calculator` reads **HepMC2**. Moving Herwig to
   HepMC3 means porting that analysis to the HepMC3 API (EPOS4's `density_calculator_epos`
   already uses HepMC3 and can be the template).

---

## 6. Housekeeping flags

- **Disk:** `Herwig/` alone is **391 GB**, `pythia/` 46 GB, `EPOS4/` 43 GB. Most of the
  Herwig bulk is source + Boost build trees (`boost_1_62_0`, `*-build` dirs) you can
  archive once installs are verified. (Note: `df` reported a 47 GB quota view that
  conflicts with `du` — worth checking your Panasas quota with the HPC admins.)
- **Duplicate LHAPDF copies:** `LHAPDF/install` (canonical), plus a `LHAPDF-new/install`
  referenced in a config.log and a `Herwig/LHAPDF-6.5.1` source tree. Keep one.
- **Two HepMC3 3.02.07 installs:** `EPOS4/install/hepmc3` (with rootIO — prefer this) and
  `Herwig/HepMC3-install` (orphaned). Consolidate on one.
- **Boost mismatch:** ThePEG built against module Boost 1.67.0, Herwig against self-built
  1.62.0. It works, but a future rebuild should standardize on one.
