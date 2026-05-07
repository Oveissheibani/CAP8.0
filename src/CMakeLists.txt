################################################################################
#
#  CMakeLists.txt for the CAP package — modernised, portable build.
#
#  Build (typical):
#    source ../SetupCAP.sh                 # optional but convenient
#    cmake -S .. -B ../build [-D CAP_ENABLE_PYTHIA=ON ...]
#    cmake --build ../build -j
#    cmake --install ../build
#
#  All optional modules are toggled with -D CAP_ENABLE_<NAME>=ON|OFF.
#  External dependencies (Pythia 8, FastJet) are located via find_package
#  using the modules in <repo>/cmake/.
#
################################################################################

cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(CAP
    VERSION 8.0
    DESCRIPTION "Correlation Analysis Package"
    LANGUAGES CXX
)

# ------------------------------------------------------------------------------
# Standard / build-type defaults
# ------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
        "Build type (Debug, Release, RelWithDebInfo, MinSizeRel)" FORCE)
endif()

# Apple-specific: target the running OS unless the caller overrode.
if(APPLE AND NOT CMAKE_OSX_DEPLOYMENT_TARGET)
    execute_process(COMMAND sw_vers -productVersion
                    OUTPUT_VARIABLE _macos_version
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    ERROR_QUIET)
    if(_macos_version)
        string(REGEX MATCH "^[0-9]+" _macos_major "${_macos_version}")
        set(CMAKE_OSX_DEPLOYMENT_TARGET "${_macos_major}.0" CACHE STRING
            "Minimum macOS deployment version" FORCE)
    endif()
endif()

# ------------------------------------------------------------------------------
# Install layout — respects -DCMAKE_INSTALL_PREFIX. Default is the repo root.
# ------------------------------------------------------------------------------
include(GNUInstallDirs)
# Default install prefix = the repo root (so libraries land in <repo>/lib and
# binaries in <repo>/bin, matching SetupCAP.sh's expectations).
#
# Use CMAKE_CURRENT_SOURCE_DIR (always <repo>/src) so the math is independent
# of whether the build was launched via the top-level wrapper CMakeLists.txt
# or directly via `cmake -S src -B build`.
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    get_filename_component(_cap_repo_root "${CMAKE_CURRENT_SOURCE_DIR}" DIRECTORY)
    set(CMAKE_INSTALL_PREFIX "${_cap_repo_root}" CACHE PATH
        "Install prefix (defaults to repo root: ./bin, ./lib)" FORCE)
    unset(_cap_repo_root)
endif()
# Per the CAP convention, libraries go into <prefix>/lib and binaries into
# <prefix>/bin even on multilib distros. Override with -DCAP_INSTALL_LIBDIR.
set(CAP_INSTALL_LIBDIR "lib" CACHE STRING "Library install dir under prefix")
set(CAP_INSTALL_BINDIR "bin" CACHE STRING "Binary install dir under prefix")

# Make per-module install targets work without sourcing SetupCAP.sh.
# All sub-CMakeLists.txt now reference ${CAP_INSTALL_LIBDIR}.

