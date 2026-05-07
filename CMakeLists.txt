################################################################################
# Top-level CMakeLists.txt for CAP — lets you build from the repo root:
#
#   cmake -S . -B build [-D CAP_ENABLE_PYTHIA=ON ...]
#   cmake --build build -j
#   cmake --install build
#
# All CAP-specific options live in src/CMakeLists.txt; this file just forwards
# to that subdirectory and offers an option to compile the vendored FastJet
# source tree alongside CAP.
################################################################################
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(CAP_TopLevel
    VERSION 8.0
    DESCRIPTION "Correlation Analysis Package — wrapper"
    LANGUAGES CXX)

list(PREPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

option(CAP_BUILD_VENDORED_FASTJET
       "Compile the vendored fastjet-3.4.3 source tree as part of the CAP build"
       OFF)

# User-defined analyses live in analyses/stubs/. Built into libCAPAnalyses.
# The default is ON because they fill in the missing orchestrator classes
# (RunAnalysis, ParticleTypeTask, EventFilterCreator, ParticleFilterCreator)
# that every shipped .ini references.
option(CAP_BUILD_USER_ANALYSES
       "Build the user analysis stubs from analyses/stubs/ into libCAPAnalyses"
       ON)

# If we're going to use the vendored FastJet, pre-build it before configuring CAP
# so the find_package call in src/CMakeLists.txt picks it up. We invoke FastJet's
# autotools build via ExternalProject so it doesn't pollute the CAP build graph.
if(CAP_BUILD_VENDORED_FASTJET)
    include(ExternalProject)
    set(_fastjet_src "${CMAKE_SOURCE_DIR}/fastjet-3.4.3")
    set(_fastjet_prefix "${CMAKE_BINARY_DIR}/fastjet-install")
    if(NOT EXISTS "${_fastjet_src}/configure")
        message(FATAL_ERROR
            "CAP_BUILD_VENDORED_FASTJET=ON but ${_fastjet_src} is missing or "
            "doesn't contain a 'configure' script. Either set the option to OFF "
            "or restore the vendored FastJet source.")
    endif()
    ExternalProject_Add(fastjet_vendored
        SOURCE_DIR        ${_fastjet_src}
        CONFIGURE_COMMAND ${_fastjet_src}/configure --prefix=${_fastjet_prefix} --enable-shared
        BUILD_COMMAND     ${CMAKE_MAKE_PROGRAM} -j
        INSTALL_COMMAND   ${CMAKE_MAKE_PROGRAM} install
        BUILD_IN_SOURCE   1
    )
    # Tell the CAP find_package about the freshly-built install location.
    set(CAP_FASTJET_PATH ${_fastjet_prefix} CACHE PATH
        "Auto-built FastJet location" FORCE)
    list(APPEND CMAKE_PREFIX_PATH ${_fastjet_prefix})
endif()

add_subdirectory(src)
# analyses/stubs/ is added from inside src/CMakeLists.txt (after
# find_package(ROOT) has run, so the ROOT include paths and the
# ROOT_GENERATE_DICTIONARY macro are in scope).
