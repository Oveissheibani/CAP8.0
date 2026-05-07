# Locate HERWIG 7 + ThePEG — full embedded-generation linking.
#
# This is the most involved find module in CAP because HERWIG sits on
# top of ThePEG, and both ship hand-written `*-config` shell helpers
# whose flags must be respected verbatim.  The install report
# (INSTALL_REPORT_HERWIG.md §5) is required reading.
#
# Specific quirks honoured here:
#   §3.8: thepeg-config exits 1 even on success — we ignore $? and check
#         that the captured stdout is non-empty instead.
#   §5.2: thepeg-config --libdir returns $HW_PREFIX/lib/ThePEG (a
#         subdirectory).  herwig-config --libdir returns
#         $HW_PREFIX/lib/Herwig.  Plugin libraries are NOT at top-level
#         lib/ — must use both subdirs.
#   §5.2: thepeg-config --ldflags bakes in critical rpath entries for
#         lib/, lib/gcc/15, etc.  We forward them to consumers via
#         INTERFACE_LINK_OPTIONS on the imported targets.
#   §5.5: Herwig.so is loaded with RTLD_GLOBAL at runtime so its plugins
#         can see the symbol table.  We document but don't enforce that.
#
# Honours, in order of priority:
#   * CAP_HERWIG_PATH         (env or CMake var)
#   * HW_PREFIX               (env var the install report uses)
#   * HERWIG_DIR              (env var)
#   * herwig-config           (executable shipped with HERWIG)
#   * thepeg-config           (executable shipped with ThePEG)
#   * Standard system paths
#
# Sets:
#   HERWIG_FOUND, HERWIG_INCLUDE_DIRS, HERWIG_LIBRARIES, HERWIG_VERSION
#   THEPEG_INCLUDE_DIRS, THEPEG_LIBRARIES, THEPEG_LIBDIR, THEPEG_VERSION
#   HERWIG_LDFLAGS_OTHER (raw -Wl,-rpath... and other linker flags from
#                          herwig-config + thepeg-config that aren't
#                          plain -L/-l; pass to target_link_options)
# Imported targets:
#   Herwig::ThePEG    — ThePEG core
#   Herwig::Herwig    — HERWIG core (depends on ThePEG)

include(FindPackageHandleStandardArgs)

set(_hw_hints)
foreach(_v CAP_HERWIG_PATH HW_PREFIX HERWIG_DIR HERWIG_ROOT HERWIG)
    if(DEFINED ${_v} AND NOT "${${_v}}" STREQUAL "")
        list(APPEND _hw_hints "${${_v}}")
    elseif(DEFINED ENV{${_v}} AND NOT "$ENV{${_v}}" STREQUAL "")
        list(APPEND _hw_hints "$ENV{${_v}}")
    endif()
endforeach()
list(APPEND _hw_hints /usr/local /opt/homebrew)

# ----- thepeg-config (note: §3.8 — its exit code lies, ignore it) -----
find_program(THEPEG_CONFIG_EXECUTABLE NAMES thepeg-config
    HINTS ${_hw_hints} PATH_SUFFIXES bin)

set(THEPEG_PREFIX     "")
set(THEPEG_LIBDIR     "")
set(THEPEG_INCLUDEDIR "")
set(THEPEG_DATADIR    "")
set(THEPEG_CPPFLAGS   "")
set(THEPEG_LDFLAGS    "")
set(THEPEG_LDLIBS     "")
set(THEPEG_VERSION    "")
if(THEPEG_CONFIG_EXECUTABLE)
    foreach(_q prefix libdir includedir datadir cppflags ldflags ldlibs)
        # Capture stdout, ignore exit code per install report §3.8.
        execute_process(COMMAND ${THEPEG_CONFIG_EXECUTABLE} --${_q}
                        OUTPUT_VARIABLE _val
                        OUTPUT_STRIP_TRAILING_WHITESPACE
                        ERROR_QUIET)
        string(TOUPPER "${_q}" _Q)
        set(THEPEG_${_Q} "${_val}")
    endforeach()
    if(THEPEG_PREFIX)
        list(APPEND _hw_hints "${THEPEG_PREFIX}")
    endif()
endif()

# ----- herwig-config -----
find_program(HERWIG_CONFIG_EXECUTABLE NAMES herwig-config
    HINTS ${_hw_hints} PATH_SUFFIXES bin)

