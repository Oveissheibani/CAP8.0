#include "GlauberAnalyzer.hpp"
#include "GlauberHistos.hpp"
#include "MathBasicFunctions.hpp"
#include "NuclearDistribution.hpp"
#include "TRandom.h"

ClassImp(CAP::GlauberAnalyzer);

namespace CAP
{

  using CAP::MATH::pi;
  using CAP::MATH::twoPi;


  GlauberAnalyzer::GlauberAnalyzer()
  :
  Task(),
  _managedEvents(),
  _managedEventFilters(),
  _managedHistograms(),
  _acceptedEventFilters(),
  _eventAccepted(),
  _useRadialProfile(false)
  {
  appendClassName("GlauberAnalyzer");
  setName("GlauberAnalyzer");
  setTitle("GlauberAnalyzer");
  }

  GlauberAnalyzer::GlauberAnalyzer(const GlauberAnalyzer & src)
  :
  Task(src),
  _managedEvents(src._managedEvents),
  _managedEventFilters(src._managedEventFilters),
  _managedHistograms(src._managedHistograms),
  _acceptedEventFilters(src._acceptedEventFilters),
  _eventAccepted(src._eventAccepted),
  _useRadialProfile(src._useRadialProfile)
  {   }

  GlauberAnalyzer & GlauberAnalyzer::operator=(const GlauberAnalyzer & rhs)
  {
  if (this!=&rhs)
    {
    Task::operator=(rhs);
    _managedEvents = rhs._managedEvents;
    _managedEventFilters = rhs._managedEventFilters;
    _managedHistograms = rhs._managedHistograms;
    _acceptedEventFilters = rhs._acceptedEventFilters;
    _eventAccepted = rhs._eventAccepted;
    _useRadialProfile = rhs._useRadialProfile;
    }
  return *this;
  }

  void GlauberAnalyzer::setDefaultConfiguration()
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  Task::setDefaultConfiguration();
  const String & taskName = Task::name();

  _configuration.addProperty(createKey(taskName,"SEVERITY"),         "TRACE");
  _configuration.addProperty(createKey(taskName,"SUBTASK:N"),        0);
  _configuration.addProperty(createKey(taskName,"EVENT:USE"),        1);
  _configuration.addProperty(createKey(taskName,"EVENT:REQUESTED:N"),1000);
  _configuration.addProperty(createKey(taskName,"EVENT:REPORT:N"),   100);
  _configuration.addProperty(createKey(taskName,"EVENT:N"),          1);
  _configuration.addProperty(createKey(taskName,"EVENT_0:NAME"),     "Glauber");
  _configuration.addProperty(createKey(taskName,"EVENT_0:OWNER"),    false);

  String objectType = "HISTOGRAM";
  _configuration.addProperty(createKey(taskName,objectType,"fillBasics"),true);
  _configuration.addProperty(createKey(taskName,objectType,"fillMoments"),false);
  _configuration.addProperty(createKey(taskName,objectType,"fillSmeared"),false);
  _configuration.addProperty(createKey(taskName,objectType,"fillArea"),false);
  _configuration.addProperty(createKey(taskName,objectType,"fillEvents"),false);
  _configuration.addProperty(createKey(taskName,objectType,"nOrders"),4);

  _configuration.addProperty(createKey(taskName,objectType,"nParts_nbins"),100);
  _configuration.addProperty(createKey(taskName,objectType,"nBinaries_nbins"),100);
  _configuration.addProperty(createKey(taskName,objectType,"nSpect_nbins"),100);
  _configuration.addProperty(createKey(taskName,objectType,"impact_nbins"),50);
  _configuration.addProperty(createKey(taskName,objectType,"area_nBins"),50);

  _configuration.addProperty(createKey(taskName,objectType,"nParts_min"),0.0);
  _configuration.addProperty(createKey(taskName,objectType,"nBinaries_min"),0.0);
  _configuration.addProperty(createKey(taskName,objectType,"nSpect_min"),0.0);
  _configuration.addProperty(createKey(taskName,objectType,"impact_min"),0.0);
  _configuration.addProperty(createKey(taskName,objectType,"area_min"),0.0);

  _configuration.addProperty(createKey(taskName,objectType,"nParts_max"),400.0);
  _configuration.addProperty(createKey(taskName,objectType,"nBinaries_maxn"),1000.0);
  _configuration.addProperty(createKey(taskName,objectType,"nSpect_max"),400.0);
  _configuration.addProperty(createKey(taskName,objectType,"impact_max"),14.0);
  _configuration.addProperty(createKey(taskName,objectType,"area_max"),14.0);

  _managedEvents.setDefaultObjectConfiguration(taskName,"EVENT", _configuration);
  _managedEventFilters.setDefaultObjectConfiguration(taskName,"EVENT_FILTER", _configuration);
  _managedHistograms.setDefaultObjectConfiguration(taskName,objectType, _configuration);


