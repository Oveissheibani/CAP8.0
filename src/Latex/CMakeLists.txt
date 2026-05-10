################################################################################################
# Project CAP/Latex
################################################################################################

#include(${ROOT_USE_FILE})
#include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
#add_definitions(${ROOT_CXX_FLAGS})

ROOT_GENERATE_DICTIONARY(G__Latex
LatexAuthor.hpp
LatexCommand.hpp
LatexDocument.hpp
LatexElement.hpp
LatexEquation.hpp
LatexFigure.hpp
LatexFigureGroup.hpp
LatexFrame.hpp
LatexList.hpp
LatexPackage.hpp
LatexParagraph.hpp
LatexSection.hpp
LatexSubsection.hpp
LatexSubsubsection.hpp
LatexText.hpp
LINKDEF LatexLinkDef.h)

################################################################################################
# Create a shared library with geneated dictionary
################################################################################################
add_compile_options(-Wall -Wextra -pedantic)
add_library(Latex SHARED
LatexAuthor.cpp
LatexCommand.cpp
LatexDocument.cpp
LatexElement.cpp
LatexEquation.cpp
LatexFigure.cpp
LatexFigureGroup.cpp
LatexFrame.cpp
LatexList.cpp
LatexPackage.cpp
LatexParagraph.cpp
LatexSection.cpp
LatexSubsection.cpp
LatexSubsubsection.cpp
LatexText.cpp
G__Latex.cxx)

target_link_libraries(Latex Base Math Helpers ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(Latex PUBLIC Base Math ${EXTRA_INCLUDES} )

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libLatex.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libLatex_rdict.pcm" DESTINATION "${CAP_INSTALL_LIBDIR}")
install(TARGETS Latex LIBRARY DESTINATION "${CAP_INSTALL_LIBDIR}")

