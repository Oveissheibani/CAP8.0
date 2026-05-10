################################################################################################
# Project CAP/ParticleSingle
################################################################################################

#include(${ROOT_USE_FILE})
#include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
#add_definitions(${ROOT_CXX_FLAGS})

ROOT_GENERATE_DICTIONARY(G__ParticleSingle ParticleSingleHistos.hpp ParticleSingleDerivedHistos.hpp  ParticleSingleAnalyzer.hpp
LINKDEF ParticleSingleLinkDef.h)


################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_compile_options(-Wall -Wextra -pedantic)
add_library(ParticleSingle SHARED   ParticleSingleHistos.cpp ParticleSingleDerivedHistos.cpp  ParticleSingleAnalyzer.cpp G__ParticleSingle.cxx)

message(STATUS "ROOT_LIBRARIES: ${ROOT_LIBRARIES}")
message(STATUS "EXTRA_LIBS: ${EXTRA_LIBS}")
message(STATUS "EXTRA_INCLUDES: ${EXTRA_INCLUDES}")
target_link_libraries(ParticleSingle Base   Particles  ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(ParticleSingle  PUBLIC Base    Particles ParticleSingle ${EXTRA_INCLUDES} )

# optimization for big histogram access within the pair inner loop
# WARNING: the sumw2 structure is not used -> estimate uncertainties using sub-samples
add_definitions(-DOPTIMIZEADDBINCONTENT)


install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libParticleSingle.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libParticleSingle_rdict.pcm" DESTINATION ${CAP_INSTALL_LIBDIR})
install(TARGETS ParticleSingle  LIBRARY DESTINATION ${CAP_INSTALL_LIBDIR})

