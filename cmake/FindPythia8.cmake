# Locate the Pythia 8 Monte-Carlo event generator.
#
# This module honours, in order of priority:
#   * CAP_PYTHIA8_PATH       (env var or CMake variable; the most common case in CAP)
#   * PYTHIA8_DIR / PYTHIA8  (env vars used by the official Pythia distribution)
#   * pythia8-config         (executable shipped with Pythia 8)
#   * pkg-config             (pythia8.pc shipped on some distros)
#   * Standard system paths  (/usr/local, /opt/homebrew, $CONDA_PREFIX, …)
#
# After running it sets:
#   PYTHIA8_FOUND            TRUE if Pythia was found.
#   PYTHIA8_INCLUDE_DIRS     Headers (contains Pythia8/Pythia.h).
#   PYTHIA8_LIBRARIES        Pythia link target list.
#   PYTHIA8_VERSION          The detected version string (best-effort).
#   PYTHIA8_XMLDOC_DIR       The xmldoc directory Pythia needs at runtime.
# It also creates an imported target Pythia8::Pythia8 if it isn't already defined.

include(FindPackageHandleStandardArgs)

# ---------- Collect candidate hint directories ----------
set(_pythia8_hints)

# Highest-priority hints: explicit user input.
foreach(_v CAP_PYTHIA8_PATH PYTHIA8_DIR PYTHIA8_ROOT PYTHIA8 PYTHIA8DATA)
    if(DEFINED ${_v} AND NOT "${${_v}}" STREQUAL "")
        list(APPEND _pythia8_hints "${${_v}}")
    elseif(DEFINED ENV{${_v}} AND NOT "$ENV{${_v}}" STREQUAL "")
        list(APPEND _pythia8_hints "$ENV{${_v}}")
    endif()
endforeach()

# Try pythia8-config.
find_program(PYTHIA8_CONFIG_EXECUTABLE NAMES pythia8-config)
if(PYTHIA8_CONFIG_EXECUTABLE)
    execute_process(COMMAND ${PYTHIA8_CONFIG_EXECUTABLE} --prefix
                    OUTPUT_VARIABLE _pythia8_cfg_prefix
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    ERROR_QUIET)
    if(_pythia8_cfg_prefix)
        list(APPEND _pythia8_hints "${_pythia8_cfg_prefix}")
    endif()
endif()

# pkg-config.
find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(_pythia8_pc QUIET pythia8)
    if(_pythia8_pc_FOUND)
        list(APPEND _pythia8_hints ${_pythia8_pc_PREFIX})
    endif()
endif()

# Common system locations.
list(APPEND _pythia8_hints
    /usr/local
    /opt/homebrew
    /opt/homebrew/opt/pythia8
    /usr/local/opt/pythia8
)
if(DEFINED ENV{CONDA_PREFIX})
    list(APPEND _pythia8_hints "$ENV{CONDA_PREFIX}")
endif()

# ---------- Locate header and library ----------
find_path(PYTHIA8_INCLUDE_DIR
    NAMES Pythia8/Pythia.h
    HINTS ${_pythia8_hints}
    PATH_SUFFIXES include
    DOC "Directory containing Pythia8/Pythia.h"
)

find_library(PYTHIA8_LIBRARY
    NAMES pythia8
    HINTS ${_pythia8_hints}
    PATH_SUFFIXES lib lib64
    DOC "The Pythia 8 shared library"
)

# Some distros ship a separate libpythia8lhapdf.so etc. — only the main library
# is required for CAP.

# ---------- xmldoc directory ----------
find_path(PYTHIA8_XMLDOC_DIR
    NAMES MainProgramSettings.xml
    HINTS ${_pythia8_hints}
    PATH_SUFFIXES share/Pythia8/xmldoc xmldoc share/pythia8/xmldoc
    DOC "Pythia 8 xmldoc directory (contains MainProgramSettings.xml)"
)

# ---------- Version detection ----------
if(PYTHIA8_INCLUDE_DIR AND EXISTS "${PYTHIA8_INCLUDE_DIR}/Pythia8/Pythia.h")
    file(STRINGS "${PYTHIA8_INCLUDE_DIR}/Pythia8/Pythia.h" _pythia8_ver_line
         REGEX "PYTHIA_VERSION[ \t]+[0-9]+\\.[0-9]+")
    if(_pythia8_ver_line)
        string(REGEX MATCH "[0-9]+\\.[0-9]+" PYTHIA8_VERSION "${_pythia8_ver_line}")
    endif()
endif()
if(NOT PYTHIA8_VERSION AND PYTHIA8_CONFIG_EXECUTABLE)
    execute_process(COMMAND ${PYTHIA8_CONFIG_EXECUTABLE} --version
                    OUTPUT_VARIABLE PYTHIA8_VERSION
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    ERROR_QUIET)
endif()

# ---------- Standard handling ----------
find_package_handle_standard_args(Pythia8
    REQUIRED_VARS PYTHIA8_LIBRARY PYTHIA8_INCLUDE_DIR
    VERSION_VAR PYTHIA8_VERSION
)

if(PYTHIA8_FOUND)
    set(PYTHIA8_LIBRARIES ${PYTHIA8_LIBRARY})
    set(PYTHIA8_INCLUDE_DIRS ${PYTHIA8_INCLUDE_DIR})
    if(NOT TARGET Pythia8::Pythia8)
        add_library(Pythia8::Pythia8 UNKNOWN IMPORTED)
        set_target_properties(Pythia8::Pythia8 PROPERTIES
            IMPORTED_LOCATION "${PYTHIA8_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${PYTHIA8_INCLUDE_DIR}"
        )
    endif()
endif()

mark_as_advanced(
    PYTHIA8_INCLUDE_DIR
    PYTHIA8_LIBRARY
    PYTHIA8_CONFIG_EXECUTABLE
    PYTHIA8_XMLDOC_DIR
)
