################################################################################################
# Project CAP/BasicEventGen
################################################################################################

ROOT_GENERATE_DICTIONARY(G__BasicEventGen
BasicEventGen.hpp
LINKDEF BasicEventGenLinkDef.h)

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_library(BasicEventGen SHARED
BasicEventGen.cpp
G__BasicEventGen.cxx)
target_link_libraries(BasicEventGen  Base  Particles   ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(BasicEventGen  PUBLIC  Base Math Particles ${EXTRA_INCLUDES})

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libBasicEventGen.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libBasicEventGen_rdict.pcm" DESTINATION ${CAP_INSTALL_LIBDIR})
install(TARGETS BasicEventGen  LIBRARY DESTINATION ${CAP_INSTALL_LIBDIR}) 