# Help find_package locate ROOT and our own modules.
list(PREPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../cmake")
if(DEFINED ENV{ROOTSYS})
    list(APPEND CMAKE_PREFIX_PATH "$ENV{ROOTSYS}")
endif()

# ------------------------------------------------------------------------------
# Build options for optional modules. Override on the cmake command line:
#   cmake -S . -B build -D CAP_ENABLE_PYTHIA=ON -D CAP_ENABLE_JETS=ON
# ------------------------------------------------------------------------------
option(CAP_ENABLE_THERMINATOR "Build the Therminator3 thermal generator"   ON)
option(CAP_ENABLE_GLAUBER     "Build the Glauber Monte-Carlo module"        ON)
option(CAP_ENABLE_PYTHIA      "Build the Pythia 8 wrapper"                  OFF)
# The Jets module is currently INCOMPLETE upstream code — it does not compile
# without further development work. See JETS_KNOWN_ISSUES.md for the bug list.
# CAP_FORCE_BROKEN_JETS lets contributors who are repairing it bypass the gate.
option(CAP_ENABLE_JETS        "Build the FastJet-based jet module (currently broken upstream)" OFF)
option(CAP_FORCE_BROKEN_JETS  "Force-build the broken Jets module (developers only)" OFF)
option(CAP_ENABLE_EPOS        "Build the EPOS event reader"                 OFF)
option(CAP_ENABLE_PHSD        "Build the PHSD event reader"                 OFF)
option(CAP_ENABLE_HEPMC3      "Build the HepMC3 reader (HERWIG/EPOS/MadGraph bridge)" OFF)
option(CAP_ENABLE_LHAPDF      "Link LHAPDF (parton distribution functions)" OFF)
option(CAP_ENABLE_FASTJET     "Link FastJet (jet algorithms; independent of Jets)" OFF)
option(CAP_ENABLE_YODA        "Link YODA histogram library"                 OFF)
option(CAP_ENABLE_RIVET       "Link Rivet analysis framework"               OFF)
option(CAP_ENABLE_HERWIG      "Build the embedded HERWIG 7 + ThePEG generator wrapper" OFF)
option(CAP_ENABLE_HIJING      "Build the HIJING wrapper (not in-tree)"      OFF)
option(CAP_ENABLE_AMPT        "Build the AMPT wrapper (not in-tree)"        OFF)
option(CAP_ENABLE_IDENTITY    "Build the Identity module (not in-tree)"     OFF)

option(CAP_BUILD_TESTS        "Build CAP unit tests (not yet implemented)"  OFF)
option(CAP_WARNINGS_AS_ERRORS "Treat warnings as errors (devs only)"        OFF)

# Backwards compatibility — accept the old set(Include_*) flag names too.
foreach(_pair
        "Include_PYTHIA;CAP_ENABLE_PYTHIA"
        "Include_JETS;CAP_ENABLE_JETS"
        "Include_FASTJET;CAP_ENABLE_FASTJET"
        "Include_EPOS;CAP_ENABLE_EPOS"
        "Include_PHSD;CAP_ENABLE_PHSD"
        "Include_HEPMC3;CAP_ENABLE_HEPMC3"
        "Include_LHAPDF;CAP_ENABLE_LHAPDF"
        "Include_YODA;CAP_ENABLE_YODA"
        "Include_RIVET;CAP_ENABLE_RIVET"
        "Include_THERMINATOR;CAP_ENABLE_THERMINATOR"
        "Include_GLAUBER;CAP_ENABLE_GLAUBER"
        "Include_HERWIG;CAP_ENABLE_HERWIG"
        "Include_HIJING;CAP_ENABLE_HIJING"
        "Include_AMPT;CAP_ENABLE_AMPT"
        "Include_IDENTITY;CAP_ENABLE_IDENTITY")
    list(GET _pair 0 _legacy)
    list(GET _pair 1 _modern)
    if(DEFINED ${_legacy} AND NOT DEFINED CACHE{${_modern}})
        if(${_legacy} STREQUAL "YES" OR ${_legacy})
            set(${_modern} ON CACHE BOOL "" FORCE)
        else()
            set(${_modern} OFF CACHE BOOL "" FORCE)
        endif()
    endif()
endforeach()
unset(_pair)
unset(_legacy)
unset(_modern)

# Jets is currently incomplete upstream code (multiple compile errors). Refuse
# to build it unless the developer explicitly asks for it via -DCAP_FORCE_BROKEN_JETS=ON.
if(CAP_ENABLE_JETS AND NOT CAP_FORCE_BROKEN_JETS)
    message(WARNING
        "  CAP_ENABLE_JETS=ON requested, but the Jets module is currently broken\n"
        "  upstream — see JETS_KNOWN_ISSUES.md. Disabling for this build.\n"
        "  If you are working on Jets, pass -DCAP_FORCE_BROKEN_JETS=ON to override.")
    set(CAP_ENABLE_JETS OFF CACHE BOOL "Disabled — Jets needs upstream repair" FORCE)
endif()

# Jets links against the CAPPythia target (it shares Pythia 8 input). Auto-
# enable Pythia when Jets is on so users don't have to think about it.
if(CAP_ENABLE_JETS AND NOT CAP_ENABLE_PYTHIA)
    message(STATUS "CAP_ENABLE_JETS=ON implies CAP_ENABLE_PYTHIA=ON; auto-enabling.")
    set(CAP_ENABLE_PYTHIA ON CACHE BOOL "Auto-enabled because CAP_ENABLE_JETS is ON" FORCE)
endif()

# ------------------------------------------------------------------------------
# Required dependency: ROOT
# ------------------------------------------------------------------------------
find_package(ROOT REQUIRED COMPONENTS EG MathCore MathMore RIO Hist Tree Net)
include(${ROOT_USE_FILE})
add_definitions(${ROOT_CXX_FLAGS})

# ------------------------------------------------------------------------------
# Optional dependencies — Pythia 8 and FastJet.
#
# Detection always runs when the user provided a hint (CAP_*_PATH) or has the
# *-config tool on PATH, regardless of whether the corresponding CAP module
# is enabled. That way the build summary clearly states whether the library
# was located on the system, separate from whether CAP is actually linking it.
# ------------------------------------------------------------------------------
set(_pythia_required FALSE)
if(CAP_ENABLE_PYTHIA OR CAP_ENABLE_JETS)
    set(_pythia_required TRUE)
endif()
if(_pythia_required OR DEFINED CAP_PYTHIA8_PATH OR DEFINED ENV{CAP_PYTHIA8_PATH})
    find_package(Pythia8)
endif()
if(_pythia_required AND NOT PYTHIA8_FOUND)
    message(FATAL_ERROR
        "CAP_ENABLE_PYTHIA / CAP_ENABLE_JETS requested but Pythia 8 was not found.\n"
        "Hints (set any one):\n"
        "  -D CAP_PYTHIA8_PATH=/path/to/pythia8XYZ   (CMake)\n"
        "  export CAP_PYTHIA8_PATH=/path/to/pythia8XYZ   (env)\n"
        "  ensure 'pythia8-config' is on PATH\n"
        "  brew install pythia8           # macOS Homebrew\n"
        "Or run ./install --gui to detect and pick an installation interactively.")
endif()
if(PYTHIA8_FOUND)
    message(STATUS "Pythia 8 ${PYTHIA8_VERSION} located at ${PYTHIA8_LIBRARY}")
endif()

# ------------------------------------------------------------------------------
#  HepMC3 — for the HERWIG/MadGraph/EPOS HepMC3-format bridge.
#  Detection via cmake/FindHepMC3.cmake, which honours the install's own
#  HepMC3-config (per INSTALL_REPORT_HERWIG.md §5).
# ------------------------------------------------------------------------------
if(CAP_ENABLE_HEPMC3 OR DEFINED CAP_HEPMC3_PATH OR DEFINED ENV{CAP_HEPMC3_PATH})
    find_package(HepMC3)
endif()
if(CAP_ENABLE_HEPMC3 AND NOT HEPMC3_FOUND)
    message(FATAL_ERROR
        "CAP_ENABLE_HEPMC3 requested but HepMC3 was not found.\n"
        "Hints (set any one):\n"
        "  -D CAP_HEPMC3_PATH=/path/to/hepmc3            (CMake)\n"
        "  export CAP_HEPMC3_PATH=/path/to/hepmc3        (env)\n"
        "  ensure 'HepMC3-config' is on PATH\n"
        "  for the local Herwig install:\n"
        "  -D CAP_HEPMC3_PATH=/Users/oveissheibani/LocalHerwig/LocalHerwig/opt")
endif()
if(HEPMC3_FOUND)
    message(STATUS "HepMC3 ${HEPMC3_VERSION} located at ${HEPMC3_LIBRARY}")
endif()

# ------------------------------------------------------------------------------
#  HERWIG 7 + ThePEG embedded — implies LHAPDF and benefits from HepMC3.
#  Auto-enable transitive deps so users don't have to think about them.
# ------------------------------------------------------------------------------
if(CAP_ENABLE_HERWIG)
    if(NOT CAP_ENABLE_LHAPDF)
        message(STATUS "CAP_ENABLE_HERWIG=ON implies CAP_ENABLE_LHAPDF=ON; "
                       "auto-enabling.")
        set(CAP_ENABLE_LHAPDF ON CACHE BOOL "Auto-enabled by CAP_ENABLE_HERWIG" FORCE)
    endif()
    if(NOT CAP_ENABLE_HEPMC3)
        message(STATUS "CAP_ENABLE_HERWIG=ON implies CAP_ENABLE_HEPMC3=ON; "
                       "auto-enabling.")
        set(CAP_ENABLE_HEPMC3 ON CACHE BOOL "Auto-enabled by CAP_ENABLE_HERWIG" FORCE)
    endif()
endif()

# ------------------------------------------------------------------------------
#  LHAPDF.
# ------------------------------------------------------------------------------
if(CAP_ENABLE_LHAPDF OR DEFINED CAP_LHAPDF_PATH OR DEFINED ENV{CAP_LHAPDF_PATH})
    find_package(LHAPDF)
endif()
if(CAP_ENABLE_LHAPDF AND NOT LHAPDF_FOUND)
    message(FATAL_ERROR
        "CAP_ENABLE_LHAPDF requested but LHAPDF was not found.\n"
        "Hints: -D CAP_LHAPDF_PATH=...  or  ensure 'lhapdf-config' is on PATH.")
endif()
if(LHAPDF_FOUND)
    message(STATUS "LHAPDF ${LHAPDF_VERSION} located at ${LHAPDF_LIBRARY}")
    if(LHAPDF_DATA_PATH)
        message(STATUS "  LHAPDF data path: ${LHAPDF_DATA_PATH}  "
                       "(set LHAPDF_DATA_PATH env var at runtime)")
    endif()
endif()

# ------------------------------------------------------------------------------
#  YODA.
# ------------------------------------------------------------------------------
if(CAP_ENABLE_YODA OR DEFINED CAP_YODA_PATH OR DEFINED ENV{CAP_YODA_PATH})
    find_package(YODA)
endif()
if(CAP_ENABLE_YODA AND NOT YODA_FOUND)
    message(FATAL_ERROR
        "CAP_ENABLE_YODA requested but YODA was not found.\n"
        "Hints: -D CAP_YODA_PATH=...  or  ensure 'yoda-config' is on PATH.")
endif()
if(YODA_FOUND)
    message(STATUS "YODA ${YODA_VERSION} located at ${YODA_LIBRARY}")
endif()

# ------------------------------------------------------------------------------
#  Rivet (depends on YODA + HepMC3 + FastJet at runtime).
# ------------------------------------------------------------------------------
if(CAP_ENABLE_RIVET OR DEFINED CAP_RIVET_PATH OR DEFINED ENV{CAP_RIVET_PATH})
    find_package(Rivet)
endif()
if(CAP_ENABLE_RIVET AND NOT RIVET_FOUND)
    message(FATAL_ERROR
        "CAP_ENABLE_RIVET requested but Rivet was not found.\n"
        "Hints: -D CAP_RIVET_PATH=...  or  ensure 'rivet-config' is on PATH.")
endif()
if(RIVET_FOUND)
    message(STATUS "Rivet ${RIVET_VERSION} located at ${RIVET_LIBRARY}")
endif()

# ------------------------------------------------------------------------------
#  HERWIG 7 + ThePEG.  Most demanding find module — handles thepeg-config
#  exit-code bug, plugin subdirectory layout, baked-in rpath.
# ------------------------------------------------------------------------------
if(CAP_ENABLE_HERWIG OR DEFINED CAP_HERWIG_PATH OR DEFINED ENV{HW_PREFIX})
    find_package(Herwig)
endif()
if(CAP_ENABLE_HERWIG AND NOT HERWIG_FOUND)
    message(FATAL_ERROR
        "CAP_ENABLE_HERWIG requested but HERWIG / ThePEG were not found.\n"
        "Hints (set any one):\n"
        "  -D CAP_HERWIG_PATH=/path/to/herwig             (CMake)\n"
        "  export HW_PREFIX=/path/to/herwig               (env, install report convention)\n"
        "  ensure 'herwig-config' and 'thepeg-config' are on PATH\n"
        "  source <prefix>/herwig-env.sh                  (recommended)\n"
        "For the documented LocalHerwig install:\n"
        "  -D CAP_HERWIG_PATH=/Users/oveissheibani/LocalHerwig/LocalHerwig/opt")
endif()
if(HERWIG_FOUND)
    message(STATUS "HERWIG ${HERWIG_VERSION} located at ${HERWIG_LIBRARY}")
    message(STATUS "  ThePEG located at ${THEPEG_LIBRARY}")
endif()

set(_fastjet_required FALSE)
if(CAP_ENABLE_JETS OR CAP_ENABLE_FASTJET)
    set(_fastjet_required TRUE)
endif()
if(_fastjet_required OR DEFINED CAP_FASTJET_PATH OR DEFINED ENV{CAP_FASTJET_PATH})
    find_package(FastJet)
endif()
if(_fastjet_required AND NOT FASTJET_FOUND)
    message(FATAL_ERROR
        "CAP_ENABLE_JETS / CAP_ENABLE_FASTJET requested but FastJet was not found.\n"
        "Hints:\n"
        "  -D CAP_FASTJET_PATH=/path/to/fastjet-X.Y.Z\n"
        "  ensure 'fastjet-config' is on PATH\n"
        "  brew install fastjet           # macOS Homebrew\n"
        "Or run ./install --gui to install the in-tree FastJet source.")
endif()
if(FASTJET_FOUND)
    message(STATUS "FastJet ${FASTJET_VERSION} located at ${FASTJET_LIBRARY}")
endif()

# ------------------------------------------------------------------------------
# Compile flags
# ------------------------------------------------------------------------------
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    add_compile_options(-Wall -Wextra)
    if(CAP_WARNINGS_AS_ERRORS)
        add_compile_options(-Werror)
    endif()
endif()
add_compile_options(-DOPTIMIZEADDBINCONTENT)

# ------------------------------------------------------------------------------
# Public include search path (kept compatible with the old layout).
# Sub-CMakeLists.txt assume each module's headers are reachable by short name.
# ------------------------------------------------------------------------------
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${ROOT_INCLUDE_DIRS}
    Base Math Helpers Xml Latex Plotting
    Particles ParticleSingle ParticlePair ParticlePair3D ParticleFlow
    SubSample CAPPythia Global Jets Spherocity NuDyn PtPt
    Therminator3 PHSD Performance BasicEventGen Glauber
)
if(PYTHIA8_FOUND)
    include_directories(${PYTHIA8_INCLUDE_DIRS} ${PYTHIA8_INCLUDE_DIRS}/Pythia8)