set(HERWIG_PREFIX     "")
set(HERWIG_LIBDIR     "")
set(HERWIG_DATADIR    "")
set(HERWIG_CPPFLAGS   "")
set(HERWIG_LDFLAGS    "")
set(HERWIG_LDLIBS     "")
if(HERWIG_CONFIG_EXECUTABLE)
    foreach(_q prefix libdir datadir cppflags ldflags ldlibs)
        execute_process(COMMAND ${HERWIG_CONFIG_EXECUTABLE} --${_q}
                        OUTPUT_VARIABLE _val
                        OUTPUT_STRIP_TRAILING_WHITESPACE
                        ERROR_QUIET)
        string(TOUPPER "${_q}" _Q)
        set(HERWIG_${_Q} "${_val}")
    endforeach()
    execute_process(COMMAND ${HERWIG_CONFIG_EXECUTABLE} --version
                    OUTPUT_VARIABLE HERWIG_VERSION
                    OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
    if(HERWIG_PREFIX)
        list(APPEND _hw_hints "${HERWIG_PREFIX}")
    endif()
endif()

# ----- Locate the headers -----
find_path(THEPEG_INCLUDE_DIR
    NAMES ThePEG/EventRecord/Event.h ThePEG/Repository/EventGenerator.h
    HINTS ${_hw_hints} PATH_SUFFIXES include)

find_path(HERWIG_INCLUDE_DIR
    NAMES Herwig/API/HerwigAPI.h Herwig/Shower/ShowerAlpha.h
    HINTS ${_hw_hints} PATH_SUFFIXES include)
# Note: Herwig headers can also live alongside ThePEG headers under the
# same include/.  We keep them separate in case future Herwig versions
# change layout.

# ----- Locate the libraries.  Plugins live in subdirs (§5.2). -----
#
# IMPORTANT: per INSTALL_REPORT_HERWIG.md §4.2 and §5.5, the Herwig
# core ships as `Herwig.so` — NO `lib` prefix, `.so` suffix even on
# macOS — because it's a dlopen-able plugin, not a regular dylib.
# CMake's find_library() applies the platform's lib-prefix and
# dylib-suffix automatically (libHerwig.dylib on macOS), so it never
# finds Herwig.so.  Use find_file() with explicit filenames instead.
find_library(THEPEG_LIBRARY
    NAMES ThePEG
    HINTS ${_hw_hints} ${THEPEG_LIBDIR}
    PATH_SUFFIXES lib lib64 lib/ThePEG lib64/ThePEG)
# Fallback: if find_library missed it (e.g. ThePEG installed as a
# bare ThePEG.so plugin too), try find_file with explicit names.
if(NOT THEPEG_LIBRARY)
    find_file(THEPEG_LIBRARY
        NAMES libThePEG.dylib libThePEG.so libThePEG.a
              ThePEG.so ThePEG.dylib
        HINTS ${_hw_hints} ${THEPEG_LIBDIR}
        PATH_SUFFIXES lib lib64 lib/ThePEG lib64/ThePEG)
endif()

# HERWIG: explicitly find Herwig.so (the dlopen-able plugin) by file
# name.  We accept both the "lib"-prefixed names (in case a future
# build switches convention) and the bare Herwig.so / Herwig.dylib.
find_file(HERWIG_LIBRARY
    NAMES Herwig.so Herwig.dylib
          libHerwig.dylib libHerwig.so libHerwig.a
    HINTS ${_hw_hints} ${HERWIG_LIBDIR}
    PATH_SUFFIXES lib lib64 lib/Herwig lib64/Herwig)

# ----- Aggregate into the public variables -----
set(THEPEG_INCLUDE_DIRS ${THEPEG_INCLUDE_DIR})
set(THEPEG_LIBRARIES    ${THEPEG_LIBRARY})
set(HERWIG_INCLUDE_DIRS ${HERWIG_INCLUDE_DIR} ${THEPEG_INCLUDE_DIR})
set(HERWIG_LIBRARIES    ${HERWIG_LIBRARY} ${THEPEG_LIBRARY})

find_package_handle_standard_args(Herwig
    REQUIRED_VARS HERWIG_LIBRARY THEPEG_LIBRARY
                  HERWIG_INCLUDE_DIR THEPEG_INCLUDE_DIR
    VERSION_VAR   HERWIG_VERSION)

# ----- Helper: extract -I dirs from a cppflags string ---------------------
# `thepeg-config --cppflags` and `herwig-config --cppflags` both return
# strings like
#   -I$HW_PREFIX/include  -I/opt/homebrew/opt/boost/include
#   -I/opt/homebrew/opt/gsl/include
# These transitive include paths are CRITICAL — ThePEG's
# RandomGenerator.h includes <gsl/gsl_rng.h>, and Particle.h pulls
# <boost/...>.  Without extracting them into INTERFACE_INCLUDE_DIRECTORIES
# the consumer's compile fails with "'gsl/gsl_rng.h' file not found".
function(_cap_extract_includes outvar cppflags)
    set(_dirs)
    if(cppflags)
        separate_arguments(_tokens NATIVE_COMMAND "${cppflags}")
        foreach(_tok ${_tokens})
            if(_tok MATCHES "^-I(.+)$")
                list(APPEND _dirs "${CMAKE_MATCH_1}")
            endif()
        endforeach()
    endif()
    set(${outvar} "${_dirs}" PARENT_SCOPE)
endfunction()

_cap_extract_includes(_thepeg_extra_includes "${THEPEG_CPPFLAGS}")
_cap_extract_includes(_herwig_extra_includes "${HERWIG_CPPFLAGS}")

# ----- Imported targets — chain Herwig → ThePEG -----
if(HERWIG_FOUND)
    if(NOT TARGET Herwig::ThePEG)
        add_library(Herwig::ThePEG UNKNOWN IMPORTED)
        set_target_properties(Herwig::ThePEG PROPERTIES
            IMPORTED_LOCATION             "${THEPEG_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${THEPEG_INCLUDE_DIR};${_thepeg_extra_includes}")
        # Critical (§5.2 obs #3): forward thepeg-config --ldflags so
        # rpath entries propagate.  We pass them as link options on the
        # imported target so consumers don't need to know the details.
        if(THEPEG_LDFLAGS)
            separate_arguments(_thepeg_ldflags_list NATIVE_COMMAND "${THEPEG_LDFLAGS}")
            set_property(TARGET Herwig::ThePEG PROPERTY
                INTERFACE_LINK_OPTIONS ${_thepeg_ldflags_list})
        endif()
        if(THEPEG_LDLIBS)
            separate_arguments(_thepeg_ldlibs_list NATIVE_COMMAND "${THEPEG_LDLIBS}")
            set_property(TARGET Herwig::ThePEG PROPERTY
                INTERFACE_LINK_LIBRARIES ${_thepeg_ldlibs_list})
        endif()
    endif()
    if(NOT TARGET Herwig::Herwig)
        # IMPORTANT: Herwig.so is loaded at runtime via dlopen (per
        # INSTALL_REPORT_HERWIG.md §5.5).  herwig-config --ldlibs
        # returns -lThePEG — there is no -lHerwig.  So this target is
        # INTERFACE-only: it propagates the Herwig include path and
        # depends on Herwig::ThePEG, but does NOT pass Herwig.so to
        # the linker.  HERWIG_LIBRARY is still recorded as
        # IMPORTED_LOCATION_FROM_FILE_PATH for documentation /
        # diagnostics — runtime users see it in their build summary.
        add_library(Herwig::Herwig INTERFACE IMPORTED)
        # Also bake -I paths from herwig-config --cppflags so consumer
        # code can include Herwig/Boost/GSL headers transparently.
        set_target_properties(Herwig::Herwig PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${HERWIG_INCLUDE_DIR};${_herwig_extra_includes}"
            INTERFACE_LINK_LIBRARIES      "Herwig::ThePEG")
        if(HERWIG_LDFLAGS)
            separate_arguments(_herwig_ldflags_list NATIVE_COMMAND "${HERWIG_LDFLAGS}")
            set_property(TARGET Herwig::Herwig PROPERTY
                INTERFACE_LINK_OPTIONS ${_herwig_ldflags_list})
        endif()
        # Record where Herwig.so lives so callers can pass it to dlopen
        # (e.g. CAP::HerwigEventGenerator::initialize() does this).  The
        # property name is custom — read with get_target_property().
        set_property(TARGET Herwig::Herwig PROPERTY
            HERWIG_RUNTIME_PLUGIN "${HERWIG_LIBRARY}")
    endif()
endif()

mark_as_advanced(THEPEG_CONFIG_EXECUTABLE HERWIG_CONFIG_EXECUTABLE
                 THEPEG_INCLUDE_DIR THEPEG_LIBRARY
                 HERWIG_INCLUDE_DIR HERWIG_LIBRARY)
