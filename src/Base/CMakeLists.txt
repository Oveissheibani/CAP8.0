################################################################################################
# Project CAP/Base
################################################################################################

#include(${ROOT_USE_FILE})
#include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
#add_definitions(${ROOT_CXX_FLAGS})

ROOT_GENERATE_DICTIONARY(G__Base
Accountant.hpp
Configuration.hpp
ConfigurationManager.hpp
EnvironmentVariables.hpp
Factory.hpp
Filter.hpp
###MessageLogger.hpp
Object.hpp
Parser.hpp
PrintHelpers.hpp
Properties.hpp
PropertiesParser.hpp
Property.hpp
SelectionGenerator.hpp
StreamHelpers.hpp
Task.hpp
Timer.hpp
LINKDEF BaseLinkDef.h)

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_compile_options(-Wall -Wextra -pedantic)
add_library(Base SHARED
Accountant.cpp
Condition.cpp
Configuration.cpp
ConfigurationManager.cpp
EnvironmentVariables.cpp
Exceptions.cpp
Factory.cpp
Filter.cpp
###MessageLogger.cpp
NameHelpers.cpp
Object.cpp
Parser.cpp
PrintHelpers.cpp
Properties.cpp
PropertiesParser.cpp
Property.cpp
SelectionGenerator.cpp
StreamHelpers.cpp
Task.cpp
Timer.cpp
G__Base.cxx)

target_link_libraries(Base ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(Base PUBLIC Base Math ${EXTRA_INCLUDES} )

# optimization for big histogram access within the pair inner loop
# WARNING: the sumw2 structure is not used -> estimate uncertainties using sub-samples
add_definitions(-DOPTIMIZEADDBINCONTENT)


install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libBase.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libBase_rdict.pcm" DESTINATION ${CAP_INSTALL_LIBDIR})
install(TARGETS Base  LIBRARY DESTINATION ${CAP_INSTALL_LIBDIR})

