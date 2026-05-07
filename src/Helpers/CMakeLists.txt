################################################################################################
# Project CAP/Helpers
################################################################################################

#include(${ROOT_USE_FILE})
#include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
#add_definitions(${ROOT_CXX_FLAGS})

ROOT_GENERATE_DICTIONARY(G__Helpers
CanvasConfiguration.hpp
CanvasList.hpp
GraphConfiguration.hpp
RootChainManager.hpp
LINKDEF HelpersLinkDef.h)

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_compile_options(-Wall -Wextra -pedantic)
add_library(Helpers SHARED
CanvasConfiguration.cpp
CanvasList.cpp
GraphConfiguration.cpp
RootChainManager.cpp
RootFileHelpers.cpp
RootHistogramHelpers.cpp
G__Helpers.cxx)

target_link_libraries(Helpers Base Math ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(Helpers PUBLIC Base Math ${EXTRA_INCLUDES} )

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libHelpers.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libHelpers_rdict.pcm" DESTINATION ${CAP_INSTALL_LIBDIR})
install(TARGETS Helpers  LIBRARY DESTINATION ${CAP_INSTALL_LIBDIR})

