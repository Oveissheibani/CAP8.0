#!/bin/bash
####################################################################################################
#
#  SetupCAP.sh — environment setup for the Correlation Analysis Package (CAP).
#
#  WHAT THIS SCRIPT DOES
#    • Defines CAP_* paths (source, build, lib, bin, projects, macros, database)
#    • Auto-detects optional external dependencies (ROOT, PYTHIA 8, FastJet)
#    • Provides sane defaults for user data/histogram paths
#    • Adds CAP's lib/bin directories to your library / PATH search
#
#  HOW TO USE
#    cd <where you cloned CAP>
#    source SetupCAP.sh                         # configure environment
#    cmake -S . -B build                        # configure CAP (top-level)
#    cmake --build build -j                     # compile
#    cmake --install build                      # install into ./bin and ./lib
#
#  OPTIONAL — you can override any of these BEFORE sourcing the script:
#    CAP_PYTHIA8_PATH      Path to a Pythia 8 install/source tree
#                          (default: ./pythia8317 or ./pythia83xx if present)
#    CAP_FASTJET_PATH      Path to a FastJet install
#                          (default: ./fastjet-3.4.3 or system location)
#    HISTOS_INPUT_PATH     Where the analysis reads input histograms from
#    HISTOS_OUTPUT_PATH    Where the analysis writes output histograms to
#                          (defaults to ./histos under the CAP root)
#
####################################################################################################

# ---------- Resolve the CAP root regardless of where the user invokes from ----------
# BASH_SOURCE works under bash; for zsh users sourcing, fall back to $0.
if [ -n "${BASH_SOURCE:-}" ]; then
    _cap_setup_self="${BASH_SOURCE[0]}"
elif [ -n "${ZSH_VERSION:-}" ]; then
    _cap_setup_self="${(%):-%x}"
else
    _cap_setup_self="$0"
fi

# Resolve symlinks and get the absolute directory of this script.
if command -v realpath > /dev/null 2>&1; then
    _cap_setup_dir="$(cd "$(dirname "$(realpath "$_cap_setup_self")")" && pwd)"
else
    _cap_setup_dir="$(cd "$(dirname "$_cap_setup_self")" && pwd)"
fi

export CAP_ROOT_PATH="$_cap_setup_dir"

unset _cap_setup_self _cap_setup_dir

# ---------- Load persisted dependency choices, if any ----------
# `./setup-cap` writes a .cap-config file with ROOTSYS / CAP_PYTHIA8_PATH /
# CAP_FASTJET_PATH and a couple of CAP_CMAKE_FLAGS_* convenience strings. By
# sourcing it before our own detection logic we honour the user's picks.
if [ -f "$CAP_ROOT_PATH/.cap-config" ]; then
    # shellcheck disable=SC1091
    source "$CAP_ROOT_PATH/.cap-config"
fi

# ---------- Core CAP paths (always defined relative to the repo) ----------
export CAP_SRC_PATH="$CAP_ROOT_PATH/src"
export CAP_BIN_PATH="$CAP_ROOT_PATH/bin"
export CAP_LIB_PATH="$CAP_ROOT_PATH/lib"
export CAP_BUILD_PATH="$CAP_ROOT_PATH/build"
export CAP_GRID_PATH="$CAP_ROOT_PATH/Grid"
export CAP_GRID_PATH_WSU="$CAP_GRID_PATH/WSU"
export CAP_GRID_PATH_ROM="$CAP_GRID_PATH/ROM"
export CAP_MACROS_PATH="$CAP_SRC_PATH/Macros"
export CAP_PROJECTS_PATH="$CAP_ROOT_PATH/projects"
export CAP_DATABASE_PATH="$CAP_ROOT_PATH/DB"

# Make CAP binaries and libraries discoverable.
case ":$PATH:" in
    *":$CAP_BIN_PATH:"*) ;;
    *) export PATH="$CAP_BIN_PATH:$PATH" ;;
esac
case ":${LD_LIBRARY_PATH:-}:" in
    *":$CAP_LIB_PATH:"*) ;;
    *) export LD_LIBRARY_PATH="$CAP_LIB_PATH:${LD_LIBRARY_PATH:-}" ;;
esac
case ":${DYLD_LIBRARY_PATH:-}:" in
    *":$CAP_LIB_PATH:"*) ;;
    *) export DYLD_LIBRARY_PATH="$CAP_LIB_PATH:${DYLD_LIBRARY_PATH:-}" ;;