  //  TF1 *radialProfile = new TF1("radialProfile","x*TMath::Exp(-x*x/(2.*[0]*[0]))",0.0,3.0*sigma);
  //  radialProfile->SetParameter(0,sigma);
  }

  void GlauberAnalyzer::configure(const Configuration & configuration)
  {
  if (reportDebug(__FUNCTION__)) { printCR();  }
  Task::configure(configuration);
  _managedEvents.configureObjects(configuration);
  _managedEventFilters.configureObjects(configuration);
  _managedHistograms.configureObjects(configuration);
  }

  void GlauberAnalyzer::initialize()
  {
  if (reportDebug(__FUNCTION__)) { printCR(); }
  _timer.start();
  _taskExecuted.initialize("taskExecuted",1);
  _managedEvents.initialize();
  _managedEventFilters.initialize();
  _managedHistograms.initialize();
  GlauberEvent & ev = event();
  GlauberEventMoments & momentsA      = ev.momentsNucleusA();
  GlauberEventMoments & momentsB      = ev.momentsNucleusB();
  GlauberEventMoments & momentsSystem = ev.momentsSystem();
  momentsA.initialize();
  momentsB.initialize();
  momentsSystem.initialize();

  int nef = nEventFilters();
  if (nef<1) nef = 1;
  _eventAccepted.initialize("EventAccepted",nef);
  _acceptedEventFilters.assign(nef,false);
  }

  void GlauberAnalyzer::finalize()
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  scaleHistograms();
  _managedEvents.finalize();
  _managedEventFilters.finalize();
  _managedHistograms.finalize();
  _taskExecuted.clear();
  _eventAccepted.clear();
  }

  void GlauberAnalyzer::reset()
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  _managedEvents.reset();
  _managedEventFilters.reset();
  _managedHistograms.reset();
  _taskExecuted.reset();
  _eventAccepted.reset();
  }

  void GlauberAnalyzer::clear()
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  _managedEvents.clear();
  _managedEventFilters.clear();
  _managedHistograms.clear();
  _taskExecuted.clear();
  _eventAccepted.clear();
  }

  void GlauberAnalyzer::print() const
  {
  if (reportDebug(__FUNCTION__)) { printCR();}
  _managedEvents.print();
  _managedEventFilters.print();
  _managedHistograms.print();
  _taskExecuted.print();
  _eventAccepted.print();
  }

  void GlauberAnalyzer::execute()
  {
  //if (reportInfo(__FUNCTION__)) { printCR();}
  GlauberEvent & ev = event();
  GlauberNucleus & nucleusA = ev.nucleusA();
  GlauberNucleus & nucleusB = ev.nucleusB();
  std::vector<GlauberNucleon> & nucleonsA = nucleusA.allNucleons();
  std::vector<GlauberNucleon> & nucleonsB = nucleusB.allNucleons();

  GlauberEventMoments & momentsA      = ev.momentsNucleusA();
  GlauberEventMoments & momentsB      = ev.momentsNucleusB();
  GlauberEventMoments & momentsSystem = ev.momentsSystem();
  momentsA.reset();
  momentsB.reset();
  momentsSystem.reset();
  double smearRadius, smearPhi, x, y;

  for (auto & nucleonA : nucleonsA)
    {
    if (!nucleonA.isWounded()) continue;
    const VectorLorentz & position = nucleonA.position();
    x = position.x();
    y = position.y();
    if (_useRadialProfile)
      {
      smearRadius = 0.5;// fix me radialProfile->GetRandom();
      smearPhi    = twoPi() * gRandom->Rndm();
      x += smearRadius*cos(smearPhi);
      y += smearRadius*sin(smearPhi);
      }
    momentsA.fill(x,y);
    momentsSystem.fill(x,y);
    }
  for (auto & nucleonB : nucleonsB)
    {
    if (!nucleonB.isWounded()) continue;
    const VectorLorentz & position = nucleonB.position();
    x = position.x();
    y = position.y();
    if (_useRadialProfile)
      {
      smearRadius = 0.5;//radialProfile->GetRandom();
      smearPhi    = twoPi() * gRandom->Rndm();
      x += smearRadius*cos(smearPhi);
      y += smearRadius*sin(smearPhi);
      }
    momentsB.fill(x,y);
    momentsSystem.fill(x,y);
    }
  momentsA.calculateAverages();
  momentsB.calculateAverages();
  momentsSystem.calculateAverages();



  std::vector<GlauberEventFilter*> & filters = eventFilters();
  std::vector<GlauberHistos*> & glauberHistos = histograms();
  if (momentsSystem._sum>0) // fill histos only for events having wounded nucleons.
    {
    if (filters.size()==0)
      {
      //printValue("glauberHistos.size()",glauberHistos.size());

      glauberHistos[0]->fill(ev,1.0);
      _eventAccepted.increment(0);
      }
    else
      {
      unsigned int iEventFilter = 0;
      for (auto & filter : filters)
        {
        if (filter->accept(ev))
          {
          glauberHistos[iEventFilter]->fill(ev,1.0);
          _eventAccepted.increment(iEventFilter);
          }
        iEventFilter++;
        }
      }
    }
  incrementTaskExecuted();
  }

  //!
  //! Scale (all) the histograms held in all the histogram groups owned by this Task instance. This operation is executed automatically within the finalize() method call if
  //! and only if the HistogramsScale parameter of the Configuration instance controlling this Task is set to "true".
  //!
  void GlauberAnalyzer::scaleHistograms()
  {
  if (reportInfo(__FUNCTION__)) { /* no ops */ };

  std::vector<long> & counts = _eventAccepted.values();
  std::vector<double> factors;
  for (auto count : counts)
    {
    if (count>0)
      {
      double f = 1.0/double(count);
      printValue("Count",count);
      printValue("Scaling factor",f);
      factors.push_back(f);
      }
    else
      factors.push_back(1.0);
    }

  scaleHistograms(factors);
  if (reportEnd(__FUNCTION__)) { /* no ops */ };
  }

  void GlauberAnalyzer::scaleHistograms(const std::vector<double> & factors)
  {
  if (reportStart(__FUNCTION__)) { /* no ops */ };
  std::vector<GlauberHistos*> & histos = histograms();
  for (unsigned int k=0; k< histos.size(); k++)
    {
    histos[k]->scaleHistograms(factors[k]);
    }
  if (reportEnd(__FUNCTION__)) { /* no ops */ };
  }
  


} // namespace CAP
