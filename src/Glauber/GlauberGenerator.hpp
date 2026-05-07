#ifndef CAP__GlauberGenerator
#define CAP__GlauberGenerator
#include "GlauberDb.hpp"
#include "GlauberEvent.hpp"
#include "GlauberEventFilter.hpp"
#include "ManagedObjects.hpp"
#include "Task.hpp"

namespace CAP
{
using namespace std;

class GlauberGenerator
  :
  public Task
  {
   public:

  GlauberGenerator();
  GlauberGenerator(const GlauberGenerator & src);
  GlauberGenerator & operator=(const GlauberGenerator & rhs);
  virtual ~GlauberGenerator() {}
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
  unsigned int nDbs() const  { return _managedDb.size();  }
  const GlauberDb & db() const;
  const std::vector<GlauberDb*>   & dbs() const;
  const GlauberEvent & event() const;
  const std::vector<GlauberEvent*> & events() const;
  const std::vector<GlauberEventFilter*> & eventFilters() const;
  GlauberDb & db();
  std::vector<GlauberDb*> & dbs();
  GlauberEvent & event();
  std::vector<GlauberEvent*> & events();

  NNXSectVsSqrtS  * nnXSectVsSqrt()   { return _nnXSectVsSqrtS;  }
  NNProfile       * nnProfile()       { return _nnXSectProfile;  }
  //SmearingProfile * smearingProfile() { return _smearingProfile; }
  PTotProfile     * ptotProfile()     { return _ptotProfile;     }
  //NucleonProfile  * nucleonProfile()  { return _nucleonProfile;  }

  protected:

  ManagedObjects<GlauberDb>          _managedDb;
  ManagedObjects<GlauberEvent>       _managedEvents;
  ManagedObjects<GlauberEventFilter> _managedEventFilters;

  int    _nnCollisionOption;
  bool   _randomizeEP;
  int    _xSectOption;
  int    _xSectVsSOption;
  double _sqrtS;
  double _xSection;
  double _xSectionOmega;
  double _xSectionG;
  double _xSectionSigma;
  double _minImpact;
  double _maxImpact;
  double _intDistanceMax;
  double _hardIntDistMax;

  double _minImpactSq;
  double _maxImpactSq;
  double _intDistanceMaxSq;
  double _hardIntDistMaxSq;

  NNXSectVsSqrtS  *_nnXSectVsSqrtS;
  NNProfile       *_nnXSectProfile;
  PTotProfile     *_ptotProfile;

  ClassDef(GlauberGenerator,0)
};

//  _smearingProfile = new SmearingProfile();
//  _smearingProfile->setNucleonSigma(nucleonSigma);
//  NucleonProfile  *_nucleonProfile;

  //  _nucleonProfile = new NucleonProfile();
//  _nucleonProfile->setNucleonRadius(nucleonRadius);




} // namespace CAP

#endif


