################################################################################
# CAP / CAPPythia — Pythia 8 event-generator wrapper
################################################################################

# NOTE: PythiaEventReader.{hpp,cpp} is broken legacy code (refers to
# ProcessorSingle, ParticleTypeList::list(), and TTree branches that no longer
# exist). It's intentionally excluded from the build until somebody decides to
# either repair or delete it. PythiaEventGenerator covers the live workflow.
ROOT_GENERATE_DICTIONARY(G__CAPPythia
    PythiaEventGenerator.hpp
    LINKDEF CAPPythiaLinkDef.h)

add_library(CAPPythia SHARED
    PythiaEventGenerator.cpp
    G__CAPPythia.cxx)

target_link_libraries(CAPPythia
    PUBLIC  Base Particles
            Pythia8::Pythia8
            ${ROOT_LIBRARIES} ${EXTRA_LIBS})

target_include_directories(CAPPythia
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}
            ${EXTRA_INCLUDES})

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/libCAPPythia.rootmap"
    "${CMAKE_CURRENT_BINARY_DIR}/libCAPPythia_rdict.pcm"
    DESTINATION ${CAP_INSTALL_LIBDIR})
install(TARGETS CAPPythia LIBRARY DESTINATION ${CAP_INSTALL_LIBDIR})
