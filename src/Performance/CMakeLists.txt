################################################################################################
# Project CAP/Performance
################################################################################################
#include(${ROOT_USE_FILE})
#include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
#add_definitions(${ROOT_CXX_FLAGS})

ROOT_GENERATE_DICTIONARY(G__Performance
ClosureCalculator.hpp
ClosureIterator.hpp
EventPlaneRandomizerTask.hpp
EventVertexRandomizerTask.hpp
MeasurementSimulator.hpp
ParticlePerformanceHistos.hpp
ParticlePerformanceDerivedHistos.hpp
PerformanceAnalyzer.hpp
RunClosureAnalysis.hpp
LINKDEF PerformanceLinkDef.h)

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_compile_options(-Wall -Wextra -pedantic)
add_library(Performance SHARED
ClosureCalculator.cpp
ClosureIterator.cpp
EventPlaneRandomizerTask.cpp
EventVertexRandomizerTask.cpp
MeasurementSimulator.cpp
ParticlePerformanceHistos.cpp
ParticlePerformanceDerivedHistos.cpp
PerformanceAnalyzer.cpp
RunClosureAnalysis.cpp
G__Performance.cxx)

target_link_libraries(Performance Base  Particles  ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(Performance  PUBLIC Performance Base  Particles   ${EXTRA_INCLUDES} )

# optimization for big histogram access within the pair inner loop
add_definitions(-DOPTIMIZEADDBINCONTENT)

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libPerformance.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libPerformance_rdict.pcm" DESTINATION ${CAP_INSTALL_LIBDIR})
install(TARGETS Performance  LIBRARY DESTINATION ${CAP_INSTALL_LIBDIR})

