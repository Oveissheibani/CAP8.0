################################################################################################
# Project CAP/NuDyn
################################################################################################
ROOT_GENERATE_DICTIONARY(G__NuDyn
NuDynAnalyzer.hpp
NuDynDerivedHistos.hpp
NuDynHistos.hpp
LINKDEF NuDynLinkDef.h)

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_compile_options(-Wall -Wextra -pedantic)
add_library(NuDyn SHARED
NuDynAnalyzer.cpp
NuDynDerivedHistos.cpp
NuDynHistos.cpp
G__NuDyn.cxx)

target_link_libraries(NuDyn Base  Particles  ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(NuDyn  PUBLIC Base  Particles  NuDyn ${EXTRA_INCLUDES} ) 

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libNuDyn.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libNuDyn_rdict.pcm" DESTINATION ${CAP_INSTALL_LIBDIR})
install(TARGETS NuDyn  LIBRARY DESTINATION ${CAP_INSTALL_LIBDIR})

