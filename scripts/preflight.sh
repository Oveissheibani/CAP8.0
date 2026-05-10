#!/usr/bin/env bash
# =============================================================================
#  preflight.sh — cross-platform pre-install check.
#
#  Prints a friendly report of what's installed, what's missing, and the
#  exact command to fix each gap on the user's platform.
#
#  Supported platforms:
#    macOS Apple Silicon (arm64)  — Homebrew /opt/homebrew, MacPorts
#    macOS Intel (x86_64)         — Homebrew /usr/local, MacPorts
#    macOS pre-Homebrew           — bootstrap hint for Homebrew install
#    Ubuntu / Debian / Mint       — apt
#    Fedora / RHEL / Rocky / Alma — dnf
#    Arch / Manjaro / EndeavourOS — pacman
#    openSUSE / SLES              — zypper
#    Alpine                       — apk
#    Any Linux + conda            — conda-forge
#    Any Linux + CVMFS            — LCG view (auto-detected)
#    Any Linux + env modules      — `module load` hints when available
#
#  Run as: ./scripts/preflight.sh        (informational; never fails)
#          ./scripts/preflight.sh --strict   (exit 1 if critical missing)
#
#  Critical: ROOT, Python 3.8+, CMake 3.16+, a C++14-capable compiler.
#  Optional: tkinter (GUI), Pythia 8, FastJet, HepMC3, LHAPDF, YODA, Rivet.
# =============================================================================

set -uo pipefail

STRICT="${1:-}"
[ "$STRICT" = "--strict" ] && STRICT=1 || STRICT=0

# ── Colors (only if TTY) ────────────────────────────────────────────────────
if [ -t 1 ]; then
    C_RED='\033[1;31m'; C_GREEN='\033[1;32m'; C_YELLOW='\033[1;33m'
    C_CYAN='\033[1;36m'; C_DIM='\033[2m'; C_OFF='\033[0m'
else
    C_RED=''; C_GREEN=''; C_YELLOW=''; C_CYAN=''; C_DIM=''; C_OFF=''
fi
ok()   { printf "  ${C_GREEN}✓${C_OFF} %-26s %s\n" "$1" "$2"; }
warn() { printf "  ${C_YELLOW}⚠${C_OFF} %-26s %s\n" "$1" "$2"; n_warn=$((n_warn+1)); }
miss() { printf "  ${C_RED}✗${C_OFF} %-26s %s\n" "$1" "$2"; n_miss=$((n_miss+1)); }
info() { printf "  ${C_CYAN}ℹ${C_OFF} %-26s %s\n" "$1" "$2"; }
hr()   { printf "${C_DIM}══════════════════════════════════════════════════════════${C_OFF}\n"; }

n_warn=0
n_miss=0

