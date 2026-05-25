################################################################################
# CAP / EventHistory — generator-agnostic event-history (provenance) graph
#
# Phase 1 of the parton-tracking feature.  Builds libEventHistory:
#   * EventHistory DAG + StageTaxonomy + HistoryCapability   (always)
#   * PythiaHistoryBuilder    — only when CAP_ENABLE_PYTHIA=ON
#   * HepMC3HistoryBuilder    — only when CAP_ENABLE_HEPMC3=ON
#
# The core is plain C++14 — no external dependencies, no ROOT
# dictionary.  No CAP analysis task uses this module yet: Phase 1 is
# infrastructure only, so building it cannot change existing behaviour.
################################################################################

set(_eventhistory_sources
    EventHistory.cpp
    StageSelector.cpp
    ProvenanceTagger.cpp
    ProvenanceObservables.cpp
    PairProvenanceObservables.cpp)
set(_eventhistory_libs)

if(CAP_ENABLE_PYTHIA AND PYTHIA8_FOUND)
    list(APPEND _eventhistory_sources PythiaHistoryBuilder.cpp)
    list(APPEND _eventhistory_libs    Pythia8::Pythia8)
endif()

if(CAP_ENABLE_HEPMC3 AND HEPMC3_FOUND)
    list(APPEND _eventhistory_sources HepMC3HistoryBuilder.cpp)
    list(APPEND _eventhistory_libs    HepMC3::HepMC3)
endif()

add_library(EventHistory SHARED ${_eventhistory_sources})

if(_eventhistory_libs)
    target_link_libraries(EventHistory PUBLIC ${_eventhistory_libs})
endif()

target_include_directories(EventHistory
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS EventHistory LIBRARY DESTINATION ${CAP_INSTALL_LIBDIR})

# ---------------------------------------------------------------------------
# Phase 3b — provenance-study : a self-contained Pythia + ROOT runner that
# produces the provenance-decomposed Single/Global observables.  Built only
# when Pythia is available (it generates its own events).
# ---------------------------------------------------------------------------
if(CAP_ENABLE_PYTHIA AND PYTHIA8_FOUND)
    add_executable(provenance-study provenance-study.cpp)
    target_link_libraries(provenance-study
        PRIVATE EventHistory Pythia8::Pythia8 ${ROOT_LIBRARIES})
    install(TARGETS provenance-study
            RUNTIME DESTINATION ${CAP_INSTALL_BINDIR})
endif()

# ---------------------------------------------------------------------------
# provenance-report : assembles the provenance report (paper + presentation
# modes) via CAP's src/Latex module.  Reads only plain-text inputs, so it
# links the Latex module but needs neither Pythia nor the EventHistory lib.
# ---------------------------------------------------------------------------
add_executable(provenance-report provenance-report.cpp)
target_link_libraries(provenance-report PRIVATE Latex ${ROOT_LIBRARIES})
install(TARGETS provenance-report RUNTIME DESTINATION ${CAP_INSTALL_BINDIR})