endif()
if(FASTJET_FOUND)
    include_directories(${FASTJET_INCLUDE_DIRS})
endif()

# ------------------------------------------------------------------------------
# Always-on modules
# ------------------------------------------------------------------------------
add_subdirectory(Base)
add_subdirectory(BasicEventGen)
add_subdirectory(Math)
add_subdirectory(Helpers)
add_subdirectory(Global)
add_subdirectory(Particles)
add_subdirectory(ParticleSingle)
add_subdirectory(ParticlePair)
add_subdirectory(ParticlePair3D)
add_subdirectory(Performance)
add_subdirectory(PtPt)
add_subdirectory(NuDyn)
add_subdirectory(Spherocity)
add_subdirectory(SubSample)
add_subdirectory(Xml)
add_subdirectory(Latex)

# ------------------------------------------------------------------------------
# Optional modules
# ------------------------------------------------------------------------------
if(CAP_ENABLE_THERMINATOR)
    add_subdirectory(Therminator3)
endif()
if(CAP_ENABLE_GLAUBER)
    add_subdirectory(Glauber)
endif()
if(CAP_ENABLE_PYTHIA)
    add_subdirectory(CAPPythia)
endif()
if(CAP_ENABLE_JETS)
    add_subdirectory(Jets)
endif()
if(CAP_ENABLE_EPOS)
    add_subdirectory(Epos)
