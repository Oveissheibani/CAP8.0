#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ class CAP::Filter<CAP::GlauberEvent>+;
#pragma link C++ class CAP::GlauberAnalyzer+;
#pragma link C++ class CAP::GlauberDb+;
// CAP::GlauberExceptions has no class definition in src/Glauber — link rule
// removed to silence "Unused class rule" dictionary warning.
//#pragma link C++ class CAP::GlauberExceptions+;
#pragma link C++ class CAP::GlauberEvent+;
#pragma link C++ class CAP::GlauberEventFilter+;
#pragma link C++ class CAP::GlauberGenerator+;
#pragma link C++ class CAP::GlauberHistos+;
#pragma link C++ class CAP::GlauberNucleon+;
#pragma link C++ class CAP::GlauberNucleus+;
#pragma link C++ class CAP::GlauberNucleusType+;
#pragma link C++ class CAP::GlauberPoint+;
#pragma link C++ class CAP::GlauberEventMoments+;
#pragma link C++ class CAP::ManagedObjects<CAP::GlauberDb>+;
#pragma link C++ class CAP::ManagedObjects<CAP::GlauberEvent>+;
#pragma link C++ class CAP::ManagedObjects<CAP::GlauberEventFilter>+;
#endif
