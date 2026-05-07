################################################################################
# CAP / CAPHepMC3 — HepMC3 event-record reader
#
# Reads HepMC3 events written by HERWIG (or any other HepMC3 producer:
# EPOS, MadGraph, Sherpa, …) and feeds them into the CAP event stream.
# Same role as CAPPythia, but for events that already exist on disk
# rather than ones generated in-process.
#
# This subdirectory is only added by src/CMakeLists.txt when
# CAP_ENABLE_HEPMC3=ON.  The find_package(HepMC3) is performed at the
# top level (where it's gated by the option) so the failure mode is
# clear and the imported target HepMC3::HepMC3 is already populated by
# the time we get here.
################################################################################

ROOT_GENERATE_DICTIONARY(G__CAPHepMC3
    HepMC3EventReader.hpp
    LINKDEF CAPHepMC3LinkDef.h)

add_library(CAPHepMC3 SHARED
    HepMC3EventReader.cpp
    G__CAPHepMC3.cxx)

target_link_libraries(CAPHepMC3
    PUBLIC  Base Particles
            HepMC3::HepMC3
            ${ROOT_LIBRARIES} ${EXTRA_LIBS})

target_include_directories(CAPHepMC3
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}
            ${EXTRA_INCLUDES})

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/libCAPHepMC3.rootmap"
    "${CMAKE_CURRENT_BINARY_DIR}/libCAPHepMC3_rdict.pcm"
    DESTINATION ${CAP_INSTALL_LIBDIR})
install(TARGETS CAPHepMC3 LIBRARY DESTINATION ${CAP_INSTALL_LIBDIR})