endif()
if(CAP_ENABLE_PHSD)
    add_subdirectory(PHSD)
endif()
if(CAP_ENABLE_HEPMC3)
    add_subdirectory(CAPHepMC3)
endif()
# Embedded HERWIG 7 wrapper — separate from the legacy in-tree Herwig
# directory (which never shipped).  CAPHerwig is a peer module of
# CAPPythia, linked against Herwig::Herwig + Herwig::ThePEG.
if(CAP_ENABLE_HERWIG AND HERWIG_FOUND)
    add_subdirectory(CAPHerwig)
endif()
if(CAP_ENABLE_HERWIG AND IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Herwig)
    add_subdirectory(Herwig)
endif()
if(CAP_ENABLE_HIJING AND IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Hijing)
    add_subdirectory(Hijing)
endif()
if(CAP_ENABLE_AMPT AND IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Ampt)
    add_subdirectory(Ampt)
endif()
if(CAP_ENABLE_IDENTITY AND IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Identity)
    add_subdirectory(Identity)
endif()

# ------------------------------------------------------------------------------
# User analysis stubs (separate folder, doesn't touch src/).
# Lives at <repo>/analyses/stubs/ — added from inside src/CMakeLists.txt
# so that find_package(ROOT) and the ROOT include paths are already in
# scope when the dictionary is generated for libCAPAnalyses.
#
# Use CMAKE_CURRENT_SOURCE_DIR (always = <repo>/src) so the path math is
# independent of whether cmake was launched on the top-level CMakeLists or
# directly on src/.
# ------------------------------------------------------------------------------
get_filename_component(_cap_repo_root "${CMAKE_CURRENT_SOURCE_DIR}" DIRECTORY)
set(_cap_analyses_stubs "${_cap_repo_root}/analyses/stubs")
if(CAP_BUILD_USER_ANALYSES AND IS_DIRECTORY ${_cap_analyses_stubs})
    message(STATUS "Building user analysis stubs: ${_cap_analyses_stubs}")
    add_subdirectory(${_cap_analyses_stubs}
                     ${CMAKE_BINARY_DIR}/analyses-stubs-build)
