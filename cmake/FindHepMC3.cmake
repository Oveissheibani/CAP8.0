# Locate the HepMC3 event-record library.
#
# HepMC3 is the modern (3.x) HEP Monte-Carlo event interchange format used
# by HERWIG, EPOS, MadGraph, Sherpa, and many others.  We detect it the
# same way our HERWIG install report (INSTALL_REPORT_HERWIG.md §5.1)
# documents — by asking the install's own `HepMC3-config --prefix` first,
# then falling back to environment variables and standard paths.
#
# Honours, in order of priority:
#   * CAP_HEPMC3_PATH        (env var or CMake variable — preferred entry)
#   * HEPMC3_DIR / HEPMC3    (env vars used by HepMC3 distributions)
#   * HepMC3-config          (executable shipped with HepMC3)
#   * Standard system paths  (/usr/local, /opt/homebrew, $CONDA_PREFIX, …)
#
# After running it sets:
#   HEPMC3_FOUND            TRUE if HepMC3 was found.
#   HEPMC3_INCLUDE_DIRS     Header dirs (contains HepMC3/GenEvent.h).
#   HEPMC3_LIBRARIES        Link target list (libHepMC3 + libHepMC3search).
#   HEPMC3_VERSION          Detected version string (best-effort).
# It also creates the imported target HepMC3::HepMC3.

include(FindPackageHandleStandardArgs)

# ---------- Collect candidate hint directories ----------
set(_hepmc3_hints)

foreach(_v CAP_HEPMC3_PATH HEPMC3_DIR HEPMC3_ROOT HEPMC3 HW_PREFIX)
    if(DEFINED ${_v} AND NOT "${${_v}}" STREQUAL "")
        list(APPEND _hepmc3_hints "${${_v}}")
    elseif(DEFINED ENV{${_v}} AND NOT "$ENV{${_v}}" STREQUAL "")
        list(APPEND _hepmc3_hints "$ENV{${_v}}")
    endif()
endforeach()

# Try HepMC3-config.  Per INSTALL_REPORT_HERWIG.md §5.2, this returns the
# install prefix reliably (unlike thepeg-config, HepMC3-config has correct
# exit semantics).
find_program(HEPMC3_CONFIG_EXECUTABLE NAMES HepMC3-config hepmc3-config)
if(HEPMC3_CONFIG_EXECUTABLE)
    execute_process(COMMAND ${HEPMC3_CONFIG_EXECUTABLE} --prefix
                    OUTPUT_VARIABLE _hepmc3_cfg_prefix
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    ERROR_QUIET)
    if(_hepmc3_cfg_prefix)
        list(APPEND _hepmc3_hints "${_hepmc3_cfg_prefix}")
    endif()
    execute_process(COMMAND ${HEPMC3_CONFIG_EXECUTABLE} --version
                    OUTPUT_VARIABLE HEPMC3_VERSION
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    ERROR_QUIET)
endif()

# Common system locations.
list(APPEND _hepmc3_hints
    /usr/local
    /opt/homebrew
    /opt/homebrew/opt/hepmc3
    /usr/local/opt/hepmc3
)
if(DEFINED ENV{CONDA_PREFIX})
    list(APPEND _hepmc3_hints "$ENV{CONDA_PREFIX}")
endif()

# ---------- Locate the header ----------
find_path(HEPMC3_INCLUDE_DIR
    NAMES HepMC3/GenEvent.h
    HINTS ${_hepmc3_hints}
    PATH_SUFFIXES include
    DOC "Directory containing HepMC3/GenEvent.h")

# ---------- Locate the libraries ----------
find_library(HEPMC3_LIBRARY
    NAMES HepMC3 HepMC3-static
    HINTS ${_hepmc3_hints}
    PATH_SUFFIXES lib lib64
    DOC "HepMC3 main library (libHepMC3.{so,dylib,a})")

# Search helpers (optional but commonly needed).
find_library(HEPMC3_SEARCH_LIBRARY
    NAMES HepMC3search
    HINTS ${_hepmc3_hints}
    PATH_SUFFIXES lib lib64
    DOC "HepMC3 search helpers library (libHepMC3search.{so,dylib})")

# ---------- Best-effort version detection from header if --version failed ----------
if(NOT HEPMC3_VERSION AND HEPMC3_INCLUDE_DIR)
    if(EXISTS "${HEPMC3_INCLUDE_DIR}/HepMC3/Version.h")
        file(STRINGS "${HEPMC3_INCLUDE_DIR}/HepMC3/Version.h" _hepmc3_ver_lines
             REGEX "HEPMC3_VERSION_CODE")
        if(_hepmc3_ver_lines)
            string(REGEX MATCH "[0-9]+" HEPMC3_VERSION_CODE "${_hepmc3_ver_lines}")
            set(HEPMC3_VERSION "${HEPMC3_VERSION_CODE}")
        endif()
    endif()
endif()

# ---------- Assemble outputs ----------
set(HEPMC3_INCLUDE_DIRS ${HEPMC3_INCLUDE_DIR})
set(HEPMC3_LIBRARIES    ${HEPMC3_LIBRARY})
if(HEPMC3_SEARCH_LIBRARY)
    list(APPEND HEPMC3_LIBRARIES ${HEPMC3_SEARCH_LIBRARY})
endif()

find_package_handle_standard_args(HepMC3
    REQUIRED_VARS HEPMC3_INCLUDE_DIR HEPMC3_LIBRARY
    VERSION_VAR   HEPMC3_VERSION)

# ---------- Imported target ----------
if(HEPMC3_FOUND AND NOT TARGET HepMC3::HepMC3)
    add_library(HepMC3::HepMC3 UNKNOWN IMPORTED)
    set_target_properties(HepMC3::HepMC3 PROPERTIES
        IMPORTED_LOCATION             "${HEPMC3_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${HEPMC3_INCLUDE_DIR}")
    if(HEPMC3_SEARCH_LIBRARY)
        set_target_properties(HepMC3::HepMC3 PROPERTIES
            INTERFACE_LINK_LIBRARIES "${HEPMC3_SEARCH_LIBRARY}")
    endif()
endif()

mark_as_advanced(HEPMC3_INCLUDE_DIR HEPMC3_LIBRARY HEPMC3_SEARCH_LIBRARY
                 HEPMC3_CONFIG_EXECUTABLE)