# ── 1. Detect platform + package manager ──────────────────────────────────
SYS="$(uname -s)"
ARCH="$(uname -m)"
DISTRO=""
DISTRO_ID=""
PKG_MGR=""           # primary system PM: brew | port | apt | dnf | pacman | zypper | apk
BREW_PREFIX=""
PORT_PREFIX=""
case "$SYS" in
    Darwin)
        case "$ARCH" in
            arm64)  PLATFORM="macOS Apple Silicon (arm64)"
                    BREW_DEFAULT="/opt/homebrew" ;;
            x86_64) PLATFORM="macOS Intel (x86_64)"
                    BREW_DEFAULT="/usr/local" ;;
            *)      PLATFORM="macOS $ARCH"; BREW_DEFAULT="/usr/local" ;;
        esac
        BREW_PREFIX="$(brew --prefix 2>/dev/null || true)"
        if [ -z "$BREW_PREFIX" ] && [ -x "$BREW_DEFAULT/bin/brew" ]; then
            BREW_PREFIX="$BREW_DEFAULT"
        fi
        [ -d /opt/local/bin ] && [ -x /opt/local/bin/port ] && PORT_PREFIX="/opt/local"
        if   [ -n "$BREW_PREFIX" ]; then PKG_MGR="brew"
        elif [ -n "$PORT_PREFIX" ]; then PKG_MGR="port"
        else PKG_MGR=""
        fi
        ;;
    Linux)
        if [ -f /etc/os-release ]; then
            DISTRO="$(. /etc/os-release; echo "$NAME $VERSION_ID")"
            DISTRO_ID="$(. /etc/os-release; echo "$ID")"
        else
            DISTRO="(unknown distro)"
        fi
        PLATFORM="Linux $ARCH — $DISTRO"
        # Pick PM by ID first, then by tool presence as fallback.
        case "$DISTRO_ID" in
            ubuntu|debian|linuxmint|pop|elementary|raspbian) PKG_MGR="apt" ;;
            fedora|rhel|centos|rocky|almalinux|amzn|ol)      PKG_MGR="dnf" ;;
            arch|manjaro|endeavouros)                         PKG_MGR="pacman" ;;
            opensuse*|sles)                                   PKG_MGR="zypper" ;;
            alpine)                                           PKG_MGR="apk" ;;
            *)
                if   command -v apt-get >/dev/null 2>&1; then PKG_MGR="apt"
                elif command -v dnf     >/dev/null 2>&1; then PKG_MGR="dnf"
                elif command -v pacman  >/dev/null 2>&1; then PKG_MGR="pacman"
                elif command -v zypper  >/dev/null 2>&1; then PKG_MGR="zypper"
                elif command -v apk     >/dev/null 2>&1; then PKG_MGR="apk"
                fi
                ;;
        esac
        ;;
    *)
        PLATFORM="$SYS $ARCH"
        ;;
esac

# Detect environment modules + CVMFS as additional install sources.
HAS_MODULES=0
command -v module >/dev/null 2>&1 && HAS_MODULES=1
HAS_CVMFS=0
[ -d /cvmfs/sft.cern.ch ] && HAS_CVMFS=1

hr
printf "  ${C_CYAN}CAP install pre-flight${C_OFF}\n"
printf "  Platform:    %s\n" "$PLATFORM"
[ -n "$PKG_MGR"        ] && printf "  Package mgr: %s\n" "$PKG_MGR"
[ -n "$BREW_PREFIX"    ] && printf "  Homebrew:    %s\n" "$BREW_PREFIX"
[ -n "$PORT_PREFIX"    ] && printf "  MacPorts:    %s\n" "$PORT_PREFIX"
[ -n "${CONDA_PREFIX:-}" ] && printf "  Conda env:   %s\n" "$CONDA_PREFIX"
[ "$HAS_CVMFS"   = 1   ] && printf "  CVMFS:       /cvmfs/sft.cern.ch detected\n"
[ "$HAS_MODULES" = 1   ] && printf "  Modules:     environment-modules / Lmod available\n"
hr

# If macOS without Homebrew or MacPorts, lead with how to install one.
if [ "$SYS" = "Darwin" ] && [ -z "$BREW_PREFIX" ] && [ -z "$PORT_PREFIX" ]; then
    warn "package manager" "no Homebrew or MacPorts detected"
    printf "      ${C_DIM}install one of:${C_OFF}\n"
    printf "        Homebrew    /bin/bash -c \"\$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)\"\n"
    printf "        MacPorts    https://www.macports.org/install.php\n"
fi

# ── 2. Helper: install-hint per platform ───────────────────────────────────
hint() {
    # $1 = pretty name, $2-* = "key:cmd" pairs, e.g. brew:"brew install root"
    local name="$1"; shift
    printf "      ${C_DIM}install via:${C_OFF}\n"
    while [ "$#" -gt 0 ]; do
        local kv="$1"; shift
        local key="${kv%%:*}"
        local cmd="${kv#*:}"
        printf "        %-10s  %s\n" "$key" "$cmd"
    done
}

# ── 3. Required: bash ─────────────────────────────────────────────────────
if [ -n "${BASH_VERSION:-}" ]; then
    bv_major="${BASH_VERSION%%.*}"
    if [ "$bv_major" -ge 4 ]; then
        ok "bash" "$BASH_VERSION"
    else
        warn "bash" "$BASH_VERSION (3.2 — Apple-frozen; setup-cap has shims)"
    fi