elseif(CAP_BUILD_USER_ANALYSES)
    message(WARNING
        "CAP_BUILD_USER_ANALYSES=ON but ${_cap_analyses_stubs} does not exist.")
endif()

# ------------------------------------------------------------------------------
# Main executable
# ------------------------------------------------------------------------------
add_executable(CAP CAP.cxx)

set(_cap_link_libraries
    Base Math Particles SubSample Global Spherocity
    ParticleSingle ParticlePair ParticlePair3D NuDyn PtPt
    Performance BasicEventGen Xml Latex Helpers
)
if(CAP_ENABLE_THERMINATOR)
    list(APPEND _cap_link_libraries Therminator3)
endif()
if(CAP_ENABLE_GLAUBER)
    list(APPEND _cap_link_libraries Glauber)
endif()
if(CAP_ENABLE_PYTHIA)
    list(APPEND _cap_link_libraries CAPPythia)
endif()
if(CAP_ENABLE_JETS)
    list(APPEND _cap_link_libraries Jets)
endif()
if(CAP_ENABLE_EPOS)
    list(APPEND _cap_link_libraries Epos)
endif()
if(CAP_ENABLE_PHSD)
    list(APPEND _cap_link_libraries PHSD)
endif()
if(CAP_ENABLE_HEPMC3)
    list(APPEND _cap_link_libraries CAPHepMC3)
