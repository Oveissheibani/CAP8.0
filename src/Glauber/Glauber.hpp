#ifndef CAP__GlauberTask
#define CAP__GlauberTask
#include "Task.hpp"
#include "HistogramGroup.hpp"
#include "Histogramset.hpp"

namespace CAP
{

#define HAVE_MATHMORE

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include <TBits.h>
#include <TEllipse.h>
#include <TF1.h>
#include <TF2.h>
#include <TFile.h>
#include <TH2.h>
#include <TLine.h>
#include <TMath.h>
#include <TNamed.h>
#include <TNtuple.h>
#include <TObjArray.h>
#include <TRandom.h>
#include <TRotation.h>
#include <String.h>
#include <TSystem.h>
#include <TVectorLorentz.h>
#ifdef HAVE_MATHMORE
 #include <Math/SpecFuncMathMore.h>
#endif
using namespace std;
#endif

#ifndef _runglauber_
#if !defined(__CINT__) || defined(__MAKECINT__)
#define _runglauber_ 3
#endif
#include "HistogramTask.hpp"

//!
//!Class implements Glauber MC generator
//!
class GlauberMC
{
  public:

  protected:
    GlauberNucleus  _nucleusA;       //Nucleus A
    GlauberNucleus  _nucleusB;       //Nucleus B
    double      _nnXSect;          //Nucleon-nucleon cross section
    double      _nnXSectOmega;     //StdDev of Nucleon-nucleon cross section
    double      _nnXSectLambda;    //Jacobian from tot to inelastic (Strikman)
    double      _nnXSectEvent;     //GlauberEventStat value of Nucleon-nucleon cross section
    TObjArray*    _nucleonsA;      //Array of nucleons in nucleus A
    TObjArray*    _nucleonsB;      //Array of nucleons in nucleus B
    TObjArray*    _nucleonsAB;       //Array which joins Nucleus A & B
    Int_t         _nucleonsA.size();             //Number of nucleons in nucleus A
    Int_t         _nucleonsB.size();             //Number of nucleons in nucleus B
    TNtuple*      fNt;             //Ntuple for results (created, but not deleted)
    Int_t         eventStatsents;         //Number of events with at least one collision
    Int_t         _nTotalEvents;    //All events within selected impact parameter range
    double      _impactParMin;           //Minimum impact parameter to be generated
    double      _impactParMax;           //Maximum impact parameter to be generated
    double      _hardFracXSect;       //Fraction of cross section used for nHardCollisionsCollisions (def=0.65)
    Int_t         fDetail;         //Detail to store (99=all by default)
    bool        _calcArea;       //If true calculate overlap area via grid (slow, off by default)
    bool        _calcength;     //If true calculate path length (slow, off by default)
    bool        _coreOnly;         //If true calculate area and eccentricy only for core participants (off by default)
    bool        _gGribovAA;         //If true do Glauber Gribov also for AA
    Int_t         _nPartMax;  //Largest value of nParticipants obtained
    double      fPsiN[10];       //Psi N
    double      fEccN[10];       //Ecc N
    double      f2Cx;            //Two-component x
    TF1          *fPTot;           //Cross section distribution
    TF1          *fNNProf;         //NN profile (hard-sphere == 0 by default)
    GlauberEventStat         eventStats;             //Glauber event (results of calculation stored in tree)
    bool        fBC[999][999];   //Array to record binary collision
    bool        CalcResults(double impactPar);
    bool        calculateEvent(double impactPar);

  public:
  GlauberMC();
  GlauberMC(const char* NA = "Pb", const char* NB = "Pb", double xsect = 42, double xsectsigma=0);
  GlauberMC(const GlauberMC & glauberMC );
  GlauberMC operator=(const GlauberMC & glauberMC);


  virtual              ~GlauberMC() {delete fNt; fNt=0;}