esac

# ---------- User-facing data and histogram paths (with sensible defaults) ----------
# A user can override these in their shell profile. Otherwise they default to
# reasonable in-repo locations so a fresh clone works without further setup.
export CAP_DATA_IMPORT_PATH="${CAP_USER_DATA_IMPORT_PATH:-$CAP_ROOT_PATH/data}"
export CAP_DATA_EXPORT_PATH="${CAP_USER_DATA_EXPORT_PATH:-$CAP_ROOT_PATH/data}"
export CAP_CALIB_IMPORT_PATH="${CAP_USER_CALIB_IMPORT_PATH:-$CAP_DATA_IMPORT_PATH}"
export CAP_CALIB_EXPORT_PATH="${CAP_USER_CALIB_EXPORT_PATH:-$CAP_DATA_EXPORT_PATH}"
export CAP_HISTOS_IMPORT_PATH="${HISTOS_INPUT_PATH:-${CAP_USER_HISTO_IMPORT_PATH:-$CAP_ROOT_PATH/histos}}"
export CAP_HISTOS_EXPORT_PATH="${HISTOS_OUTPUT_PATH:-${CAP_USER_HISTO_EXPORT_PATH:-$CAP_ROOT_PATH/histos}}"

# Auto-create the default folders so first runs don't crash on missing dirs.
mkdir -p "$CAP_DATA_IMPORT_PATH" "$CAP_HISTOS_IMPORT_PATH" 2>/dev/null || true

# ---------- Helper: detect Apple Homebrew prefix on Apple Silicon vs Intel ----------
_cap_brew_prefix() {
    if command -v brew > /dev/null 2>&1; then
        brew --prefix 2>/dev/null
    elif [ -d /opt/homebrew ]; then
        echo /opt/homebrew
    elif [ -d /usr/local/Homebrew ]; then
        echo /usr/local
    fi
}

# ---------- ROOT detection (informational only — CMake does the real find_package) ----------
if [ -z "${ROOTSYS:-}" ]; then
    if command -v root-config > /dev/null 2>&1; then
        export ROOTSYS="$(root-config --prefix)"
    fi
fi
if [ -n "${ROOTSYS:-}" ] && [ -f "$ROOTSYS/bin/thisroot.sh" ] && [ -z "${CAP_ROOT_SOURCED:-}" ]; then
    # Source ROOT's own setup so users don't have to remember to.
    # shellcheck disable=SC1091
    source "$ROOTSYS/bin/thisroot.sh" > /dev/null 2>&1
    export CAP_ROOT_SOURCED=1
fi

# ---------- PYTHIA 8 detection ----------
# Order of preference:
#   1. User-supplied $CAP_PYTHIA8_PATH
#   2. In-tree pythia8XYZ directory (whatever version the user dropped in)
#   3. pythia8-config in PATH
#   4. Homebrew (brew install pythia8)
#   5. Conda environment ($CONDA_PREFIX)
if [ -z "${CAP_PYTHIA8_PATH:-}" ]; then
    # Look for pythia8NNN sibling directory inside the repo.
    for _candidate in "$CAP_ROOT_PATH"/pythia8[0-9]*; do
        if [ -d "$_candidate" ]; then
            CAP_PYTHIA8_PATH="$_candidate"
            break
        fi
    done
    unset _candidate
fi
if [ -z "${CAP_PYTHIA8_PATH:-}" ] && command -v pythia8-config > /dev/null 2>&1; then
    CAP_PYTHIA8_PATH="$(pythia8-config --prefix 2>/dev/null)"
fi
if [ -z "${CAP_PYTHIA8_PATH:-}" ]; then
    _brew="$(_cap_brew_prefix)"
    if [ -n "$_brew" ] && [ -d "$_brew/opt/pythia8" ]; then
        CAP_PYTHIA8_PATH="$_brew/opt/pythia8"
    fi
    unset _brew
fi
if [ -z "${CAP_PYTHIA8_PATH:-}" ] && [ -n "${CONDA_PREFIX:-}" ] && [ -f "$CONDA_PREFIX/include/Pythia8/Pythia.h" ]; then
    CAP_PYTHIA8_PATH="$CONDA_PREFIX"