endif()
if(CAP_ENABLE_HERWIG AND TARGET CAPHerwig)
    list(APPEND _cap_link_libraries CAPHerwig)
endif()
if(TARGET CAPAnalyses)
    list(APPEND _cap_link_libraries CAPAnalyses)
endif()
target_link_libraries(CAP PUBLIC ${_cap_link_libraries})

install(TARGETS CAP RUNTIME DESTINATION ${CAP_INSTALL_BINDIR})

# ------------------------------------------------------------------------------
# Configuration summary printed at the end of `cmake -S . -B build`
# ------------------------------------------------------------------------------
# Build the human-readable status strings: differentiates "detected on system"
# from "actually linked into CAP".
function(_cap_status_line varname enable_flag found_flag version path)
    if(${enable_flag})
        if(${found_flag})
            set(${varname}
                "linked  (v${${version}}, ${${path}})" PARENT_SCOPE)
        else()
            set(${varname} "ENABLED but NOT FOUND" PARENT_SCOPE)
        endif()
    else()
        if(${found_flag})
            set(${varname}
                "detected on system (v${${version}}) but not linked"
                PARENT_SCOPE)
        else()
            set(${varname} "off (not detected, not linked)" PARENT_SCOPE)
        endif()
    endif()
endfunction()

_cap_status_line(_pythia_status  CAP_ENABLE_PYTHIA  PYTHIA8_FOUND  PYTHIA8_VERSION  PYTHIA8_LIBRARY)
_cap_status_line(_fastjet_status CAP_ENABLE_FASTJET FASTJET_FOUND  FASTJET_VERSION  FASTJET_LIBRARY)
_cap_status_line(_hepmc3_status  CAP_ENABLE_HEPMC3  HEPMC3_FOUND   HEPMC3_VERSION   HEPMC3_LIBRARY)
_cap_status_line(_lhapdf_status  CAP_ENABLE_LHAPDF  LHAPDF_FOUND   LHAPDF_VERSION   LHAPDF_LIBRARY)
_cap_status_line(_yoda_status    CAP_ENABLE_YODA    YODA_FOUND     YODA_VERSION     YODA_LIBRARY)
_cap_status_line(_rivet_status   CAP_ENABLE_RIVET   RIVET_FOUND    RIVET_VERSION    RIVET_LIBRARY)
_cap_status_line(_herwig_status  CAP_ENABLE_HERWIG  HERWIG_FOUND   HERWIG_VERSION   HERWIG_LIBRARY)