    double            CalcDens(TF1 & prof, double xval, double yval) const;
    void                Draw(Option_t* option="");
    double            getB()                 const {return eventStats.B;}
    double            getBNN()               const {return eventStats.BNN;}
    double            getBmax()              const {return _impactParMax;}
    double            getBmin()              const {return _impactParMin;}
    double            getEcc(Int_t i=2)      const {return fEccN[i];}
    double            getHardFrac()          const {return _hardFracXSect;}
    double            getmeanX()             const {return eventStats.meanX;}
    double            getmeanXParts()        const {return eventStats.meanX;}
    double            getmeanXSystem()       const {return eventStats.meanXSystem;}
    double            getmeanY()             const {return eventStats.meanY;}
    double            getmeanYParts()        const {return eventStats.meanY;}
    double            getmeanYSystem()       const {return eventStats.meanYSystem;}
    double            psi(Int_t i=2)      const {return fPsiN[i];}
    double            getSx2()               const {return eventStats.VarX;}    
    double            getSxy()               const {return eventStats.VarXY;}    
    double            getSy2()               const {return eventStats.VarY;}    
    double            totXSect()          const;
    double            totXSectErr()       const;
    double            getXSectEvent()        const {return _nnXSectEvent;}
    Int_t               nCollisions()             const {return eventStats.nCollisions;}
    Int_t               nCollisionsnn()           const {return eventStats.nCollisions_nn;}
    Int_t               nCollisionspn()           const {return eventStats.nCollisions_pn;}
    Int_t               nCollisionspp()           const {return eventStats.nCollisions_pp;}
    Int_t               nParticipants()             const {return eventStats.nParticipants;}
    Int_t               nParticipants0()            const {return eventStats.nParticipants0;}
    Int_t               nParticipantsA()            const {return eventStats.nParticipantsA;}
    Int_t               nParticipantsB()            const {return eventStats.nParticipantsB;}
    Int_t               nParticipantsFound()        const {return _nPartMax;}
    TF1*                getXSectDist()         const {return fPTot;}
    GlauberNucleus*       _nucleusA()                {return & _nucleusA;}
    GlauberNucleus*       _nucleusB()                {return & _nucleusB;}
    TNtuple*            ntuple()            const {return fNt;}
    TObjArray          *allNucleons();
    const GlauberEventStat       & event()             const {return eventStats;}
    const GlauberEventStat        *event()                   {return & eventStats;}
    const GlauberNucleus* _nucleusA()          const {return & _nucleusA;}
    const GlauberNucleus* _nucleusB()          const {return & _nucleusB;}
    bool              IsBC(Int_t i, Int_t j) const {return fBC[i][j];}
    bool              NextEvent(double impactPar=-1);
    void                GlauberNucleon()                      {delete fNt; fNt=0; }
    bool              readNextEvent(bool calc=1, const char *fname=0);       
    void                run(Int_t nevents,double b=-1);
    void                set2Cx(double x)           {f2Cx = x;}
    void                setBmax(double bmax)       {_impactParMax = bmax;}
    void                setBmin(double bmin)       {_impactParMin = bmin;}
    void                setCalcAAGG(bool b)        {_gGribovAA = b;}
    void                setCalcArea(bool b)        {_calcArea = b;}
    void                setCalcCore(bool b)        {_coreOnly = b;}
    void                setCalcLength(bool b)      {_calcength = b;}
    void                setDetail(Int_t d)           {fDetail = d;}
    void                setHardFrac(double f)      {_hardFracXSect=f;}
    void                setLattice(Int_t i)          {_nucleusA.setLattice(i); _nucleusB.setLattice(i);}
    void                setMinDistance(double d)   {_nucleusA.setMinDist(d); _nucleusB.setMinDist(d);}
    void                setNNProf(TF1 *f1)           {fNNProf = f1;}
    void                setNodeDistance(double d)  {_nucleusA.setNodeDist(d); _nucleusB.setNodeDist(d);}
    void                setRecenter(Int_t b)         {_nucleusA.setRecenter(b); _nucleusB.setRecenter(b);}
    void                setShiftMax(double s)      {_nucleusA.setShiftMax(s); _nucleusB.setShiftMax(s);}
    void                setSmearing(double s)      {_nucleusA.setSmearing(s); _nucleusB.setSmearing(s);}
    const char         *Str()                  const {return Form("gmc-%s%s-_xSection%.1f-md%.1f-nd%.1f-rc%d-smax%.1f",_nucleusA.name(),_nucleusB.name(),_nnXSect,_nucleusB.getMinDist(),_nucleusB.nodeDist(),_nucleusB.getRecenter(),_nucleusB.getShiftMax());}
    static void         PrintVersion()               {cout << "GlauberMC " << Version() << endl;}
    static const char  *Version()                    {return "v3.2";}

