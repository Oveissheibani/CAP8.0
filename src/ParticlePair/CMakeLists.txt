################################################################################################
# Project CAP/ParticlePair
################################################################################################

#include(${ROOT_USE_FILE})
#include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
#add_definitions(${ROOT_CXX_FLAGS})

ROOT_GENERATE_DICTIONARY(
G__ParticlePair
ParticlePairAnalyzer.hpp
ParticlePairBfHistos.hpp
ParticlePairDerivedHistos.hpp
ParticlePairHistos.hpp
LINKDEF ParticlePairLinkDef.h)

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_compile_options(-Wall -Wextra -pedantic)
add_library(ParticlePair SHARED
ParticlePairAnalyzer.cpp
ParticlePairBfHistos.cpp
ParticlePairDerivedHistos.cpp
ParticlePairHistos.cpp
G__ParticlePair.cxx)

target_link_libraries(ParticlePair Base   Particles ParticleSingle ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(ParticlePair  PUBLIC Base Particles ParticleSingle ParticlePair ${EXTRA_INCLUDES} )

# optimization for big histogram access within the pair inner loop
# WARNING: the sumw2 structure is not used -> estimate uncertainties using sub-samples
add_definitions(-DOPTIMIZEADDBINCONTENT)


install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libParticlePair.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libParticlePair_rdict.pcm" DESTINATION ${CAP_INSTALL_LIBDIR})
install(TARGETS ParticlePair  LIBRARY DESTINATION ${CAP_INSTALL_LIBDIR})

