# Locate Rivet — particle-physics analysis framework (depends on YODA,
# HepMC3, FastJet at runtime).  rivet-config has reliable exit semantics
# (per INSTALL_REPORT_HERWIG.md §3.8: "lhapdf-config, fastjet-config,
# rivet-config, yoda-config, HepMC3-config, herwig-config all support
# --version" — the exception is thepeg-config).

include(FindPackageHandleStandardArgs)

set(_rivet_hints)
foreach(_v CAP_RIVET_PATH RIVET_DIR RIVET_ROOT RIVET HW_PREFIX)
    if(DEFINED ${_v} AND NOT "${${_v}}" STREQUAL "")
        list(APPEND _rivet_hints "${${_v}}")
    elseif(DEFINED ENV{${_v}} AND NOT "$ENV{${_v}}" STREQUAL "")
        list(APPEND _rivet_hints "$ENV{${_v}}")
    endif()
endforeach()

find_program(RIVET_CONFIG_EXECUTABLE NAMES rivet-config)
if(RIVET_CONFIG_EXECUTABLE)
    execute_process(COMMAND ${RIVET_CONFIG_EXECUTABLE} --prefix
                    OUTPUT_VARIABLE _rivet_cfg_prefix
                    OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
    if(_rivet_cfg_prefix)
        list(APPEND _rivet_hints "${_rivet_cfg_prefix}")
    endif()
    execute_process(COMMAND ${RIVET_CONFIG_EXECUTABLE} --version
                    OUTPUT_VARIABLE RIVET_VERSION
                    OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
endif()

list(APPEND _rivet_hints /usr/local /opt/homebrew)

find_path(RIVET_INCLUDE_DIR
    NAMES Rivet/Rivet.hh Rivet/Analysis.hh
    HINTS ${_rivet_hints}
    PATH_SUFFIXES include)

find_library(RIVET_LIBRARY
    NAMES Rivet
    HINTS ${_rivet_hints}
    PATH_SUFFIXES lib lib64)

set(RIVET_INCLUDE_DIRS ${RIVET_INCLUDE_DIR})
set(RIVET_LIBRARIES    ${RIVET_LIBRARY})

find_package_handle_standard_args(Rivet
    REQUIRED_VARS RIVET_INCLUDE_DIR RIVET_LIBRARY
    VERSION_VAR   RIVET_VERSION)

if(RIVET_FOUND AND NOT TARGET Rivet::Rivet)
    add_library(Rivet::Rivet UNKNOWN IMPORTED)
    set_target_properties(Rivet::Rivet PROPERTIES
        IMPORTED_LOCATION             "${RIVET_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${RIVET_INCLUDE_DIR}")
endif()

mark_as_advanced(RIVET_INCLUDE_DIR RIVET_LIBRARY RIVET_CONFIG_EXECUTABLE)