fi

# ── helpers: pkg-manager-aware install hint builder ───────────────────────
# Build "key:cmd" pairs filtered to whatever is actually usable on this box.
# Pass it: pretty name + a series of "mgr=cmd" mappings.
pm_hint() {
    local name="$1"; shift
    local args=()
    while [ "$#" -gt 0 ]; do
        local mc="$1"; shift
        local mgr="${mc%%=*}"
        local cmd="${mc#*=}"
        case "$mgr" in
            brew)   [ -n "$BREW_PREFIX" ]    && args+=("brew:$cmd") ;;
            port)   [ -n "$PORT_PREFIX" ]    && args+=("port:$cmd") ;;
            apt)    [ "$PKG_MGR" = apt ]     && args+=("apt:$cmd") ;;
            dnf)    [ "$PKG_MGR" = dnf ]     && args+=("dnf:$cmd") ;;
            pacman) [ "$PKG_MGR" = pacman ]  && args+=("pacman:$cmd") ;;
            zypper) [ "$PKG_MGR" = zypper ]  && args+=("zypper:$cmd") ;;
            apk)    [ "$PKG_MGR" = apk ]     && args+=("apk:$cmd") ;;
            conda)  args+=("conda:$cmd") ;;            # always offer conda
            pip)    args+=("pip:$cmd") ;;              # always offer pip
            module) [ "$HAS_MODULES" = 1 ]   && args+=("module:$cmd") ;;
            cvmfs)  [ "$HAS_CVMFS"   = 1 ]   && args+=("cvmfs:$cmd") ;;
            xcode)  [ "$SYS" = Darwin ]      && args+=("xcode:$cmd") ;;
            source) args+=("source:$cmd") ;;
        esac
    done
    [ "${#args[@]}" -gt 0 ] && hint "$name" "${args[@]}"
}

# ── 4. Required: Python 3.8+ ──────────────────────────────────────────────
if command -v python3 >/dev/null 2>&1; then
    py_ver="$(python3 -c 'import sys; print(".".join(map(str,sys.version_info[:3])))' 2>/dev/null)"
    py_major="$(python3 -c 'import sys; print(sys.version_info[0])')"
    py_minor="$(python3 -c 'import sys; print(sys.version_info[1])')"
    if [ "$py_major" -eq 3 ] && [ "$py_minor" -ge 8 ]; then
        ok "Python" "$py_ver"
    else
        miss "Python" "$py_ver — need 3.8+"
        pm_hint "Python" \
            "brew=brew install python@3.12" \
            "port=sudo port install python312" \
            "apt=sudo apt install python3.12" \
            "dnf=sudo dnf install python3.12" \
            "pacman=sudo pacman -S python" \
            "zypper=sudo zypper install python3" \
            "apk=sudo apk add python3" \
            "conda=conda install python=3.12"
    fi
else
    miss "Python" "python3 not found on PATH"
    pm_hint "Python" \
        "brew=brew install python" \
        "port=sudo port install python312" \
        "apt=sudo apt install python3" \
        "dnf=sudo dnf install python3" \
        "pacman=sudo pacman -S python" \
        "zypper=sudo zypper install python3" \
        "apk=sudo apk add python3" \
        "conda=conda install python=3.12"
fi

# ── 5. Optional: tkinter (only required for GUI mode) ─────────────────────
if command -v python3 >/dev/null 2>&1; then
    if python3 -c "import tkinter" 2>/dev/null; then
        ok "tkinter" "available (GUI works)"
    else
        warn "tkinter" "missing — GUI install/runner won't launch (CLI works)"
        pm_hint "tkinter" \
            "brew=brew install python-tk" \
            "port=sudo port install py312-tkinter" \
            "apt=sudo apt install python3-tk" \
            "dnf=sudo dnf install python3-tkinter" \
            "pacman=sudo pacman -S tk" \
            "zypper=sudo zypper install python3-tk" \
            "apk=sudo apk add python3-tkinter" \
            "conda=conda install tk"
    fi
fi

