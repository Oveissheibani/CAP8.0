################################################################################################
# Project CAP/Plotting
################################################################################################
ROOT_GENERATE_DICTIONARY(G__Plotting CanvasList.hpp CanvasConfiguration.hpp DataGraph.hpp FunctionList.hpp GraphConfiguration.hpp  Plotter.hpp  LegendConfiguration.hpp LINKDEF PlottingLinkDef.h)
#BalFctPlotter.hpp

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_compile_options(-Wall -Wextra -pedantic)
add_library(Plotting SHARED CanvasList.cpp CanvasConfiguration.cpp  DataGraph.cpp   FunctionList.cpp  GraphConfiguration.cpp Plotter.cpp LegendConfiguration.cpp
 G__Plotting.cxx)
 
 # BalFctPlotter.cpp

target_link_libraries(Plotting Base  ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(Plotting  PUBLIC Plotting Base  ${EXTRA_INCLUDES} )

# optimization for big histogram access within the pair inner loop
# WARNING: the sumw2 structure is not used -> estimate uncertainties using sub-samples
add_definitions(-DOPTIMIZEADDBINCONTENT)


install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libPlotting.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libPlotting_rdict.pcm" DESTINATION ${CAP_INSTALL_LIBDIR})
install(TARGETS Plotting  LIBRARY DESTINATION ${CAP_INSTALL_LIBDIR})

