################################################################################################
# Project CAP/Spherocity
################################################################################################

ROOT_GENERATE_DICTIONARY(G__Spherocity
SpherocityDerivedHistos.hpp
SpherocityHistos.hpp
SpherocityAnalyzer.hpp
LINKDEF SpherocityLinkDef.h)


################################################################################################
# Create a shared library with generated dictionary
################################################################################################
add_compile_options(-Wall -Wextra -pedantic)
add_library(Spherocity SHARED
SpherocityDerivedHistos.cpp
SpherocityHistos.cpp
SpherocityAnalyzer.cpp
G__Spherocity.cxx)

target_link_libraries(Spherocity Base  Particles  ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(Spherocity  PUBLIC Spherocity Base  Particles  ${EXTRA_INCLUDES} )

# optimization for big histogram access within the pair inner loop
# WARNING: the sumw2 structure is not used -> estimate uncertainties using sub-samples
add_definitions(-DOPTIMIZEADDBINCONTENT)


install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libSpherocity.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libSpherocity_rdict.pcm" DESTINATION ${CAP_INSTALL_LIBDIR})
install(TARGETS Spherocity  LIBRARY DESTINATION ${CAP_INSTALL_LIBDIR})