# ── 6. Required: CMake 3.16+ ──────────────────────────────────────────────
if command -v cmake >/dev/null 2>&1; then
    cm_ver="$(cmake --version 2>/dev/null | head -1 | awk '{print $3}')"
    cm_major="$(echo "$cm_ver" | cut -d. -f1)"
    cm_minor="$(echo "$cm_ver" | cut -d. -f2)"
    if [ "$cm_major" -gt 3 ] || { [ "$cm_major" -eq 3 ] && [ "$cm_minor" -ge 16 ]; }; then
        ok "CMake" "$cm_ver"
    else
        miss "CMake" "$cm_ver — need 3.16+"
        pm_hint "CMake" \
            "brew=brew install cmake" \
            "port=sudo port install cmake" \
            "apt=sudo apt install cmake" \
            "dnf=sudo dnf install cmake" \
            "pacman=sudo pacman -S cmake" \
            "zypper=sudo zypper install cmake" \
            "apk=sudo apk add cmake" \
            "conda=conda install cmake" \
            "pip=pip install --user cmake"
    fi
else
    miss "CMake" "not found"
    pm_hint "CMake" \
        "brew=brew install cmake" \
        "port=sudo port install cmake" \
        "apt=sudo apt install cmake" \
        "dnf=sudo dnf install cmake" \
        "pacman=sudo pacman -S cmake" \
        "zypper=sudo zypper install cmake" \
        "apk=sudo apk add cmake" \
        "conda=conda install cmake"
fi

# ── 7. Required: C++ compiler (any of g++, clang++, c++) ──────────────────
cxx_found=""
cxx_old=0
for cxx in c++ g++ clang++; do
    if command -v "$cxx" >/dev/null 2>&1; then
        cxx_found="$cxx"
        cxx_ver_full="$($cxx --version 2>/dev/null | head -1)"
        # Try to extract a major version (works for gcc / clang / Apple clang).
        cxx_major="$(echo "$cxx_ver_full" | grep -oE '[0-9]+\.[0-9]+(\.[0-9]+)?' | head -1 | cut -d. -f1)"
        # C++14 needs gcc ≥ 5 / clang ≥ 3.4 / Apple clang ≥ 6.
        # We treat anything ≥ 5 as comfortably modern.
        if [ -n "$cxx_major" ] && [ "$cxx_major" -lt 5 ]; then
            cxx_old=1
        fi
        if [ "$cxx_old" = 1 ]; then
            warn "C++ compiler" "$cxx → $cxx_ver_full (may be too old for C++14)"
        else
            ok "C++ compiler" "$cxx → $cxx_ver_full"
        fi
        break
    fi
done
if [ -z "$cxx_found" ]; then
    miss "C++ compiler" "no g++/clang++/c++ on PATH"
    pm_hint "C++" \
        "xcode=xcode-select --install" \
        "brew=brew install gcc" \
        "port=sudo port install gcc13" \
        "apt=sudo apt install build-essential" \
        "dnf=sudo dnf install gcc-c++ make" \
        "pacman=sudo pacman -S base-devel" \
        "zypper=sudo zypper install -t pattern devel_C_C++" \
        "apk=sudo apk add build-base" \
        "conda=conda install -c conda-forge cxx-compiler"
fi

# ── 7b. macOS-only: Xcode Command Line Tools sanity ──────────────────────
if [ "$SYS" = "Darwin" ]; then
    if xcode-select -p >/dev/null 2>&1; then
        clt_path="$(xcode-select -p 2>/dev/null)"
        if [ -d "$clt_path" ]; then
            ok "Xcode CLT" "$clt_path"
        else
            miss "Xcode CLT" "xcode-select points to $clt_path which does not exist"
            pm_hint "Xcode CLT" "xcode=xcode-select --install"
        fi
    else
        miss "Xcode CLT" "Command Line Tools not installed"
        pm_hint "Xcode CLT" "xcode=xcode-select --install"
    fi
fi

# ── 8. Required: ROOT 6 ───────────────────────────────────────────────────
if command -v root-config >/dev/null 2>&1; then
    root_ver="$(root-config --version 2>/dev/null)"
    root_pfx="$(root-config --prefix 2>/dev/null)"
    ok "ROOT" "$root_ver  ($root_pfx)"
