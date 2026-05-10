###############################################################################################
# Project CAP/Therminator3
###############################################################################################
ROOT_GENERATE_DICTIONARY(G__Therminator3
Chemistry.hpp
Hypersurface.hpp
Model.hpp
ModelBlastWave.hpp
ModelBWA.hpp
ModelHydro2D.hpp
ModelHydro3D.hpp
ModelKrakowSFO.hpp
ModelHadronGas.hpp
ModelSettings.hpp
ThermalDerivedHistos.hpp
ThermalHadron.hpp
ThermalHadronGas.hpp
ThermalHadronGasFD.hpp
ThermalHistos.hpp
ThermalModel.hpp
Therminator3.hpp
Thermodynamics.hpp
Viscosity.hpp
LINKDEF Therminator3LinkDef.h)

add_compile_options(-Wall -Wextra -pedantic)
add_library(Therminator3 SHARED
Chemistry.cpp
Hypersurface.cpp
Model.cpp
ModelBlastWave.cpp
ModelBWA.cpp
ModelHydro2D.cpp
ModelHydro3D.cpp
ModelKrakowSFO.cpp
ModelHadronGas.cpp
ModelSettings.cpp
ThermalDerivedHistos.cpp
ThermalHadron.cpp
ThermalHadronGas.cpp
ThermalHadronGasFD.cpp
ThermalHistos.cpp
ThermalModel.cpp
Therminator3.cpp
Thermodynamics.cpp
Viscosity.cpp
G__Therminator3.cxx)

target_link_libraries(Therminator3 Base Math Helpers Xml  Particles  ${ROOT_LIBRARIES} ${EXTRA_LIBS} )
target_include_directories(Therminator3  PUBLIC  Base Math Helpers Xml Particles  ${EXTRA_INCLUDES} )
install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/libTherminator3.rootmap" "${CMAKE_CURRENT_BINARY_DIR}/libTherminator3_rdict.pcm" DESTINATION ${CAP_INSTALL_LIBDIR})
install(TARGETS Therminator3  LIBRARY DESTINATION ${CAP_INSTALL_LIBDIR})
