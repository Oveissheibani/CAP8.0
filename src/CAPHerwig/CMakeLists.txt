################################################################################
# CAP / CAPHerwig — embedded HERWIG 7 + ThePEG event generator wrapper
#
# Mirrors src/CAPPythia/CMakeLists.txt.  Built only when CAP_ENABLE_HERWIG
# is ON.  find_package(Herwig) at the top level sets up the imported
# targets Herwig::Herwig + Herwig::ThePEG with the right rpath baked in
# (per INSTALL_REPORT_HERWIG.md §5.2 critical observation #3).
#
# The class CAP::HerwigEventGenerator embeds ThePEG's EventGenerator
# API in-process and converts ThePEG events into CAP::Event objects so
# any CAP analyzer (Single / Pair / Pair3D / Global / NuDyn / Spherocity
# / PtPt) can consume Herwig output the same way it consumes Pythia.
################################################################################

ROOT_GENERATE_DICTIONARY(G__CAPHerwig
    HerwigEventGenerator.hpp
    LINKDEF CAPHerwigLinkDef.h)

add_library(CAPHerwig SHARED
    HerwigEventGenerator.cpp
    G__CAPHerwig.cxx)

# Link against the imported targets created by FindHerwig.cmake.  These
# already carry the right rpath flags from herwig-config / thepeg-config.
target_link_libraries(CAPHerwig
    PUBLIC  Base Particles
            Herwig::Herwig
            Herwig::ThePEG
            ${CMAKE_DL_LIBS}            # for dlopen() on Herwig.so
            ${ROOT_LIBRARIES} ${EXTRA_LIBS})

target_include_directories(CAPHerwig
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}
            ${HERWIG_INCLUDE_DIRS}
            ${THEPEG_INCLUDE_DIRS}
            ${EXTRA_INCLUDES})

# The HepMC3 dependency is transitive through Herwig → ThePEG, but if
# our consumer code ever directly references HepMC3 types we want it
# explicit.  Don't add it unless CAP_ENABLE_HEPMC3 is ON.
if(CAP_ENABLE_HEPMC3 AND TARGET HepMC3::HepMC3)
    target_link_libraries(CAPHerwig PUBLIC HepMC3::HepMC3)
endif()

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/libCAPHerwig.rootmap"
    "${CMAKE_CURRENT_BINARY_DIR}/libCAPHerwig_rdict.pcm"
    DESTINATION ${CAP_INSTALL_LIBDIR})
install(TARGETS CAPHerwig LIBRARY DESTINATION ${CAP_INSTALL_LIBDIR})
