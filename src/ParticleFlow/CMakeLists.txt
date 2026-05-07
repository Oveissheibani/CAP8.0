################################################################################################
# Project CAP/ParticleFlow
################################################################################################

#include(${ROOT_USE_FILE})
#include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
#add_definitions(${ROOT_CXX_FLAGS})

ROOT_GENERATE_DICTIONARY(G__ParticleFlow ParticleFlowHistos.hpp ParticleFlowDerivedHistos.hpp  ParticleFlowAnalyzer.hpp
LINKDEF ParticleFlowLinkDef.h)


################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_compile_options(-Wall -Wextra -pedantic)
add_library(ParticleFlow SHARED   ParticleFlowHistos.cpp ParticleFlowDerivedHistos.cpp  ParticleFlowAnalyzer.cpp   G__ParticleFlow.cxx)

target_link_libraries(ParticleFlow Base  Particles  ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(ParticleFlow  PUBLIC Base   Particles ParticleFlow ${EXTRA_INCLUDES} )

# optimization for big histogram access within the pair inner loop
# WARNING: the sumw2 structure is not used -> estimate uncertainties using sub-samples
add_definitions(-DOPTIMIZEADDBINCONTENT)


install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libParticleFlow.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libParticleFlow_rdict.pcm" DESTINATION ${CAP_INSTALL_LIBDIR})
install(TARGETS ParticleFlow  LIBRARY DESTINATION ${CAP_INSTALL_LIBDIR})