else
    miss "ROOT" "root-config not found — required"
    pm_hint "ROOT" \
        "brew=brew install root" \
        "port=sudo port install root6" \
        "apt=sudo apt install root-system-bin libroot-dev" \
        "dnf=sudo dnf install root root-montecarlo-eg" \
        "pacman=yay -S root  (AUR)" \
        "zypper=sudo zypper install root6" \
        "conda=conda install -c conda-forge root" \
        "cvmfs=source /cvmfs/sft.cern.ch/lcg/views/LCG_104/x86_64-el9-gcc11-opt/setup.sh" \
        "module=module load root" \
        "source=download from root.cern (binaries / build from source)"
fi

# ── 9. Optional: Pythia 8 ────────────────────────────────────────────────
if command -v pythia8-config >/dev/null 2>&1; then
    p8_ver="$(pythia8-config --version 2>/dev/null)"
    ok "Pythia 8" "$p8_ver"
elif [ -n "${CAP_PYTHIA8_PATH:-}" ] && [ -d "${CAP_PYTHIA8_PATH}" ]; then
    ok "Pythia 8" "via CAP_PYTHIA8_PATH = $CAP_PYTHIA8_PATH"
else
    info "Pythia 8" "not auto-detected (optional; setup-cap will offer to build it)"
fi

# ── 10. Optional: FastJet 3 ──────────────────────────────────────────────
if command -v fastjet-config >/dev/null 2>&1; then
    fj_ver="$(fastjet-config --version 2>/dev/null)"
    ok "FastJet" "$fj_ver"
elif [ -n "${CAP_FASTJET_PATH:-}" ] && [ -d "${CAP_FASTJET_PATH}" ]; then
    ok "FastJet" "via CAP_FASTJET_PATH"
else
    info "FastJet" "not auto-detected (optional; vendored fastjet-3.4.3/ can be used)"
fi

# ── 11. Optional: HepMC3, LHAPDF, YODA, Rivet (all from HW_PREFIX or PATH) ─
for tool in HepMC3-config lhapdf-config yoda-config rivet-config; do
    pretty="${tool%-config}"
    case "$pretty" in
        HepMC3) pretty="HepMC3" ;;
        lhapdf) pretty="LHAPDF" ;;
        yoda)   pretty="YODA"   ;;
        rivet)  pretty="Rivet"  ;;
    esac
    if command -v "$tool" >/dev/null 2>&1; then
        v="$($tool --version 2>/dev/null | head -1)"
        ok "$pretty" "$v"
    else
        info "$pretty" "not on PATH (optional; only needed for HERWIG / file-bridge)"
    fi
done

# ── 12. Optional: HW_PREFIX (HERWIG bundle root) ─────────────────────────
if [ -n "${HW_PREFIX:-}" ] && [ -d "$HW_PREFIX" ]; then
    ok "HW_PREFIX" "$HW_PREFIX"
elif [ -n "${HW_PREFIX:-}" ]; then
    warn "HW_PREFIX" "set to non-existent path: $HW_PREFIX"
else
    info "HW_PREFIX" "unset (optional; needed only for HERWIG-embedded build)"
fi

# ── Summary ───────────────────────────────────────────────────────────────
echo
hr
if [ "$n_miss" -eq 0 ] && [ "$n_warn" -eq 0 ]; then
    printf "  ${C_GREEN}✓ All checks passed.${C_OFF}  Ready for ./install\n"
elif [ "$n_miss" -eq 0 ]; then
    printf "  ${C_YELLOW}$n_warn warning(s).${C_OFF}  CAP will build, optional pieces disabled.\n"
else
    printf "  ${C_RED}$n_miss missing required component(s)${C_OFF}"
    [ "$n_warn" -gt 0 ] && printf ", $n_warn warning(s)"
    printf ".\n"
    printf "  Install the items above, then re-run.\n"
fi
hr

if [ "$STRICT" -eq 1 ] && [ "$n_miss" -gt 0 ]; then
    exit 1
fi
exit 0