message(STATUS "")
message(STATUS "========================  CAP build configuration  ========================")
message(STATUS "  Install prefix         : ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  Build type             : ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ standard           : ${CMAKE_CXX_STANDARD}")
message(STATUS "  ROOT                   : ${ROOT_VERSION}")
message(STATUS "  Therminator            : ${CAP_ENABLE_THERMINATOR}")
message(STATUS "  Glauber                : ${CAP_ENABLE_GLAUBER}")
message(STATUS "  Pythia 8               : ${_pythia_status}")
message(STATUS "  FastJet                : ${_fastjet_status}")
message(STATUS "  HepMC3                 : ${_hepmc3_status}")
message(STATUS "  LHAPDF                 : ${_lhapdf_status}")
message(STATUS "  YODA                   : ${_yoda_status}")
message(STATUS "  Rivet                  : ${_rivet_status}")
message(STATUS "  HERWIG 7 / ThePEG      : ${_herwig_status}")
message(STATUS "  Jets module            : ${CAP_ENABLE_JETS}  (broken upstream — see JETS_KNOWN_ISSUES.md)")
message(STATUS "  EPOS / PHSD            : ${CAP_ENABLE_EPOS} / ${CAP_ENABLE_PHSD}")
message(STATUS "===========================================================================")
message(STATUS "")
