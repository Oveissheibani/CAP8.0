################################################################################################
# Project CAP/PHSD
################################################################################################

ROOT_GENERATE_DICTIONARY(G__PHSD PHSDEventReader.hpp LINKDEF PHSDLinkDef.h)

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_library(PHSD SHARED PHSDEventReader.cpp G__PHSD.cxx)
target_link_libraries(PHSD Base Particles ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(PHSD  PUBLIC Base  Particles  Cluster PHSD ${EXTRA_INCLUDES})

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libPHSD.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libPHSD_rdict.pcm" DESTINATION ${CAP_INSTALL_LIBDIR})
install(TARGETS PHSD  LIBRARY DESTINATION ${CAP_INSTALL_LIBDIR})

