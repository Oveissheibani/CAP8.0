#include "GlauberGenerator.hpp"
#include "GlauberEvent.hpp"
#include "GlauberEventMoments.hpp"
#include "RootHistogramHelpers.hpp"
#include "TCanvas.h"
#include "TH1D.h"
#include "TRandom.h"


ClassImp(CAP::GlauberGenerator)

namespace CAP
{

  using CAP::MATH::pi;
  using CAP::MATH::twoPi;

  //TF1 *getNNProf(Double_t snn=67.6, Double_t omega=0.4, Double_t G=1);
//  Double_t      fXSectOmega;     //StdDev of Nucleon-nucleon cross section
//  Double_t      fXSectLambda;    //Jacobian from tot to inelastic (Strikman)
//  Double_t      fXSectEvent;     //Event value of Nucleon-nucleon cross section
//  Double_t      fHardFrac;       //Fraction of cross section used for Nhard (def=0.65)
//  TF1          *fPTot;           //Cross section distribution
//  TF1          *fNNProf;         //NN profile (hard-sphere == 0 by default)

  GlauberGenerator::GlauberGenerator()
  :
  Task(),
  _managedDb(),
  _managedEvents(),
  _managedEventFilters(),
  _nnCollisionOption(1),
  _randomizeEP(false),
  _xSectOption(1),
  _xSectVsSOption(1),
  _sqrtS(13000.0),
  _xSection(63.0),
  _xSectionOmega(1.0),
  _xSectionG(1.0),
  _xSectionSigma(0.0),
  _minImpact(0.0),
  _maxImpact(20.0),
  _intDistanceMax(4.0),
  _hardIntDistMax(0.4),
  _minImpactSq(0.0),
  _maxImpactSq(400.0),
  _intDistanceMaxSq(16.0),
  _hardIntDistMaxSq(0.4*0.4),
  _nnXSectVsSqrtS(),
  _nnXSectProfile(),
  _ptotProfile()
  {
  // no ops
  }

  GlauberGenerator::GlauberGenerator(const GlauberGenerator & src)
  :
  Task(src),
  _managedDb(src._managedDb),
  _managedEvents(src._managedEvents),
  _managedEventFilters(src._managedEventFilters),
  _nnCollisionOption(src._nnCollisionOption),
  _randomizeEP(src._randomizeEP),
  _xSectOption(src._xSectOption),
  _xSectVsSOption(src._xSectVsSOption),
  _sqrtS(src._sqrtS),
  _xSection(src._xSection),
  _xSectionOmega(src._xSectionOmega),
  _xSectionG(src._xSectionG),
  _xSectionSigma(src._xSectionSigma),
  _minImpact(src._minImpact),
  _maxImpact(src._maxImpact),
  _intDistanceMax(src._intDistanceMax),
  _hardIntDistMax(src._hardIntDistMax),
  _minImpactSq(src._minImpactSq),
  _maxImpactSq(src._maxImpactSq),
  _intDistanceMaxSq(src._intDistanceMaxSq),
  _hardIntDistMaxSq(src._hardIntDistMaxSq),
  _nnXSectVsSqrtS(src._nnXSectVsSqrtS),
  _nnXSectProfile(src._nnXSectProfile),
  _ptotProfile(src._ptotProfile)
  {
  // no ops
  }

  GlauberGenerator & GlauberGenerator::operator=(const GlauberGenerator & rhs)
  {
  if (this!=&rhs)
    {
    Task::operator=(rhs);
    _managedDb              = rhs._managedDb;
    _managedEvents          = rhs._managedEvents;
    _managedEventFilters    = rhs._managedEventFilters;
    _nnCollisionOption      = rhs._nnCollisionOption;
    _randomizeEP            = rhs._randomizeEP;
    _xSectOption            = rhs._xSectOption;
    _xSectVsSOption         = rhs._xSectVsSOption;
    _sqrtS                  = rhs._sqrtS;
    _xSection               = rhs._xSection;
    _xSectionOmega          = rhs._xSectionOmega;
    _xSectionG              = rhs._xSectionG;
    _xSectionSigma          = rhs._xSectionSigma;
    _minImpact              = rhs._minImpact;
    _maxImpact              = rhs._maxImpact;
    _intDistanceMax         = rhs._intDistanceMax;
    _hardIntDistMax         = rhs._hardIntDistMax;
    _minImpactSq            = rhs._minImpactSq;
    _maxImpactSq            = rhs._maxImpactSq;
    _intDistanceMaxSq       = rhs._intDistanceMaxSq;
    _hardIntDistMaxSq       = rhs._hardIntDistMaxSq;
    _nnXSectVsSqrtS         = rhs._nnXSectVsSqrtS;
    _nnXSectProfile         = rhs._nnXSectProfile;
    _ptotProfile            = rhs._ptotProfile;
    }
  return *this;
  }

