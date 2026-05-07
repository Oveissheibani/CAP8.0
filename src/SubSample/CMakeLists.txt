################################################################################################
# Project CAP/SubSample
################################################################################################
#include(${ROOT_USE_FILE})
#include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
#add_definitions(${ROOT_CXX_FLAGS})

ROOT_GENERATE_DICTIONARY(G__SubSample
SubSampleStatCalculator.hpp
RunSubSampleSum.hpp
LINKDEF SubSampleLinkDef.h)


################################################################################################
# Create a shared library with generated dictionary
################################################################################################
add_compile_options(-Wall -Wextra -pedantic)
add_library(SubSample SHARED
SubSampleStatCalculator.cpp
RunSubSampleSum.cpp
G__SubSample.cxx)

target_link_libraries(SubSample Base  Particles  ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(SubSample  PUBLIC Base  Particles SubSample ${EXTRA_INCLUDES} )

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libSubSample.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libSubSample_rdict.pcm" DESTINATION ${CAP_INSTALL_LIBDIR})
install(TARGETS SubSample  LIBRARY DESTINATION ${CAP_INSTALL_LIBDIR})

