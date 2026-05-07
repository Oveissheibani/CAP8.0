################################################################################################
# Project CAP/Particles
################################################################################################

#include(${ROOT_USE_FILE})
#include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
#add_definitions(${ROOT_CXX_FLAGS})

ROOT_GENERATE_DICTIONARY(G__Particles
BaseHistos.hpp
BaseDerivedHistos.hpp
Event.hpp
EventEfficiency.hpp
EventFilter.hpp
EventIterator.hpp
EventMerger.hpp
EventProcessor.hpp
EventSmearer.hpp
FileIterator.hpp
HistogramGroup.hpp
ManagedObject.hpp
Particle.hpp
ParticleDecayMode.hpp
ParticleDb.hpp
ParticleDbParser.hpp
#ParticleDbParser_CS.hpp
#ParticleDbXmlWriter.hpp
ParticleDecayer.hpp
ParticleDecayerTask.hpp
ParticleEfficiency.hpp
ParticleFilter.hpp
ParticlePairFilter.hpp
ParticleSmearer.hpp
ParticleType.hpp
LINKDEF ParticlesLinkDef.h)

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_compile_options(-Wall -Wextra -pedantic)
add_library(Particles SHARED
BaseHistos.cpp
BaseDerivedHistos.cpp
Event.cpp
EventEfficiency.cpp
EventFilter.cpp
EventIterator.cpp
EventMerger.cpp
EventProcessor.cpp
EventSmearer.cpp
FileIterator.cpp
HistogramGroup.cpp
ManagedObject.cpp
Particle.cpp
ParticleDb.cpp
ParticleDbParser.cpp
#ParticleDbParser_CS.cpp
#ParticleDbXmlWriter.cpp
ParticleDecayer.cpp
ParticleDbExceptions.cpp
ParticleDecayerTask.cpp
ParticleDecayMode.cpp
ParticleEfficiency.cpp
ParticleFilter.cpp
ParticlePairFilter.cpp
ParticleSmearer.cpp
ParticleType.cpp
G__Particles.cxx)

target_link_libraries(Particles Base Math Helpers   ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(Particles  PUBLIC Base Math Helpers Particles  ${EXTRA_INCLUDES} )

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libParticles.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libParticles_rdict.pcm" DESTINATION ${CAP_INSTALL_LIBDIR})
install(TARGETS Particles  LIBRARY DESTINATION ${CAP_INSTALL_LIBDIR})