  void GlauberGenerator::setDefaultConfiguration()
  {
  if (reportDebug(__FUNCTION__)) { printCR();  }
  Task::setDefaultConfiguration();
  String taskName = name();
  String objectType;

  _configuration.addProperty(createKey(taskName,"SEVERITY"),         "TRACE");
  _configuration.addProperty(createKey(taskName,"SUBTASK:N"),        0);
  _configuration.addProperty(createKey(taskName,"EVENT:USE"),        1);
  _configuration.addProperty(createKey(taskName,"EVENT:REQUESTED:N"),1000);
  _configuration.addProperty(createKey(taskName,"EVENT:REPORT:N"),   100);
  _configuration.addProperty(createKey(taskName,"EVENT:N"),          1);
  _configuration.addProperty(createKey(taskName,"EVENT_0:NAME"),     "Glauber");
  _configuration.addProperty(createKey(taskName,"EVENT_0:OWNER"),    1);

  _configuration.addProperty(createKey(taskName,"DB","USE"),1);
  _configuration.addProperty(createKey(taskName,"DB","IMPORT"),0);
  _configuration.addProperty(createKey(taskName,"DB","EXPORT"),0);
  _configuration.addProperty(createKey(taskName,"DB","IMPORT:PATH"),"NONE");
  _configuration.addProperty(createKey(taskName,"DB","IMPORT:FILE_NAME"),"NONE");
  _configuration.addProperty(createKey(taskName,"DB","EXPORT:PATH"),"NONE");
  _configuration.addProperty(createKey(taskName,"DB","EXPORT:FILE_NAME"),"NONE");
  _configuration.addProperty(createKey(taskName,"DB","N"),1);
  _configuration.addProperty(createKey(taskName,"DB_0:NAME"),"DefaultDb");
  _configuration.addProperty(createKey(taskName,"DB_0:TITLE"),"DefaultDb");
  _configuration.addProperty(createKey(taskName,"DB_0:OWNER"),1);

  objectType = "GENERATION";
  _configuration.addProperty(createKey(taskName,objectType,"NUCLEUS_A:NAME"),          "Pb");
  _configuration.addProperty(createKey(taskName,objectType,"NUCLEUS_A:NTRIALS"),       40);
  _configuration.addProperty(createKey(taskName,objectType,"NUCLEUS_A:RECENTERING"),   false);
  _configuration.addProperty(createKey(taskName,objectType,"NUCLEUS_A:ALLOW_OVERLAP"), true);
  _configuration.addProperty(createKey(taskName,objectType,"NUCLEUS_A:MIN_INTER"),     0.2);
  _configuration.addProperty(createKey(taskName,objectType,"NUCLEUS_A:USE_USERSIGMA"), false);


  _configuration.addProperty(createKey(taskName,objectType,"NUCLEUS_B:NAME"),          "Pb");
  _configuration.addProperty(createKey(taskName,objectType,"NUCLEUS_B:NTRIALS"),       40);
  _configuration.addProperty(createKey(taskName,objectType,"NUCLEUS_B:RECENTERING"),   false);
  _configuration.addProperty(createKey(taskName,objectType,"NUCLEUS_B:ALLOW_OVERLAP"), true);
  _configuration.addProperty(createKey(taskName,objectType,"NUCLEUS_B:MIN_INTER"),     0.2);
  _configuration.addProperty(createKey(taskName,objectType,"NUCLEUS_B:USE_USERSIGMA"), false);

  _configuration.addProperty(createKey(taskName,objectType,"INTERACTION:NNCOLLISION_OPTION"),_nnCollisionOption);
  _configuration.addProperty(createKey(taskName,objectType,"INTERACTION:xSECT_OPTION"),   _xSectOption);
  _configuration.addProperty(createKey(taskName,objectType,"INTERACTION:IMPACT_MIN"),    _minImpact);
  _configuration.addProperty(createKey(taskName,objectType,"INTERACTION:IMPACT_MAX"),    _maxImpact);
  _configuration.addProperty(createKey(taskName,objectType,"INTERACTION:RANDOMIZE_EP"),  _randomizeEP);
  _configuration.addProperty(createKey(taskName,objectType,"INTERACTION:SQRT_S"),        _sqrtS);
  _configuration.addProperty(createKey(taskName,objectType,"INTERACTION:SQRT_S_OPTION"), _xSectVsSOption);
  _configuration.addProperty(createKey(taskName,objectType,"INTERACTION:xSECTION"),      _xSection);
  _configuration.addProperty(createKey(taskName,objectType,"INTERACTION:xSECTION_OMEGA"),_xSectionOmega);
  _configuration.addProperty(createKey(taskName,objectType,"INTERACTION:xSECTION_G"),    _xSectionG);
  //_configuration.addProperty(createKey(taskName,objectType,"INTERACTION:xSECTION_SIGMA"),_xSectionSigma);
  _configuration.addProperty(createKey(taskName,objectType,"INTERACTION:DISTANCE_MAX"),  _intDistanceMax);
  _configuration.addProperty(createKey(taskName,objectType,"INTERACTION:HARD_DISTANCE_MAX"),_hardIntDistMax);
  //_configuration.addProperty(createKey(taskName,objectType,"INTERACTION:SMEARINGPROFILE:NUCLEONSIGMA"),1.0);
//  _configuration.addProperty(createKey(taskName,objectType,"PTOTPROFILE:NNXSECT"),         1.0);
//  _configuration.addProperty(createKey(taskName,objectType,"PTOTPROFILE:NNXSECTOMEGA"),    1.0);
//  _configuration.addProperty(createKey(taskName,objectType,"PTOTPROFILE:NNXSECTLAMBDA"),   1.0);

  _managedDb.setDefaultObjectConfiguration(taskName,"DB", _configuration);
  _managedEvents.setDefaultObjectConfiguration(taskName,"EVENT", _configuration);
  _managedEventFilters.setDefaultObjectConfiguration(taskName,"EVENT_FILTER", _configuration);
  }