    ClassDef(GlauberMC,6) // GlauberMC class
};

//---------------------------------------------------------------------------------
TF1 *nNProfile(double _xSection, double omega, double G) 
{ // NN collisoin profile from https://arxiv.org/abs/1307.0636
  if ((omega<0) || (omega>1))
    return 0;
  double R2 = _xSection/10./CAP::MATH::pi();
  TF1 *nnprof = new TF1("nnprofgamma","[2]*(1-TMath::Gamma([0],[1]*x^2))",0,3);
  nnprof->setParameters(1./omega,G/omega/R2,G);
  return nnprof;
}



ClassImp(GlauberMC)
  ClassImp(GlauberMC::GlauberEventStat)
//---------------------------------------------------------------------------------

GlauberMC::GlauberMC(const char* NA, const char* NB, double xsect, double xsectsigma) :
  _nucleusA(NA),_nucleusB(NB),
  _nnXSect(xsect),_nnXSectOmega(0),_nnXSectLambda(0),_nnXSectEvent(0),
  _nucleonsA(0),_nucleonsB(0),_nucleonsAB(0),
  _nucleonsA.size()(0),_nucleonsB.size()(0),fNt(0),
  eventStatsents(0),_nTotalEvents(0),_impactParMin(0),_impactParMax(20),_hardFracXSect(0.65),
  fDetail(99),_calcArea(0),_calcength(0), _coreOnly(0), _gGribovAA(1),
  _nPartMax(0),f2Cx(0),fPTot(0),fNNProf(0),
  eventStats()
{
  if (xsectsigma>0) {
    _nnXSectOmega = xsectsigma;
    _nnXSectLambda = 1;
    fPTot = new TF1("fPTot","((x/[2])/(x/[2]+[0]))*exp(-(((x/[2])/[0]-1 )**2)/([1]*[1]))/[2]",0,300);
    fPTot->setParameters(_nnXSect,_nnXSectOmega,_nnXSectLambda);
    fPTot->setNpx(1000);
    _nnXSectLambda = _nnXSect/fPTot->getHistogram()->getmean();
    cout << "final lambda=" << _nnXSectLambda << endl;
    fPTot->setParameters(_nnXSect,_nnXSectOmega,_nnXSectLambda);
    cout << "final <sigma>=" << fPTot->getHistogram()->getmean() << endl;
  }

  String name(Form("Glauber_%s_%s",_nucleusA.name(),_nucleusB.name()));
  String title(Form("Glauber %s+%s Version",_nucleusA.name(),_nucleusB.name()));
  setName(name);
  setTitle(title);
}

bool GlauberMC::calculateEvent(double impactPar)
{
  // calc next event
  if (!_nucleonsA)
    {
    _nucleonsA = _nucleusA.allNucleons();
    _nucleonsA.size() = _nucleusA.nNucleons();
    for (Int_t i = 0; i<_nucleonsA.size(); ++i) {
      GlauberNucleon *nucleonA=(GlauberNucleon*)(_nucleonsA->At(i));
      nucleonA->setInNucleusA();
    }
  }

  if (!_nucleonsB)
    {
    _nucleonsB = _nucleusB.allNucleons();
    _nucleonsB.size() = _nucleusB.nNucleons();
    for (Int_t i = 0; i<_nucleonsB.size(); ++i) {
      GlauberNucleon *nucleonB=(GlauberNucleon*)(_nucleonsB->At(i));
      nucleonB->setInNucleusB();
    }
  }

  double xsecA[999] = {0};
  double xsecB[999] = {0};
  if (fPTot) {
    _nnXSectEvent = fPTot->getRandom();
    if (_gGribovAA) {
      for (Int_t i = 0; i<_nucleonsA.size(); ++i)
	xsecA[i] = fPTot->getRandom();
      for (Int_t i = 0; i<_nucleonsB.size(); ++i)
	xsecB[i] = fPTot->getRandom();
    }
  }
  else 
    _nnXSectEvent = _nnXSect;

  // "ball" diameter = distance at which two balls interact
  double d2 = (double)_nnXSectEvent/(CAP::MATH::pi()*10); // in fm^2
  double bh = TMath::Sqrt(d2*_hardFracXSect);
  if (fNNProf) {
    double xmin=0,xmax=0;
    fNNProf->getRange(xmin,xmax);
    d2 = xmax*xmax;
  }

  eventStats.GlauberNucleon();
  memset(fBC,0,sizeof(bool)*999*999);
  Int_t nc=0,nh=0;
  for (Int_t i = 0; i<_nucleonsB.size(); ++i) {
    GlauberNucleon *nucleonB=(GlauberNucleon*)(_nucleonsB->At(i));
    bool tB=nucleonB->nucleonType();
    for (Int_t j = 0; j<_nucleonsA.size(); ++j) {
      GlauberNucleon *nucleonA=(GlauberNucleon*)(_nucleonsA->At(j));
      double dx = nucleonB->getX()-nucleonA->getX();
      double dy = nucleonB->getY()-nucleonA->getY();
      double dij = dx*dx+dy*dy;
      if (_gGribovAA  &&  fPTot)
	d2 = 0.5*(xsecA[j]+xsecB[i])/(CAP::MATH::pi()*10);
      if (dij>d2) 
        continue;
      double bij = TMath::Sqrt(dij);
      if (fNNProf) {
        double val = fNNProf->Eval(bij);
        double ran = gRandom->Uniform();
        if (ran>val)
          continue;
      }
      nucleonB->collide();
      nucleonA->collide();
      fBC[i][j] = 1;
      eventStats.BNN  += bij;
      ++nc;
      if (bij<bh)
        ++nh;
      bool tA=nucleonA->nucleonType();
      if (tA!=tB)
        ++eventStats.nCollisions_pn;
      else if (tA==1)
        ++eventStats.nCollisions_pp;
      else
        ++eventStats.nCollisions_nn;
      if (nc==1) {
        eventStats.X0 = (nucleonA->getX()+nucleonB->getX())/2;
        eventStats.Y0 = (nucleonA->getY()+nucleonB->getY())/2;
      }
    }
  }
  eventStats.B = impactPar;
  ++_nTotalEvents;
  if (nc>0) {
    ++eventStatsents;
    eventStats.nCollisions     = nc;
    eventStats.nHardCollisionsCollisions     = nh;
    eventStats.BNN      /= nc;
    return CalcResults(impactPar);
  }
  return kFALSE;
}

bool GlauberMC::CalcResults(double impactPar)
{
  // calc results for the given event
  double sumW=0;
  double sumWA=0;
  double sumWB=0;

  double sinPhi[10] = {0};
  double cosPhi[10] = {0};
  double rn[10]     = {0};

  const Int_t kNc = _coreOnly; // used later for core/corona

  for (Int_t i = 0; i<_nucleonsA.size(); ++i) {
    GlauberNucleon *nucleonA=(GlauberNucleon*)(_nucleonsA->At(i));
    double xA=nucleonA->getX();
    double yA=nucleonA->getY();
    eventStats.meanXSystem  += xA;
    eventStats.meanYSystem  += yA;
    eventStats.meanXA  += xA;
    eventStats.meanYA  += yA;
    if (nucleonA->isWounded()) {
      double w = nucleonA->get2CWeight(f2Cx);
      ++eventStats.nParticipants;
      if (nucleonA->nCollisions()==1)
	++eventStats.nParticipants0;
      ++eventStats.nParticipantsA;
      sumW   += w;
      sumWA  += w;
      eventStats.meanX  += xA * w;
      eventStats.meanY  += yA * w;
      eventStats.meanX2 += xA * xA * w;
      eventStats.meanY2 += yA * yA * w;
      eventStats.meanXY += xA * yA * w;
    }
  }

  for (Int_t i = 0; i<_nucleonsB.size(); ++i) {
    GlauberNucleon *nucleonB=(GlauberNucleon*)(_nucleonsB->At(i));
    double xB=nucleonB->getX();
    double yB=nucleonB->getY();
    eventStats.meanXSystem  += xB;
    eventStats.meanYSystem  += yB;
    eventStats.meanXB  += xB;
    eventStats.meanYB  += yB;
    if (nucleonB->isWounded()) {
      double w = nucleonB->get2CWeight(f2Cx);
      ++eventStats.nParticipants;
      if (nucleonB->nCollisions()==1)
	++eventStats.nParticipants0;
      ++eventStats.nParticipantsB;
      sumW   += w;
      sumWB  += w;
      eventStats.meanX  += xB * w;
      eventStats.meanY  += yB * w;
      eventStats.meanX2 += xB * xB * w;
      eventStats.meanY2 += yB * yB * w;
      eventStats.meanXY += xB * yB * w;
    }
  }
  if (eventStats.nParticipants>0) {
    eventStats.meanX  /= sumW;
    eventStats.meanY  /= sumW;
    eventStats.meanX2 /= sumW;
    eventStats.meanY2 /= sumW;
    eventStats.meanXY /= sumW;
  } else {
    eventStats.meanX = 0;
    eventStats.meanY  = 0;
    eventStats.meanX2 = 0;
    eventStats.meanY2 = 0;
    eventStats.meanXY = 0;
  }

  if (_nucleonsA.size()+_nucleonsB.size()>0) {
    eventStats.meanXSystem /= (_nucleonsA.size() + _nucleonsB.size());
    eventStats.meanYSystem /= (_nucleonsA.size() + _nucleonsB.size());
  } else {
    eventStats.meanXSystem = 0;
    eventStats.meanYSystem = 0;
  }
  if (_nucleonsA.size()>0) {
    eventStats.meanXA /= _nucleonsA.size();
    eventStats.meanYA /= _nucleonsA.size();
  } else {
    eventStats.meanXA = 0;
    eventStats.meanYA = 0;
  }
  if (_nucleonsB.size()>0) {
    eventStats.meanXB /= _nucleonsB.size();
    eventStats.meanYB /= _nucleonsB.size();
  } else {
    eventStats.meanXB = 0;
    eventStats.meanYB = 0;
  }

  eventStats.VarX  = eventStats.meanX2-(eventStats.meanX*eventStats.meanX);
  eventStats.VarY  = eventStats.meanY2-(eventStats.meanY*eventStats.meanY);
  eventStats.VarXY = eventStats.meanXY-eventStats.meanX*eventStats.meanY;
  double tmpa = eventStats.VarX*eventStats.VarY-eventStats.VarXY*eventStats.VarXY;
  if (tmpa<0) 
    eventStats.areaW = -1;
  else 
    eventStats.areaW = TMath::Sqrt(tmpa);

  if (eventStats.nParticipants>0) {
    // do full moments relative to meanX and meanY
    for (Int_t n = 1; n<10; ++n) {
      for (Int_t ia = 0; ia<_nucleonsA.size(); ++ia) {
        GlauberNucleon *nucleonA=(GlauberNucleon*)(_nucleonsA->At(ia));
	if (nucleonA->nCollisions()<=kNc) 
	  continue;
	double xA=nucleonA->getX() - eventStats.meanX;
	double yA=nucleonA->getY() - eventStats.meanY;
	double r = TMath::Sqrt(xA*xA+yA*yA);
	double phi = TMath::ATan2(yA,xA);
	double w = n;
	if (n==1) 
	  w = 3; // use r^3 weighting for Ecc1/Psi1
	double rw = TMath::Power(r,w);
	cosPhi[n] += rw*TMath::Cos(n*phi);
	sinPhi[n] += rw*TMath::Sin(n*phi);
	rn[n] += rw;
      }
      for (Int_t ib = 0; ib<_nucleonsB.size(); ++ib) {
        GlauberNucleon *nucleonB=(GlauberNucleon*)(_nucleonsB->At(ib));
	if (nucleonB->nCollisions()<=kNc) 
	  continue;
	double xB=nucleonB->getX() - eventStats.meanX;
	double yB=nucleonB->getY() - eventStats.meanY;
	double r = TMath::Sqrt(xB*xB+yB*yB);
	double phi = TMath::ATan2(yB,xB);
	double w = n;
	if (n==1)
	  w = 3; // use r^3 weighting for Ecc1/Psi1
	double rw = TMath::Power(r,w);
	cosPhi[n] += rw*TMath::Cos(n*phi);
	sinPhi[n] += rw*TMath::Sin(n*phi);
	rn[n] += rw;
      }
      cosPhi[n] /= eventStats.nParticipants;
      sinPhi[n] /= eventStats.nParticipants;
      rn[n] /= eventStats.nParticipants;
      if (rn[n]>0) {
	fPsiN[n] = (TMath::ATan2(sinPhi[n],cosPhi[n]) + CAP::MATH::pi())/n;
	fEccN[n] = TMath::Sqrt(sinPhi[n]*sinPhi[n]+cosPhi[n]*cosPhi[n])/rn[n];
      } else {
	fPsiN[n] = -1;
	fEccN[n] = -1;
      }
    }
    if (!kNc) { //silly test but useful to catch errors 
      double t=TMath::Sqrt(TMath::Power(eventStats.VarY-eventStats.VarX,2)+4.*eventStats.VarXY*eventStats.VarXY)/(eventStats.VarY+eventStats.VarX)/fEccN[2];
      if (t<0.99||t>1.01)
        cout << "Error: Expected t=1 but found t=" << t << endl;
    }
  }

  eventStats.B      = impactPar;
  eventStats.PhiA   = _nucleusA.phiRot();
  eventStats.ThetaA = _nucleusA.thetaRot();
  eventStats.PhiB   = _nucleusB.phiRot();
  eventStats.ThetaB = _nucleusB.thetaRot();
  eventStats.Psi1   = fPsiN[1];
  eventStats.Ecc1   = fEccN[1];
  eventStats.Psi2   = fPsiN[2];
  eventStats.Ecc2   = fEccN[2];
  eventStats.Psi3   = fPsiN[3];
  eventStats.Ecc3   = fEccN[3];
  eventStats.Psi4   = fPsiN[4];
  eventStats.Ecc4   = fEccN[4];
  eventStats.Psi5   = fPsiN[5];
  eventStats.Ecc5   = fEccN[5];

  if (_calcArea) {
    const Int_t nbins=200;
    const double ell=10;
    const double da=2*ell*2*ell/nbins/nbins;
    const double d2 = (double)_nnXSectEvent/(CAP::MATH::pi()*10); // in fm^2
    const double r2 = d2/4.;
    const double mx = eventStats.meanX;
    const double my = eventStats.meanY;
    TH2D areaA("hAreaA",";x (fm);y (fm)",nbins,-ell,ell,nbins,-ell,ell);
    TH2D areaB("hAreaB",";x (fm);y (fm)",nbins,-ell,ell,nbins,-ell,ell);
    for (Int_t i = 0; i<_nucleonsA.size(); ++i) {
      GlauberNucleon *nucleonA=(GlauberNucleon*)(_nucleonsA->At(i));
      if (!nucleonA->isWounded())
        continue;
      if (nucleonA->nCollisions()==kNc)
        continue;
      double x = nucleonA->getX()-mx;
      double y = nucleonA->getY()-my;
      for (Int_t xi=1; xi<=nbins; ++xi) {
        for (Int_t yi=1; yi<=nbins; ++yi) {
          Int_t bin = areaA.getBin(xi,yi);
          double val=areaA.getBinContent(bin);
          if (val>0)
            continue;
          double dx=x-areaA.getXaxis()->getBinCenter(xi);
          double dy=y-areaA.getYaxis()->getBinCenter(yi);
          if (dx*dx+dy*dy<r2)
            areaA.setBinContent(bin,1);
        }
      }
    }
    for (Int_t i = 0; i<_nucleonsB.size(); ++i) {
      GlauberNucleon *nucleonB=(GlauberNucleon*)(_nucleonsB->At(i));
      if (!nucleonB->isWounded())
        continue;
      if (nucleonB->nCollisions()==kNc)
        continue;
      double x = nucleonB->getX()-mx;
      double y = nucleonB->getY()-my;
      for (Int_t xi=1; xi<=nbins; ++xi) {
        for (Int_t yi=1; yi<=nbins; ++yi) {
          Int_t bin = areaB.getBin(xi,yi);
          double val=areaB.getBinContent(bin);
          if (val>0)
            continue;
          double dx=x-areaB.getXaxis()->getBinCenter(xi);
          double dy=y-areaB.getYaxis()->getBinCenter(yi);
          if (dx*dx+dy*dy<r2)
            areaB.setBinContent(bin,1);
        }
      }
    }
    double overlap1=0;
    double overlap2=0;
    for (Int_t xi=1; xi<=nbins; ++xi) {
      for (Int_t yi=1; yi<=nbins; ++yi) {
        Int_t bin = areaA.getBin(xi,yi);
        double vA=areaA.getBinContent(bin);
        double vB=areaB.getBinContent(bin);
        if (vA>0&&vB>0)
          ++overlap1;
        if (vA>0||vB>0)
          ++overlap2;
      }
    }
    eventStats.AreaO = overlap1*da;
    eventStats.AreaA = overlap2*da;
  }

  if (_calcength) {
    const double krhs = TMath::Sqrt(_nnXSectEvent/40./CAP::MATH::pi());
    const double ksg  = krhs/TMath::Sqrt(5);
    const double kDL  = 0.1;
    TF1 rad("rad","2*pi/[0]/[0]*TMath::Exp(-x*x/(2.*[0]*[0]))",0.0,5*ksg); 
    rad.setParameter(0,ksg);
    const double minval = rad.Eval(5*ksg);
    eventStats.Phi0         = gRandom->Uniform(0,TMath::TwoPi());
    double kcphi0  = TMath::Cos(eventStats.Phi0);
    double ksphi0  = TMath::Sin(eventStats.Phi0);
    double x       = eventStats.X0;
    double y       = eventStats.Y0;
    double i0a     = 0;
    double i1a     = 0;
    double l       = 0;
    double val     = CalcDens(rad,x,y);
    while (val>minval) {
      x     += kDL * kcphi0;
      y     += kDL * ksphi0;
      i0a   += val;
      i1a   += l*val;
      l+=kDL;
      val    = CalcDens(rad,x,y);
    }
    eventStats.Length = 2*i1a/i0a;
  }

  if (eventStats.nParticipants > _nPartMax) 
    _nPartMax = eventStats.nParticipants;

  return kTRUE;
}

double GlauberMC::CalcDens(TF1 & prof, double xval, double yval) const
{
  double rmin=0,rmax=0;
  prof.getRange(rmin,rmax);
  double r2max = rmax*rmax;
  double ret = 0;
  for (Int_t i = 0; i<_nucleonsA.size(); ++i) {
    GlauberNucleon *nucleonA=(GlauberNucleon*)(_nucleonsA->At(i));
    if (!nucleonA->isWounded())
      continue;
    double x = nucleonA->getX();
    double y = nucleonA->getY();
    double r2=(xval-x)*(xval-x)+(yval-y)*(yval-y);
    if (r2>r2max)
      continue;
    ret += prof.Eval(TMath::Sqrt(r2));
  }
  for (Int_t i = 0; i<_nucleonsB.size(); ++i) {
    GlauberNucleon *nucleonB=(GlauberNucleon*)(_nucleonsB->At(i));
    if (!nucleonB->isWounded())
      continue;
    double x = nucleonB->getX();
    double y = nucleonB->getY();
    double r2=(xval-x)*(xval-x)+(yval-y)*(yval-y);
    if (r2>r2max)
      continue;
    ret += prof.Eval(TMath::Sqrt(r2));
  }
  return ret;
}

void GlauberMC::Draw(Option_t* option)
{
  static TH2F *h2f = new TH2F("hGlauberMC",";x (fm);y(fm)",1,-18,18,1,-12,12);

  h2f->GlauberNucleon();
  h2f->setStats(0);
  h2f->Draw();

  TEllipse e;
  e.setFillColor(0);
  e.setFillStyle(0);
  e.setLineColor(1);
  e.setLineStyle(2);
  e.setLineWidth(1);
  e.DrawEllipse(getB()/2,0,_nucleusB.getR(),_nucleusB.getR(),0,360,0);
  e.DrawEllipse(-getB()/2,0,_nucleusA.getR(),_nucleusA.getR(),0,360,0);
  _nucleusA.Draw(_nnXSect, kMagenta, kYellow);
  _nucleusB.Draw(_nnXSect, kMagenta, kOrange);

  String opt(option);
  if (opt.IsNull())
    return;

  double sy2 = getSy2();
  double sx2 = getSx2();
  double phase = 0;
  if (sy2<sx2) {
    double d = sx2;
    sx2 = sy2;
    sy2 = d;
    phase = CAP::MATH::pi()/2.;
  }
  double x1 = (0.5*(sy2-sx2)+TMath::Sqrt(TMath::Power(sy2-sx2,2.)-4*TMath::Power(getSxy(),2)));
  double ang = TMath::ATan2(-getSxy(),x1)+phase;
  TLine l;
  l.setLineWidth(3);
  l.DrawLine(-10*TMath::Cos(ang),-10*TMath::Sin(ang),10*TMath::Cos(ang),10*TMath::Sin(ang));
}

double GlauberMC::totXSect() const
{
  return (1.*eventStatsents/_nTotalEvents)*CAP::MATH::pi()*_impactParMax*_impactParMax/100;
}

double GlauberMC::totXSectErr() const
{
  return totXSect()/TMath::Sqrt((double)eventStatsents) * TMath::Sqrt(double(1.-eventStatsents/_nTotalEvents));
}

TObjArray *GlauberMC::allNucleons()
{

  _nucleonsA->setOwner(0);
  _nucleonsB->setOwner(0);
  TObjArray *allNucleons=new TObjArray(_nucleonsA.size()+_nucleonsB.size());
  allNucleons->setOwner();
  for (Int_t i = 0; i<_nucleonsA.size(); ++i) {
    allNucleons->Add(_nucleonsA->At(i));
  }
  for (Int_t i = 0; i<_nucleonsB.size(); ++i) {
    allNucleons->Add(_nucleonsB->At(i));
  }
  _nucleonsAB = allNucleons;
  return allNucleons;
}

bool GlauberMC::nextEvent(double impactPar)
{
  if (impactPar<0) 
    impactPar = TMath::Sqrt((_impactParMax*_impactParMax-_impactParMin*_impactParMin)*gRandom->Rndm()+_impactParMin*_impactParMin);

  _nucleusA.generateNucleons(-impactPar/2.);
  _nucleusB.generateNucleons(impactPar/2.);
  return calculateEvent(impactPar);
}

//bool GlauberMC::readNextEvent(bool calc, const char *fname)
//{
//  static TFile *inf = 0;
//  static Int_t iev  = 0;
//  if (fname) {
//    cout << "readNextEvent: setting up file " << fname << endl;
//    delete inf;
//    inf = TFile::Open(fname);
//    if (!inf)
//      return 0;
//    if (!_nucleonsA) {
//      _nucleusA.generateNucleons();
//      _nucleonsA = _nucleusA.allNucleons();
//      _nucleonsA.size() = _nucleusA.nNucleons();
//      for (Int_t i = 0; i<_nucleonsA.size(); ++i) {
//	GlauberNucleon *nucleonA=(GlauberNucleon*)(_nucleonsA->At(i));
//	nucleonA->setInNucleusA();
//      }
//    }
//    if (!_nucleonsB) {
//      _nucleusB.generateNucleons();
//      _nucleonsB = _nucleusB.allNucleons();
//      _nucleonsB.size() = _nucleusB.nNucleons();
//      for (Int_t i = 0; i<_nucleonsB.size(); ++i) {
//	GlauberNucleon *nucleonB=(GlauberNucleon*)(_nucleonsB->At(i));
//	nucleonB->setInNucleusB();
//      }
//    }
//    if (calc)
//      return 1;
//    fNt = dynamic_cast<TNtuple*>(inf->get(Form("nt_%s_%s",_nucleusA.name(),_nucleusB.name())));
//    if (!fNt) {
//      cerr << "readNextEvent: Could not find ntuple!" << endl;
//      inf->ls();
//      return 0;
//    }
//    fNt->setBranchAddress("nParticipants",&eventStats.nParticipants);
//    fNt->setBranchAddress("nCollisions",&eventStats.nCollisions);
//    fNt->setBranchAddress("B",&eventStats.B);
//    fNt->setBranchAddress("BNN",&eventStats.BNN);
//    fNt->setBranchAddress("VarX",&eventStats.VarX);
//    fNt->setBranchAddress("VarY",&eventStats.VarY);
//    fNt->setBranchAddress("VarXY",&eventStats.VarXY);
//    fNt->setBranchAddress("nParticipantsA",&eventStats.nParticipantsA);
//    fNt->setBranchAddress("nParticipantsB",&eventStats.nParticipantsB);
//    fNt->setBranchAddress("nParticipants0",&eventStats.nParticipants0);
//    fNt->setBranchAddress("Psi1",&eventStats.Psi1);
//    fNt->setBranchAddress("Ecc1",&eventStats.Ecc1);
//    fNt->setBranchAddress("Psi2",&eventStats.Psi2);
//    fNt->setBranchAddress("Ecc2",&eventStats.Ecc2);
//    fNt->setBranchAddress("Psi3",&eventStats.Psi3);
//    fNt->setBranchAddress("Ecc3",&eventStats.Ecc3);
//    fNt->setBranchAddress("Psi4",&eventStats.Psi4);
//    fNt->setBranchAddress("Ecc4",&eventStats.Ecc4);
//    fNt->setBranchAddress("Psi5",&eventStats.Psi5);
//    fNt->setBranchAddress("Ecc5",&eventStats.Ecc5);
//    return 1;
//  }
//  if ((!inf)||(!fNt&&!calc)) {
//    cerr << "readNextEvent was not initialized" <<endl;
//    return 0;
//  }
//  TObjArray *arr = dynamic_cast<TObjArray*>(inf->get(Form("nucleonarray%d",iev)));
//  if (!arr) {
//    if (iev==0) {
//      cerr << "readNextEvent could not read nucleon array for event " << iev << endl;
//      return 0;
//    }
//    iev = 0;
//    cerr << "readNextEvent GlauberNucleonting to first event" << endl;
//    arr = dynamic_cast<TObjArray*>(inf->get(Form("nucleonarray%d",iev)));
//  }
//
//  double impactParA=0, impactParB=0;
//  Int_t inA=0, inB=0;
//  const Int_t nNucls = arr->getEntries();
//  for (Int_t iNucl=0; iNucl<nNucls; ++iNucl) {
//    GlauberNucleon *nuclinp = static_cast<GlauberNucleon*>(arr->At(iNucl));
//    GlauberNucleon *nuclout = 0;
//    if (nuclinp->isInNucleusB()) {
//      nuclout = static_cast<GlauberNucleon*>(_nucleonsB->At(inB));
//      impactParB += nuclinp->getX();
//      ++inB;
//    } else {
//      nuclout = static_cast<GlauberNucleon*>(_nucleonsA->At(inA));
//      impactParA += nuclinp->getX();
//      ++inA;
//    }
//    nuclout->GlauberNucleon();
//    nuclout->setXYZ(nuclinp->getX(),nuclinp->getY(),nuclinp->getZ());
//    nuclout->setNucleonType(nuclinp->nucleonType());
//    nuclout->setEnergy(nuclinp->energy());
//    if (!calc)
//      nuclout->setnCollisions(nuclinp->nCollisions());
//  }
//  delete arr;
//  double impactPar = impactParB/inB-impactParA/inA;
//  if (calc) {
//    bool ret = calculateEvent(impactPar);
//    if (0)
//      cout << iev << ": " << eventStats.B << " " << eventStats.nParticipants << " " << eventStats.nCollisions << " " << eventStats.nParticipants0 << endl;
//    ++iev;
//    return ret;
//  }
//  Int_t ret = fNt->getEntry(iev);
//  if (ret<=0)
//    return 0;
//  fEccN[1]=eventStats.Ecc1;
//  fEccN[2]=eventStats.Ecc2;
//  fEccN[3]=eventStats.Ecc3;
//  fEccN[4]=eventStats.Ecc4;
//  fEccN[5]=eventStats.Ecc5;
//  if (0)
//    cout << iev << ": " << eventStats.B << " " << eventStats.nParticipants << " " << eventStats.nCollisions << " " << eventStats.nParticipants0 << endl;
//  if (0) { // test ntuple values vs re-calculated values
//    double nParticipants = eventStats.nParticipants;
//    double nCollisions = eventStats.nCollisions;
//    double ecc2  = eventStats.Ecc2;
//    calculateEvent(impactPar);
//    if (nParticipants!=eventStats.nParticipants)
//      cout << iev << " differ in nParticipants " << nParticipants << " " << eventStats.nParticipants << endl;
//    if (nCollisions!=eventStats.nCollisions)
//      cout << iev << " differ in nCollisions " << nCollisions << " " << eventStats.nCollisions << endl;
//    if (absolute(ecc2-eventStats.Ecc2)>0.001)
//      cout << iev << " differ in ecc2 " << ecc2 << " " << eventStats.Ecc2 << endl;
//  }
//  ++iev;
//  return 1;
//}

void GlauberMC::run(Int_t nevents, double b)
{
  if (fNt == 0) {
    String name(Form("nt_%s_%s",_nucleusA.name(),_nucleusB.name()));
    String title(Form("%s + %s (x-sect = %.1f mb) str %s",_nucleusA.name(),_nucleusB.name(),_nnXSect,Str()));
    String vars("nParticipants:nCollisions:nHardCollisionsCollisions:B:BNN:nCollisions_pp:nCollisions_pn:nCollisions_nn:VarX:VarY:VarXY:nParticipantsA:nParticipantsB:nParticipants0:areaW");
    if (fDetail>1)
      vars+=":Psi1:Ecc1:Psi2:Ecc2:Psi3:Ecc3:Psi4:Ecc4:Psi5:Ecc5";
    if (fDetail>2)
      vars+=":AreaO:AreaA:X0:Y0:Phi0:Length";
    if (fDetail>3)
      vars+=":meanX:meanY:meanX2:meanY2:meanXY:meanXSystem:meanYSystem:meanXA:meanYA:meanXB:meanYB";
    if (fDetail>4)
      vars+=":PhiA:ThetaA:PhiB:ThetaB";
    fNt = new TNtuple(name,title,vars);
    fNt->setDirectory(0);
    TObjArray *l = fNt->listOfBranches();
    for (Int_t i=0; i<l->getEntries(); ++i) {
      TBranch *br = dynamic_cast<TBranch*>(l->At(i));
      if (br)
        br->setCompressionLevel(9);
    }
  }

  for (Int_t i = 0; i<nevents; ++i) {
    while (!NextEvent(b)) {}
    fNt->Fill((Float_t*)(&eventStats.nParticipants));
    if ((i>0)&&(i%100)==0) 
      cout << "GlauberEventStat # " << i << " x-sect = " << totXSect() << " +- " << totXSectErr() << " b        \r" << flush;
  }
  if (nevents>99)
    cout << endl << "Done!" << endl;
}
#endif



