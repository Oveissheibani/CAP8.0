################################################################################
# analyses/stubs/CMakeLists.txt
#
# Builds libCAPAnalyses with the orchestrator-class stubs that the shipped
# .ini files (and everything emitted by build-ini-gui) reference but that are
# missing from src/. See MISSING_CLASSES.md for context.
#
# This subdir is added by src/CMakeLists.txt only when the option
# CAP_BUILD_USER_ANALYSES is ON (default ON when building from the top
# level). Nothing in src/ depends on it, so disabling it is safe.
################################################################################

# Most stubs derive from CAP::Task (libBase).  The Calculator subclasses
# (Single/Pair/Pair3D/Global) inherit from the per-module Analyzer
# classes — pull in the analyzer headers + libraries so they build.
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/Base
    ${CMAKE_SOURCE_DIR}/src/Math
    ${CMAKE_SOURCE_DIR}/src/Helpers
    ${CMAKE_SOURCE_DIR}/src/Particles
    ${CMAKE_SOURCE_DIR}/src/ParticleSingle
    ${CMAKE_SOURCE_DIR}/src/ParticlePair
    ${CMAKE_SOURCE_DIR}/src/ParticlePair3D
    ${CMAKE_SOURCE_DIR}/src/Global
    ${ROOT_INCLUDE_DIRS}
)

set(_cap_stub_headers
    RunAnalysis.hpp
    ParticleTypeTask.hpp
    ParticleDbTask.hpp
    EventFilterCreator.hpp
    ParticleFilterCreator.hpp
    FilterCreator.hpp
    ParticleSingleCalculator.hpp
    ParticlePairCalculator.hpp
    ParticlePair3DCalculator.hpp
    ParticlePair3DBfCalculator.hpp
    GlobalCalculator.hpp
    # 1D-Pair BF stub: file deliberately renamed Stub*.hpp so rootcling
    # parses our stub instead of the broken shipped
    # src/ParticlePair/ParticlePairBfCalculator.hpp (which #includes a
    # missing 'Calculator.hpp').  The C++ class name is unchanged.
    StubParticlePairBfCalculator.hpp
)
# Note: CAP::FileIterator is provided by libParticles — do NOT compile a stub.

set(_cap_stub_sources
    RunAnalysis.cpp
    ParticleTypeTask.cpp
    ParticleDbTask.cpp
    EventFilterCreator.cpp
    ParticleFilterCreator.cpp
    FilterCreator.cpp
    ParticleSingleCalculator.cpp
    ParticlePairCalculator.cpp
    ParticlePair3DCalculator.cpp
    ParticlePair3DBfCalculator.cpp
    GlobalCalculator.cpp
    StubParticlePairBfCalculator.cpp
)

ROOT_GENERATE_DICTIONARY(G__CAPAnalyses
    ${_cap_stub_headers}
    LINKDEF CAPAnalysesLinkDef.h)

add_library(CAPAnalyses SHARED
    ${_cap_stub_sources}
    G__CAPAnalyses.cxx)

target_link_libraries(CAPAnalyses
    PUBLIC  Base Particles
            ParticleSingle ParticlePair ParticlePair3D Global
            ${ROOT_LIBRARIES} ${EXTRA_LIBS})

target_include_directories(CAPAnalyses
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}
            ${EXTRA_INCLUDES})

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/libCAPAnalyses.rootmap"
    "${CMAKE_CURRENT_BINARY_DIR}/libCAPAnalyses_rdict.pcm"
    DESTINATION ${CAP_INSTALL_LIBDIR})
install(TARGETS CAPAnalyses LIBRARY DESTINATION ${CAP_INSTALL_LIBDIR})
