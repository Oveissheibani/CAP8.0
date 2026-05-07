################################################################################
# CAP / Jets — FastJet-based jet reconstruction
################################################################################

ROOT_GENERATE_DICTIONARY(G__Jets
    JetHistos.hpp JetSingleHistos.hpp JetPairHistos.hpp JetAnalyzer.hpp
    JetHistosDerived.hpp JetSingleHistosDerived.hpp JetPairHistosDerived.hpp
    LINKDEF JetLinkDef.h)

add_library(Jets SHARED
    JetHistos.cpp JetSingleHistos.cpp JetPairHistos.cpp
    JetHelpers.cpp JetAnalyzer.cpp
    JetHistosDerived.cpp JetSingleHistosDerived.cpp JetPairHistosDerived.cpp
    G__Jets.cxx)

target_link_libraries(Jets
    PUBLIC  Base Particles CAPPythia
            FastJet::FastJet
            Pythia8::Pythia8
            ${ROOT_LIBRARIES} ${EXTRA_LIBS})

target_include_directories(Jets
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}
            ${EXTRA_INCLUDES})

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/libJets.rootmap"
    "${CMAKE_CURRENT_BINARY_DIR}/libJets_rdict.pcm"
    DESTINATION ${CAP_INSTALL_LIBDIR})
install(TARGETS Jets LIBRARY DESTINATION ${CAP_INSTALL_LIBDIR})
