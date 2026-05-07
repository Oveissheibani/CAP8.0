################################################################################################
# Project CAP/Glauber
################################################################################################

#include(${ROOT_USE_FILE})
#include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
#add_definitions(${ROOT_CXX_FLAGS})

ROOT_GENERATE_DICTIONARY(G__Glauber
GlauberAnalyzer.hpp
GlauberDb.hpp
GlauberEvent.hpp
GlauberEventFilter.hpp
GlauberEventMoments.hpp
GlauberGenerator.hpp
GlauberHistos.hpp
GlauberNucleon.hpp
GlauberNucleusType.hpp
GlauberNucleus.hpp
GlauberPoint.hpp
LINKDEF GlauberLinkDef.h)

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_compile_options(-Wall -Wextra -pedantic)
add_library(Glauber SHARED
GlauberAnalyzer.cpp
GlauberDb.cpp
GlauberEvent.cpp
GlauberEventFilter.cpp
GlauberEventMoments.cpp
GlauberExceptions.cpp 
GlauberGenerator.cpp
GlauberHistos.cpp
GlauberNucleon.cpp
GlauberNucleusType.cpp
GlauberNucleus.cpp
GlauberPoint.cpp
NNXSectVsSqrtS.cpp
NNProfile.cpp
NucleonProfile.cpp
NuclearDistribution.cpp
NNXSectVsSqrtS.cpp
PTotProfile.cpp
SmearingProfile.cpp
G__Glauber.cxx)

target_link_libraries(Glauber Base Math Helpers Particles  ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(Glauber PUBLIC Base Math Glauber Helpers ParticleDb Particles ${EXTRA_INCLUDES} )

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libGlauber.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libGlauber_rdict.pcm" DESTINATION ${CAP_INSTALL_LIBDIR})
install(TARGETS Glauber  LIBRARY DESTINATION ${CAP_INSTALL_LIBDIR})

