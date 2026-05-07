################################################################################################
# Project CAP/PtPt
################################################################################################
ROOT_GENERATE_DICTIONARY(G__PtPt
PtPtAnalyzer.hpp
PtPtDerivedHistos.hpp
PtPtHistos.hpp
LINKDEF PtPtLinkDef.h)

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_compile_options(-Wall -Wextra -pedantic)
add_library(PtPt SHARED
PtPtAnalyzer.cpp
PtPtDerivedHistos.cpp
PtPtHistos.cpp
G__PtPt.cxx)

target_link_libraries(PtPt Base  Particles  ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(PtPt  PUBLIC Base  Particles  PtPt ${EXTRA_INCLUDES} ) 

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libPtPt.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libPtPt_rdict.pcm" DESTINATION ${CAP_INSTALL_LIBDIR})
install(TARGETS PtPt  LIBRARY DESTINATION ${CAP_INSTALL_LIBDIR})