  void GlauberGenerator::configure(const Configuration & configuration)
  {
  if (reportDebug(__FUNCTION__)) { printCR();  }
  Task::configure(configuration);
  _managedDb.configureObjects(configuration);
  _managedEvents.configureObjects(configuration);
  _managedEventFilters.configureObjects(configuration);
  }

  void GlauberGenerator::initialize()
  {
  if (reportDebug(__FUNCTION__)) { printCR();  }
  GlauberDb & glauberDb = db();
  GlauberEvent & glauberEvent = event();

  String taskName = name();
  String objectType = "GENERATION";
  bool verbose = true;
  if (reportInfo(__FUNCTION__))
    {
    verbose = true;
    printCR();
    printValue("taskName",taskName);
    printValue("objectType",objectType);
    }
  String  nameA     = _configuration.valueString(createKey(taskName,objectType,"NUCLEUS_A:NAME"));
  int     nTrialsA  = _configuration.valueInt(   createKey(taskName,objectType,"NUCLEUS_A:NTRIALS"));
  bool    recenterA = _configuration.valueBool(  createKey(taskName,objectType,"NUCLEUS_A:RECENTERING"));
  bool    allowOverlapA     = _configuration.valueInt(   createKey(taskName,objectType,"NUCLEUS_A:ALLOW_OVERLAP"));
  double  minInterA = _configuration.valueDouble(createKey(taskName,objectType,"NUCLEUS_A:MIN_INTER"));
  bool    useUserSA = _configuration.valueBool(  createKey(taskName,objectType,"NUCLEUS_A:USE_USERSIGMA"));

  if (verbose)
    {
    printValue("nameA",nameA);
    printValue("nTrialsA",nTrialsA);
    printValue("recenterA",recenterA);
    printValue("allowOverlapA",allowOverlapA);
    printValue("minInterA",minInterA);
    printValue("useUserSA",useUserSA);
    }

  GlauberNucleus & nucleusA = glauberEvent.nucleusA();
  glauberDb.findNucleus(nameA,nucleusA.type());
  nucleusA.setNucleus();
  nucleusA.setRecenteringMode(recenterA);
  nucleusA.setAllowOverlap(allowOverlapA);
  nucleusA.setUserSigmaMode(useUserSA);
  nucleusA.setUseLattice(false);
  nucleusA.setMaxNumberTrials(nTrialsA);
  nucleusA.setMinInterNucleonDistance(minInterA);
  nucleusA.generateNucleons();

  String  nameB     = _configuration.valueString(   createKey(taskName,objectType,"NUCLEUS_B:NAME"));
  int     nTrialsB  = _configuration.valueInt(      createKey(taskName,objectType,"NUCLEUS_B:NTRIALS"));
  bool    recenterB = _configuration.valueBool(     createKey(taskName,objectType,"NUCLEUS_B:RECENTERING"));
  bool    allowOverlapB = _configuration.valueBool(     createKey(taskName,objectType,"NUCLEUS_B:ALLOW_OVERLAP"));
  double  minInterB = _configuration.valueDouble(   createKey(taskName,objectType,"NUCLEUS_B:MIN_INTER"));
  bool    useUserSB = _configuration.valueBool(     createKey(taskName,objectType,"NUCLEUS_B:USE_USERSIGMA"));

  if (verbose)
    {
    printValue("nameB",nameB);
    printValue("nTrialsB",nTrialsB);
    printValue("recenterB",recenterB);
    printValue("allowOverlapB",allowOverlapB);
    printValue("minInterB",minInterB);
    printValue("useUserSB",useUserSB);
    }

  GlauberNucleus & nucleusB = glauberEvent.nucleusB();
  glauberDb.findNucleus(nameA, nucleusB.type());
  nucleusB.setNucleus();
  nucleusB.setRecenteringMode(recenterB);
  nucleusB.setAllowOverlap(allowOverlapB);
  nucleusB.setUserSigmaMode(useUserSB);
  nucleusB.setMaxNumberTrials(nTrialsB);
  nucleusB.setMinInterNucleonDistance(minInterB);
  nucleusB.generateNucleons();

  _nnCollisionOption  = _configuration.valueInt(createKey(taskName,objectType,"INTERACTION:NNCOLLISION_OPTION"));
  _minImpact          = _configuration.valueDouble(createKey(taskName,objectType,"INTERACTION:IMPACT_MIN"));
  _maxImpact          = _configuration.valueDouble(createKey(taskName,objectType,"INTERACTION:IMPACT_MAX"));
  _randomizeEP        = _configuration.valueBool(  createKey(taskName,objectType,"INTERACTION:RANDOMIZE_EP"));
  _intDistanceMax     = _configuration.valueDouble(createKey(taskName,objectType,"INTERACTION:DISTANCE_MAX"));
  _hardIntDistMax     = _configuration.valueDouble(createKey(taskName,objectType,"INTERACTION:HARD_DISTANCE_MAX"));
  _minImpactSq        = _minImpact*_minImpact;
  _maxImpactSq        = _maxImpact*_maxImpact;
  _intDistanceMaxSq   = _intDistanceMax*_intDistanceMax;
  _hardIntDistMaxSq   = _hardIntDistMax*_hardIntDistMax;

  if (verbose)
    {
    printValue("_nnCollisionOption",_nnCollisionOption);
    printValue("minImpact",_minImpact);
    printValue("maxImpact",_maxImpact);
    printValue("randomizeEP",_randomizeEP);
    printValue("intDistanceMax",_intDistanceMax);
    printValue("hardIntDistMax",_hardIntDistMax);
    }


  switch (_nnCollisionOption)
    {
      default:
      case 1: // fixed nn cross section using predetermined profile (NNProfile)
      {
      // use fixed cross section with user selected profile
      // cross section to be used determined
      _xSectOption = _configuration.valueInt(createKey(taskName,objectType,"INTERACTION:XSECT_OPTION"));
      switch (_xSectOption)
        {
          case 1:// _xSection = function of sqrt(s)
          {
          _sqrtS           = _configuration.valueDouble(createKey(taskName,objectType,"INTERACTION:SQRT_S"));
          _xSectVsSOption  = _configuration.valueInt(createKey(taskName,objectType,   "INTERACTION:SQRT_S_OPTION"));
          printValue("_xSectVsSOption",_xSectVsSOption);
          printValue("_sqrtS",_sqrtS);
          _nnXSectVsSqrtS = new NNXSectVsSqrtS();
          _nnXSectVsSqrtS->setOption(_xSectVsSOption);
          _xSection = _nnXSectVsSqrtS->evaluate(_sqrtS);
          break;
          }

          case 2: // use the user defined xSection.
          {
          _xSection = _configuration.valueDouble(createKey(taskName,objectType,"INTERACTION:xSECTION"));
          break;
          }
        }
      // nucleon-nucleon cross section profile
      _xSectionOmega   = _configuration.valueDouble(createKey(taskName,objectType,"INTERACTION:xSECTION_OMEGA"));
      _xSectionG       = _configuration.valueDouble(createKey(taskName,objectType,"INTERACTION:xSECTION_G"));
      printValue("_xSection",_xSection);
      printValue("_xSectionOmega",_xSectionOmega);
      printValue("_xSectionG",_xSectionG);
      _nnXSectProfile = new NNProfile();
      _nnXSectProfile->setParameters(_xSection,_xSectionOmega,_xSectionG);

      // the following is used for diagnostic purpose only

      int n = 400;
      double max = 6.0;
      TH1 * h = new TH1D("nnXSectProfile","nnXSectProfile",n, -max, max);
      TH1 * hs = new TH1D("nnXSectProfileS","nnXSectProfileS",n, -max, max);
      double dx = 2.0*max/double(n);
      double x = -max;
      double sum = 0.0;
      for (int k=0; k<n; k++)
        {
        double v = _nnXSectProfile->evaluate(fabs(x));
        x += dx;
        sum += dx*v;
        h->SetBinContent(k,v);
        h->SetBinError(k,0.0);
        hs->SetBinContent(k,sum);
        hs->SetBinError(k,0.0);
        }
      cout << " integral: " << sum << endl;

      TCanvas * canvas = new TCanvas("NNProfile","NNProfile",5,5,800, 800);
      canvas->SetLogy(1);
      h->Draw();
      canvas->Print("NNProfile.pdf");

      TCanvas * canvas2 = new TCanvas("NNProfileS","NNProfileS",25,5,800, 800);
      //canvas2->SetLogy(1);
      hs->Draw();
      canvas2->Print("NNProfileS.pdf");

      break;
      }

      case 2: // Use PTProfile and xSection fluctuations
      {
      // fix me...
      }
    }
  }

