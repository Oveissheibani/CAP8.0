################################################################################################
# Project CAP/Math
################################################################################################
ROOT_GENERATE_DICTIONARY(G__Math
Complex.hpp
#Histogram.hpp
#Partition.hpp
Vector3D.hpp
VectorLorentz.hpp
ScalarField.hpp
LINKDEF MathLinkDef.h)

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_compile_options(-Wall -Wextra -pedantic)
add_library(Math SHARED
BesselFunctions.cpp
Complex.cpp
#Histogram.cpp
MathBasicFunctions.cpp
MathBasicGeometry.cpp
MathBasicInterpolation.cpp
MathBasicIntegration.cpp
MathBasicSearch.cpp
MathBasicSortingFunctions.cpp
MathBasicStatistics.cpp
MathConstants.cpp
MathExceptions.cpp
#Partition.cpp
Vector3D.cpp
VectorLorentz.cpp
ScalarField.cpp
G__Math.cxx)

target_link_libraries(Math Base ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(Math  PUBLIC Base ${EXTRA_INCLUDES} )

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libMath.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libMath_rdict.pcm" DESTINATION ${CAP_INSTALL_LIBDIR})
install(TARGETS Math  LIBRARY DESTINATION ${CAP_INSTALL_LIBDIR})

