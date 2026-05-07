# Locate LHAPDF — parton distribution-function library.
#
# Honours, in order of priority:
#   * CAP_LHAPDF_PATH        (env var or CMake variable)
#   * LHAPDF_DIR / LHAPDF    (env vars used by LHAPDF distributions)
#   * lhapdf-config          (executable shipped with LHAPDF)
#   * Standard system paths  (/usr/local, /opt/homebrew, $CONDA_PREFIX, …)
#
# Sets:
#   LHAPDF_FOUND, LHAPDF_INCLUDE_DIRS, LHAPDF_LIBRARIES, LHAPDF_VERSION
#   LHAPDF_DATA_PATH (best-effort guess; runtime users should set the
#                     LHAPDF_DATA_PATH env var explicitly).
# Imported target: LHAPDF::LHAPDF.

include(FindPackageHandleStandardArgs)

set(_lhapdf_hints)
foreach(_v CAP_LHAPDF_PATH LHAPDF_DIR LHAPDF_ROOT LHAPDF HW_PREFIX)
    if(DEFINED ${_v} AND NOT "${${_v}}" STREQUAL "")
        list(APPEND _lhapdf_hints "${${_v}}")
    elseif(DEFINED ENV{${_v}} AND NOT "$ENV{${_v}}" STREQUAL "")
        list(APPEND _lhapdf_hints "$ENV{${_v}}")
    endif()
endforeach()

find_program(LHAPDF_CONFIG_EXECUTABLE NAMES lhapdf-config)
if(LHAPDF_CONFIG_EXECUTABLE)
    execute_process(COMMAND ${LHAPDF_CONFIG_EXECUTABLE} --prefix
                    OUTPUT_VARIABLE _lhapdf_cfg_prefix
                    OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
    if(_lhapdf_cfg_prefix)
        list(APPEND _lhapdf_hints "${_lhapdf_cfg_prefix}")
    endif()
    execute_process(COMMAND ${LHAPDF_CONFIG_EXECUTABLE} --version
                    OUTPUT_VARIABLE LHAPDF_VERSION
                    OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
    execute_process(COMMAND ${LHAPDF_CONFIG_EXECUTABLE} --datadir
                    OUTPUT_VARIABLE LHAPDF_DATA_PATH
                    OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
endif()

list(APPEND _lhapdf_hints
    /usr/local /opt/homebrew /opt/homebrew/opt/lhapdf /usr/local/opt/lhapdf)
if(DEFINED ENV{CONDA_PREFIX})
    list(APPEND _lhapdf_hints "$ENV{CONDA_PREFIX}")
endif()

find_path(LHAPDF_INCLUDE_DIR
    NAMES LHAPDF/LHAPDF.h
    HINTS ${_lhapdf_hints}
    PATH_SUFFIXES include)

find_library(LHAPDF_LIBRARY
    NAMES LHAPDF
    HINTS ${_lhapdf_hints}
    PATH_SUFFIXES lib lib64)

set(LHAPDF_INCLUDE_DIRS ${LHAPDF_INCLUDE_DIR})
set(LHAPDF_LIBRARIES    ${LHAPDF_LIBRARY})

find_package_handle_standard_args(LHAPDF
    REQUIRED_VARS LHAPDF_INCLUDE_DIR LHAPDF_LIBRARY
    VERSION_VAR   LHAPDF_VERSION)

if(LHAPDF_FOUND AND NOT TARGET LHAPDF::LHAPDF)
    add_library(LHAPDF::LHAPDF UNKNOWN IMPORTED)
    set_target_properties(LHAPDF::LHAPDF PROPERTIES
        IMPORTED_LOCATION             "${LHAPDF_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${LHAPDF_INCLUDE_DIR}")
endif()

mark_as_advanced(LHAPDF_INCLUDE_DIR LHAPDF_LIBRARY LHAPDF_CONFIG_EXECUTABLE)