  void GlauberGenerator::execute()
  {
  //if (reportInfo(__FUNCTION__)) { printCR();  }
  GlauberEvent & ev = event();
  GlauberNucleus & nucleusA = ev.nucleusA();
  GlauberNucleus & nucleusB = ev.nucleusB();
  ev.reset();
  nucleusA.positionNucleons();
  nucleusB.positionNucleons();
  // printValue("Check",1);
  std::vector<GlauberPoint> & interactionPositions = ev.interactionPositions();
  interactionPositions.clear();
  double impact = sqrt(_minImpactSq + (_maxImpactSq-_minImpactSq)*gRandom->Rndm());
  ev.setImpactParameter(impact);

  // printValue("Check",2);
  VectorLorentz shiftA(0.0,-impact/2.0,0.0,0.0);
  VectorLorentz shiftB(0.0,impact/2.0,0.0,0.0);
  // printValue("Check",3);
  if (_randomizeEP)
    {
    double eventPlaneAngle = twoPi()*gRandom->Rndm();
    shiftA.rotateZ(eventPlaneAngle);
    shiftB.rotateZ(eventPlaneAngle);
    }
  // printValue("Check",4);
  nucleusA.shiftNucleus(shiftA);
  nucleusB.shiftNucleus(shiftB);
  // printValue("Check",5);
  if (_nnCollisionOption==3)
    {
    // In Loizides, arrays are used to compute the x-sect
    // for current event
    // xsecA[i] = fPTot->GetRandom();
    // xsecB[i] = fPTot->GetRandom();
    // here use member of GlauberNucleon
    for (auto & nucleonA : nucleusA.allNucleons()) nucleonA.setXSect(_ptotProfile->random());
    for (auto & nucleonB : nucleusB.allNucleons()) nucleonB.setXSect(_ptotProfile->random());
    }
  // printValue("Check",6);
  double distanceAB, ran;
  for (auto & nucleonA : nucleusA.allNucleons())
    {
    for (auto & nucleonB : nucleusB.allNucleons())
      {
      distanceAB = nucleonA.distanceTo2D(nucleonB);
      if (distanceAB>4.0) continue;

      //printValue("_nnCollisionOption",_nnCollisionOption);
      // few methods:
      // (1) fixed cross section
      // (2) nucleon's size fluctuate (all same) e-by-e
      // (3) all nucleon size fluctuate independently
      switch (_nnCollisionOption)
        {
          case 1: // fix nn cross section using predetermined profile
          {
          // printValue("Check",9);
         double prob = _nnXSectProfile->evaluate(distanceAB);
          ran = gRandom->Rndm();
          ev.setEventXSection(_xSection);
//          printValue("distanceAB",distanceAB);
//          printValue("prob",prob);
//          printValue("ran",ran);
          if (ran > prob) continue;  // no  interaction
          break;
          }

          case 2: // nucleon sizes fluctuate (the same for all nucleons)
          {
          ev.setEventXSection(_xSection);

          // printValue("Check",10);
          //          double xSect = sqrt( 0.1*_ptotProfile->GetRandom()/pi());
          //          double bh = TMath::Sqrt(d2*fHardFrac);
          }

          case 3:
          {
          // printValue("Check",11);
          double xSectA = nucleonA.xSect();
          double xSectB = nucleonB.xSect();
          double d = sqrt(0.05*(xSectA+xSectB)/pi()); // 0.05 = 0.5/10 --> fm
          if (distanceAB>d) continue;
          double prob = _nnXSectProfile->evaluate(distanceAB);
          if (gRandom->Uniform() > prob) continue;
          ev.setEventXSection(_xSection);
          }
        }
      bool isHard = false;
      // printValue("Check",12);

      if (distanceAB < _hardIntDistMax) isHard = true;
      ev.incrementColls(nucleonA,nucleonB,isHard);
      // keep track of the interaction positions
      VectorLorentz & positionA = nucleonA.position();
      double xA = positionA.x();
      double yA = positionA.y();
      VectorLorentz & positionB = nucleonB.position();
      double xB = positionB.x();
      double yB = positionB.y();
      interactionPositions.push_back(GlauberPoint(0.5*(xA+xB),0.5*(yA+yB)));
      } // nucleonB
    } // nucleonA

  }

