#ifndef CAP__GlauberAnalyzer
#define CAP__GlauberAnalyzer
#include "Accountant.hpp"
#include "Task.hpp"
#include "GlauberDb.hpp"
#include "GlauberEvent.hpp"
#include "GlauberEventFilter.hpp"
#include "GlauberHistos.hpp"

namespace CAP
{

  class GlauberAnalyzer
  :
  public Task
  {
  protected:

  ManagedObjects<GlauberEvent>         _managedEvents;
  ManagedObjects<GlauberEventFilter>   _managedEventFilters;
  ManagedObjects<GlauberHistos>        _managedHistograms;
  std::vector<bool>                    _acceptedEventFilters;
  Accountant _eventAccepted;
  bool _useRadialProfile;
  //bool _fillEvents;
  //radialProfile;

  public:

  GlauberAnalyzer();
  GlauberAnalyzer(const GlauberAnalyzer & analyzer);
  GlauberAnalyzer & operator=(const GlauberAnalyzer & analyzer);
  virtual ~GlauberAnalyzer() {}
  virtual void setDefaultConfiguration();
  virtual void configure(const Configuration & configuration);
  virtual void initialize();
  virtual void execute();
  virtual void finalize();
  virtual void reset();
  virtual void clear();
  virtual void print() const;
  unsigned int nEvents() const  { return _managedEvents.size();  }
  unsigned int nEventFilters() const  { return _managedEventFilters.size();  }
  unsigned int nHistograms() const  { return _managedHistograms.size();  }
  const GlauberEvent & event() const                                  { return *_managedEvents.objectAt(0); }
  const std::vector<GlauberEvent*> & events() const                   { return _managedEvents.getObjects(); }
  const std::vector<GlauberEventFilter*> & eventFilters() const       { return _managedEventFilters.getObjects(); }
  const std::vector<GlauberHistos*> & histograms() const              { return _managedHistograms.getObjects(); }
  GlauberEvent & event()                                              { return *_managedEvents.objectAt(0); }
  std::vector<GlauberEvent*> & events()                               { return _managedEvents.getObjects(); }
  std::vector<GlauberEventFilter*> & eventFilters()                   { return _managedEventFilters.getObjects(); }
  std::vector<GlauberHistos*> & histograms()                          { return _managedHistograms.getObjects(); }
  const std::vector<bool> & acceptedEventFilters() const { return _acceptedEventFilters; }
  Accountant & eventAccepted() { return _eventAccepted; }

  virtual void scaleHistograms();
  virtual void scaleHistograms(const std::vector<double> & factors);

//  double calculateDensity(NuclearDistribution & dist, double x, double y);
//  void   calculateArea(double xSectionEvent,
//                       const GlauberEventMoments & momentsSystem,
//                       double & areaOverlap,
//                       double & areaAll);
//  double calculateLength();

  ClassDef(GlauberAnalyzer,0)
  };

} // namespace CAP

#endif // !CAP__GlauberAnalyzer
