# Locate the FastJet jet-finding library.
#
# Honoured (in priority order):
#   * CAP_FASTJET_PATH       (env var or CMake variable; CAP-specific)
#   * FASTJET_DIR / FASTJET  (env vars used by upstream FastJet)
#   * fastjet-config         (executable shipped with FastJet)
#   * pkg-config             (fastjet.pc)
#   * Standard system paths  (/usr/local, /opt/homebrew, $CONDA_PREFIX)
#
# After running it sets:
#   FASTJET_FOUND
#   FASTJET_INCLUDE_DIRS
#   FASTJET_LIBRARIES        (main libfastjet plus optional libfastjettools)
#   FASTJET_VERSION
# and creates an imported target FastJet::FastJet.

include(FindPackageHandleStandardArgs)

set(_fastjet_hints)

foreach(_v CAP_FASTJET_PATH FASTJET_DIR FASTJET FASTJET_ROOT)
    if(DEFINED ${_v} AND NOT "${${_v}}" STREQUAL "")
        list(APPEND _fastjet_hints "${${_v}}")
    elseif(DEFINED ENV{${_v}} AND NOT "$ENV{${_v}}" STREQUAL "")
        list(APPEND _fastjet_hints "$ENV{${_v}}")
    endif()
endforeach()

find_program(FASTJET_CONFIG_EXECUTABLE NAMES fastjet-config)
if(FASTJET_CONFIG_EXECUTABLE)
    execute_process(COMMAND ${FASTJET_CONFIG_EXECUTABLE} --prefix
                    OUTPUT_VARIABLE _fastjet_cfg_prefix
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    ERROR_QUIET)
    if(_fastjet_cfg_prefix)
        list(APPEND _fastjet_hints "${_fastjet_cfg_prefix}")
    endif()

    execute_process(COMMAND ${FASTJET_CONFIG_EXECUTABLE} --version
                    OUTPUT_VARIABLE FASTJET_VERSION
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    ERROR_QUIET)
endif()

find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(_fastjet_pc QUIET fastjet)
    if(_fastjet_pc_FOUND)
        list(APPEND _fastjet_hints ${_fastjet_pc_PREFIX})
        if(NOT FASTJET_VERSION)
            set(FASTJET_VERSION ${_fastjet_pc_VERSION})
        endif()
    endif()
endif()

list(APPEND _fastjet_hints
    /usr/local
    /opt/homebrew
    /opt/homebrew/opt/fastjet
    /usr/local/opt/fastjet
)
if(DEFINED ENV{CONDA_PREFIX})
    list(APPEND _fastjet_hints "$ENV{CONDA_PREFIX}")
endif()

find_path(FASTJET_INCLUDE_DIR
    NAMES fastjet/PseudoJet.hh
    HINTS ${_fastjet_hints}
    PATH_SUFFIXES include
    DOC "Directory containing fastjet/PseudoJet.hh"
)

find_library(FASTJET_LIBRARY
    NAMES fastjet
    HINTS ${_fastjet_hints}
    PATH_SUFFIXES lib lib64
    DOC "The FastJet shared library"
)

find_library(FASTJET_TOOLS_LIBRARY
    NAMES fastjettools
    HINTS ${_fastjet_hints}
    PATH_SUFFIXES lib lib64
    DOC "The FastJet tools shared library (optional)"
)

# Version detection from header if not yet known.
if(NOT FASTJET_VERSION AND FASTJET_INCLUDE_DIR
   AND EXISTS "${FASTJET_INCLUDE_DIR}/fastjet/version.hh")
    file(STRINGS "${FASTJET_INCLUDE_DIR}/fastjet/version.hh" _fastjet_ver_line
         REGEX "fastjet_version[ \t]+\"[0-9]+\\.[0-9]+\\.[0-9]+\"")
    if(_fastjet_ver_line)
        string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" FASTJET_VERSION "${_fastjet_ver_line}")
    endif()
endif()

find_package_handle_standard_args(FastJet
    REQUIRED_VARS FASTJET_LIBRARY FASTJET_INCLUDE_DIR
    VERSION_VAR FASTJET_VERSION
)

if(FASTJET_FOUND)
    set(FASTJET_INCLUDE_DIRS ${FASTJET_INCLUDE_DIR})
    set(FASTJET_LIBRARIES ${FASTJET_LIBRARY})
    if(FASTJET_TOOLS_LIBRARY)
        list(APPEND FASTJET_LIBRARIES ${FASTJET_TOOLS_LIBRARY})
    endif()

    if(NOT TARGET FastJet::FastJet)
        add_library(FastJet::FastJet UNKNOWN IMPORTED)
        set_target_properties(FastJet::FastJet PROPERTIES
            IMPORTED_LOCATION "${FASTJET_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${FASTJET_INCLUDE_DIR}"
        )
        if(FASTJET_TOOLS_LIBRARY)
            set_target_properties(FastJet::FastJet PROPERTIES
                INTERFACE_LINK_LIBRARIES "${FASTJET_TOOLS_LIBRARY}"
            )
        endif()
    endif()
endif()

mark_as_advanced(
    FASTJET_INCLUDE_DIR
    FASTJET_LIBRARY
    FASTJET_TOOLS_LIBRARY
    FASTJET_CONFIG_EXECUTABLE
)