fi
if [ -n "${CAP_PYTHIA8_PATH:-}" ]; then
    export CAP_PYTHIA8_PATH
    export CAP_PYTHIA_ROOT_DIR="$CAP_PYTHIA8_PATH"
    export CAP_PYTHIA8_INCLUDE_PATH="$CAP_PYTHIA8_PATH/include"
    if [ -d "$CAP_PYTHIA8_PATH/lib" ]; then
        export CAP_PYTHIA8_LIB_PATH="$CAP_PYTHIA8_PATH/lib"
    elif [ -d "$CAP_PYTHIA8_PATH/lib64" ]; then
        export CAP_PYTHIA8_LIB_PATH="$CAP_PYTHIA8_PATH/lib64"
    fi
fi

# ---------- FastJet detection ----------
# Order of preference:
#   1. User-supplied $CAP_FASTJET_PATH
#   2. In-tree fastjet-X.Y.Z source tree (CMake will offer to build it)
#   3. fastjet-config in PATH
#   4. Homebrew (brew install fastjet)
#   5. Conda environment ($CONDA_PREFIX)
if [ -z "${CAP_FASTJET_PATH:-}" ]; then
    for _candidate in "$CAP_ROOT_PATH"/fastjet-[0-9]*; do
        if [ -d "$_candidate" ]; then
            CAP_FASTJET_PATH="$_candidate"
            break
        fi
    done
    unset _candidate
fi
if [ -z "${CAP_FASTJET_PATH:-}" ] && command -v fastjet-config > /dev/null 2>&1; then
    CAP_FASTJET_PATH="$(fastjet-config --prefix 2>/dev/null)"
fi
if [ -z "${CAP_FASTJET_PATH:-}" ]; then
    _brew="$(_cap_brew_prefix)"
    if [ -n "$_brew" ] && [ -d "$_brew/opt/fastjet" ]; then
        CAP_FASTJET_PATH="$_brew/opt/fastjet"
    fi
    unset _brew
fi
if [ -z "${CAP_FASTJET_PATH:-}" ] && [ -n "${CONDA_PREFIX:-}" ] && [ -f "$CONDA_PREFIX/include/fastjet/PseudoJet.hh" ]; then
    CAP_FASTJET_PATH="$CONDA_PREFIX"
fi
if [ -n "${CAP_FASTJET_PATH:-}" ]; then
    export CAP_FASTJET_PATH
    export CAP_FASTJET_INCLUDE_PATH="$CAP_FASTJET_PATH/include"
    if [ -d "$CAP_FASTJET_PATH/lib" ]; then
        export CAP_FASTJET_LIB_PATH="$CAP_FASTJET_PATH/lib"
    elif [ -d "$CAP_FASTJET_PATH/lib64" ]; then
        export CAP_FASTJET_LIB_PATH="$CAP_FASTJET_PATH/lib64"
    fi
fi

unset -f _cap_brew_prefix

# ---------- Print the resolved configuration ----------
echo "========================================================================================"
echo " CAP environment configured"
echo "========================================================================================"
printf "  %-30s %s\n" "CAP_ROOT_PATH"            "$CAP_ROOT_PATH"
printf "  %-30s %s\n" "CAP_BIN_PATH"             "$CAP_BIN_PATH"
printf "  %-30s %s\n" "CAP_LIB_PATH"             "$CAP_LIB_PATH"
printf "  %-30s %s\n" "CAP_PROJECTS_PATH"        "$CAP_PROJECTS_PATH"
printf "  %-30s %s\n" "CAP_DATABASE_PATH"        "$CAP_DATABASE_PATH"
printf "  %-30s %s\n" "CAP_HISTOS_IMPORT_PATH"   "$CAP_HISTOS_IMPORT_PATH"
printf "  %-30s %s\n" "CAP_HISTOS_EXPORT_PATH"   "$CAP_HISTOS_EXPORT_PATH"
echo "----------------------------------------------------------------------------------------"
printf "  %-30s %s\n" "ROOTSYS"                  "${ROOTSYS:-(not detected — install ROOT)}"
printf "  %-30s %s\n" "CAP_PYTHIA8_PATH"         "${CAP_PYTHIA8_PATH:-(not detected — optional)}"
printf "  %-30s %s\n" "CAP_FASTJET_PATH"         "${CAP_FASTJET_PATH:-(not detected — optional)}"
echo "========================================================================================"
echo " Next: cmake -S . -B build && cmake --build build -j && cmake --install build"
echo "========================================================================================"