  void GlauberGenerator::finalize()
  {
  // fix me...
  }

  void GlauberGenerator::reset()
  {
  // fix me...
  }

  void GlauberGenerator::clear()
  {
  // fix me...
  }

  void GlauberGenerator::print() const
  {
  // fix me...
  }

  const GlauberDb & GlauberGenerator::db() const
  {
  //printString("GlauberGenerator::db() const");
  return *_managedDb.objectAt(0);
  }

  const std::vector<GlauberDb*>   & GlauberGenerator::dbs() const
  {
  //printString("GlauberGenerator::dbs() const");
  return _managedDb.getObjects();
  }

  const GlauberEvent & GlauberGenerator::event() const
  {
  //printString("GlauberGenerator::event() const");
  return *_managedEvents.objectAt(0);
  }

  const std::vector<GlauberEvent*> & GlauberGenerator::events() const
  {
  //printString("GlauberGenerator::events() const");
  return _managedEvents.getObjects();
  }

  const std::vector<GlauberEventFilter*> & GlauberGenerator::eventFilters() const
  {
  //printString("GlauberGenerator::eventFilters() const");
  return _managedEventFilters.getObjects();
  }

  GlauberDb & GlauberGenerator::db()
  {
  //printString("GlauberGenerator::db()");
  return *_managedDb.objectAt(0);
  }

  std::vector<GlauberDb*> & GlauberGenerator::dbs()
  {
  //printString("GlauberGenerator::dbs()");
  return _managedDb.getObjects();
  }

  GlauberEvent & GlauberGenerator::event()
  {
  //printString("GlauberGenerator::event()");
  return *_managedEvents.objectAt(0);
  }

  std::vector<GlauberEvent*> & GlauberGenerator::events()
  {
  //printString("GlauberGenerator::events()");
  return _managedEvents.getObjects();
  }


} // namespace CAP
