# Locate YODA — Yet more Objects for Data Analysis (histogram library
# used by Rivet).  Detection follows the same pattern as FindLHAPDF.

include(FindPackageHandleStandardArgs)

set(_yoda_hints)
foreach(_v CAP_YODA_PATH YODA_DIR YODA_ROOT YODA HW_PREFIX)
    if(DEFINED ${_v} AND NOT "${${_v}}" STREQUAL "")
        list(APPEND _yoda_hints "${${_v}}")
    elseif(DEFINED ENV{${_v}} AND NOT "$ENV{${_v}}" STREQUAL "")
        list(APPEND _yoda_hints "$ENV{${_v}}")
    endif()
endforeach()

find_program(YODA_CONFIG_EXECUTABLE NAMES yoda-config)
if(YODA_CONFIG_EXECUTABLE)
    execute_process(COMMAND ${YODA_CONFIG_EXECUTABLE} --prefix
                    OUTPUT_VARIABLE _yoda_cfg_prefix
                    OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
    if(_yoda_cfg_prefix)
        list(APPEND _yoda_hints "${_yoda_cfg_prefix}")
    endif()
    execute_process(COMMAND ${YODA_CONFIG_EXECUTABLE} --version
                    OUTPUT_VARIABLE YODA_VERSION
                    OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
endif()

list(APPEND _yoda_hints /usr/local /opt/homebrew)
if(DEFINED ENV{CONDA_PREFIX})
    list(APPEND _yoda_hints "$ENV{CONDA_PREFIX}")
endif()

find_path(YODA_INCLUDE_DIR
    NAMES YODA/Histo1D.h
    HINTS ${_yoda_hints}
    PATH_SUFFIXES include)

find_library(YODA_LIBRARY
    NAMES YODA
    HINTS ${_yoda_hints}
    PATH_SUFFIXES lib lib64)

set(YODA_INCLUDE_DIRS ${YODA_INCLUDE_DIR})
set(YODA_LIBRARIES    ${YODA_LIBRARY})

find_package_handle_standard_args(YODA
    REQUIRED_VARS YODA_INCLUDE_DIR YODA_LIBRARY
    VERSION_VAR   YODA_VERSION)

if(YODA_FOUND AND NOT TARGET YODA::YODA)
    add_library(YODA::YODA UNKNOWN IMPORTED)
    set_target_properties(YODA::YODA PROPERTIES
        IMPORTED_LOCATION             "${YODA_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${YODA_INCLUDE_DIR}")
endif()

mark_as_advanced(YODA_INCLUDE_DIR YODA_LIBRARY YODA_CONFIG_EXECUTABLE)
