#include "Glauber.hpp"

namespace CAP
{
  
    //!
    //!Class implements MC Glauber generator
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
    double      _nnXSectEvent;     //eventStat value of Nucleon-nucleon cross section
    vector<GlauberNucleon> _nucleonsA;
    vector<GlauberNucleon> _nucleonsB;
    vector<GlauberNucleon> _nucleonsAB;
      // TNtuple*      fNt;             //Ntuple for results (created, but not deleted)
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
    eventStat         eventStat;  //Glauber event (results of calculation stored in tree)
    bool        fBC[999][999];   //Array to record binary collision
    bool        CalcResults(double impactPar);
    bool        calculateEvent(double impactPar);
    
  public:
    GlauberMC(const char* NA = "Pb", const char* NB = "Pb", double xsect = 42, double xsectsigma=0);
      //  virtual              ~GlauberMC() {delete fNt; fNt=0;}
    
    double            CalcDens(TF1 & prof, double xval, double yval) const;
    void                Draw(Option_t* option="");
    double            getB()                 const {return eventStat.B;}
    double            getBNN()               const {return eventStat.BNN;}
    double            getBmax()              const {return _impactParMax;}
    double            getBmin()              const {return _impactParMin;}
    double            getEcc(Int_t i=2)      const {return fEccN[i];}
    double            getHardFrac()          const {return _hardFracXSect;}
    double            getmeanX()             const {return eventStat.meanX;}
    double            getmeanXParts()        const {return eventStat.meanX;}
    double            getmeanXSystem()       const {return eventStat.meanXSystem;}
    double            getmeanY()             const {return eventStat.meanY;}
    double            getmeanYParts()        const {return eventStat.meanY;}
    double            getmeanYSystem()       const {return eventStat.meanYSystem;}
    double            psi(Int_t i=2)      const {return fPsiN[i];}
    double            getSx2()               const {return eventStat.VarX;}
    double            getSxy()               const {return eventStat.VarXY;}
    double            getSy2()               const {return eventStat.VarY;}
    double            totXSect()          const;
    double            totXSectErr()       const;
    double            getXSectEvent()        const {return _nnXSectEvent;}
    Int_t               nCollisions()             const {return eventStat.nCollisions;}
    Int_t               nCollisionsnn()           const {return eventStat.nCollisions_nn;}
    Int_t               nCollisionspn()           const {return eventStat.nCollisions_pn;}
    Int_t               nCollisionspp()           const {return eventStat.nCollisions_pp;}
    Int_t               nParticipants()             const {return eventStat.nParticipants;}
    Int_t               nParticipants0()            const {return eventStat.nParticipants0;}
    Int_t               nParticipantsA()            const {return eventStat.nParticipantsA;}
    Int_t               nParticipantsB()            const {return eventStat.nParticipantsB;}
    Int_t               nParticipantsFound()        const {return _nPartMax;}
    TF1*                getXSectDist()         const {return fPTot;}
    GlauberNucleus*       _nucleusA()                {return & _nucleusA;}
    GlauberNucleus*       _nucleusB()                {return & _nucleusB;}
      //  TNtuple*            ntuple()            const {return fNt;}
    vector<GlauberNucleons*> & throwNucleons();
    vector<GlauberNucleons*> & allNucleons();
    const eventStat       & event()             const {return eventStat;}
    const eventStat        *event()                   {return & eventStat;}
    const GlauberNucleus* _nucleusA()          const {return & _nucleusA;}
    const GlauberNucleus* _nucleusB()          const {return & _nucleusB;}
    bool              IsBC(Int_t i, Int_t j) const {return fBC[i][j];}
      //bool              NextEvent(double impactPar=-1);
      //    bool              readNextEvent(bool calc=1, const char *fname=0);
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
  
}

ClassImp(GlauberMC)

GlauberM::GlauberMC()
{

}

GlauberMC::GlauberMC(const char* NA, const char* NB, double xsect, double xsectsigma)
:
_nucleusA(NA),
_nucleusB(NB),
_nnXSect(xsect),
_nnXSectOmega(0),
_nnXSectLambda(0),
_nnXSectEvent(0),
_nucleonsA(0),
_nucleonsB(0),
_nucleonsAB(0),
fNt(0),
eventStatents(0),
_nTotalEvents(0),
_impactParMin(0),
_impactParMax(20),
_hardFracXSect(0.65),
fDetail(99),
_calcArea(0),
_calcength(0),
_coreOnly(0),
_gGribovAA(1),
_nPartMax(0),
f2Cx(0),
fPTot(0),
fNNProf(0),
eventStat()
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
}

GlauberMC::GlauberMC(const GlauberMC & glauberMC )
{

}

GlauberMC GlauberMC::operator=(const GlauberMC & glauberMC)
{
  if (this!=&glauberMC)
  {

  }
  return *this;
}


bool GlauberMC::calculateEvent(double impactPar)
{
  if (fNNProf) throw NNProfGlauberException("No NNProf defined",__FUNCTION__);

  for (auto & nucleonA : _nucleonsA) nucleonA->setInNucleusA();
  for (auto & nucleonB : _nucleonsB) nucleonB->setInNucleusB();
  vector<double> xsecA; xsecA.assigned(900,0.0);
  vector<double> xsecB; xsecB.assigned(900,0.0);
  double xsecB[999] = {0};
  if (fPTot)
  {
    _nnXSectEvent = fPTot->getRandom();
    if (_gGribovAA)
    {
      for (auto vA : xsecA) vA = fPTot->getRandom();
      for (auto vB : xsecB) vB = fPTot->getRandom();
    }
    else
      _nnXSectEvent = _nnXSect;
  }
  // "ball" diameter = distance at which two balls interact
  double d2 = (double)_nnXSectEvent/(CAP::MATH::pi()*10); // in fm^2
  double bh = TMath::Sqrt(d2*_hardFracXSect);

    double xmin=0,xmax=0;
    fNNProf->getRange(xmin,xmax);
    d2 = xmax*xmax;


  eventStat eventStat;
  memset(fBC,0,sizeof(bool)*999*999);
  Int_t nc=0,nh=0;
  for (auto & nucleonB : _nucleonsB)
  {
    bool tB=nucleonB->nucleonType();
    for (auto & nucleonA : _nucleonsA)
    {

      double dx = nucleonB->getX()-nucleonA->getX();
      double dy = nucleonB->getY()-nucleonA->getY();
      double dij = dx*dx+dy*dy;
      if (_gGribovAA  &&  fPTot)	d2 = 0.5*(xsecA[j]+xsecB[i])/(CAP::MATH::pi()*10);
      if (dij>d2) continue;
      double bij = TMath::Sqrt(dij);
      double val = fNNProf->Eval(bij);
      double ran = gRandom->Uniform();
      if (ran>val) continue;
      nucleonB->collide();
      nucleonA->collide();
      fBC[i][j] = 1;
      eventStat.BNN  += bij;
      ++nc;
      if (bij<bh) ++nh;
      bool tA=nucleonA->nucleonType();
      if (tA!=tB)
        ++eventStat.nCollisions_pn;
      else if (tA==1)
        ++eventStat.nCollisions_pp;
      else
        ++eventStat.nCollisions_nn;
      if (nc==1)
      {
        eventStat.X0 = (nucleonA->getX()+nucleonB->getX())/2;
        eventStat.Y0 = (nucleonA->getY()+nucleonB->getY())/2;
      }
    }
  }
  eventStat.B = impactPar;
  ++_nTotalEvents;
  if (nc>0)
  {
    ++eventStatents;
    eventStat.nCollisions     = nc;
    eventStat.nHardCollisionsCollisions     = nh;
    eventStat.BNN      /= nc;
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

  for (auto & nucleonA : _nucleonsA)
  {
    double xA=nucleonA->_position.x();
    double yA=nucleonA->_position.y();
    eventStat.meanXSystem  += xA;
    eventStat.meanYSystem  += yA;
    eventStat.meanXA  += xA;
    eventStat.meanYA  += yA;
    if (nucleonA->isWounded())
    {
      double w = nucleonA->get2CWeight(f2Cx);
      ++eventStat.nParticipants;
      if (nucleonA->nCollisions()==1)
        ++eventStat.nParticipants0;
      ++eventStat.nParticipantsA;
      sumW   += w;
      sumWA  += w;
      eventStat.meanX  += xA * w;
      eventStat.meanY  += yA * w;
      eventStat.meanX2 += xA * xA * w;
      eventStat.meanY2 += yA * yA * w;
      eventStat.meanXY += xA * yA * w;
    }
  }

  for (auto & nucleonB : _nucleonsB)
  {
    double xB=nucleonB->_position.x();
    double yB=nucleonB->_position.y();
    double xB=nucleonB->getX();
    double yB=nucleonB->getY();
    eventStat.meanXSystem  += xB;
    eventStat.meanYSystem  += yB;
    eventStat.meanXB  += xB;
    eventStat.meanYB  += yB;
    if (nucleonB->isWounded()) {
      double w = nucleonB->get2CWeight(f2Cx);
      ++eventStat.nParticipants;
      if (nucleonB->nCollisions()==1) ++eventStat.nParticipants0;
      ++eventStat.nParticipantsB;
      sumW   += w;
      sumWB  += w;
      eventStat.meanX  += xB * w;
      eventStat.meanY  += yB * w;
      eventStat.meanX2 += xB * xB * w;
      eventStat.meanY2 += yB * yB * w;
      eventStat.meanXY += xB * yB * w;
    }
  }
  if (eventStat.nParticipants>0)
  {
    eventStat.meanX  /= sumW;
    eventStat.meanY  /= sumW;
    eventStat.meanX2 /= sumW;
    eventStat.meanY2 /= sumW;
    eventStat.meanXY /= sumW;
  } else {
    eventStat.meanX = 0;
    eventStat.meanY  = 0;
    eventStat.meanX2 = 0;
    eventStat.meanY2 = 0;
    eventStat.meanXY = 0;
  }

  if (_nucleonsA.size()+_nucleonsB.size()>0) {
    eventStat.meanXSystem /= (_nucleonsA.size() + _nucleonsB.size());
    eventStat.meanYSystem /= (_nucleonsA.size() + _nucleonsB.size());
  } else {
    eventStat.meanXSystem = 0;
    eventStat.meanYSystem = 0;
  }
  if (_nucleonsA.size()>0) {
    eventStat.meanXA /= _nucleonsA.size();
    eventStat.meanYA /= _nucleonsA.size();
  } else {
    eventStat.meanXA = 0;
    eventStat.meanYA = 0;
  }
  if (_nucleonsB.size()>0) {
    eventStat.meanXB /= _nucleonsB.size();
    eventStat.meanYB /= _nucleonsB.size();
  } else {
    eventStat.meanXB = 0;
    eventStat.meanYB = 0;
  }

  eventStat.VarX  = eventStat.meanX2-(eventStat.meanX*eventStat.meanX);
  eventStat.VarY  = eventStat.meanY2-(eventStat.meanY*eventStat.meanY);
  eventStat.VarXY = eventStat.meanXY-eventStat.meanX*eventStat.meanY;
  double tmpa = eventStat.VarX*eventStat.VarY-eventStat.VarXY*eventStat.VarXY;
  if (tmpa<0) 
    eventStat.areaW = -1;
  else 
    eventStat.areaW = TMath::Sqrt(tmpa);

  if (eventStat.nParticipants>0)
  {
    // do full moments relative to meanX and meanY
    for (Int_t n = 1; n<10; ++n)
    {
      for (Int_t ia = 0; ia<_nucleonsA.size(); ++ia) {
        GlauberNucleon *nucleonA=(GlauberNucleon*)(_nucleonsA->At(ia));
	if (nucleonA->nCollisions()<=kNc) 
	  continue;
	double xA=nucleonA->getX() - eventStat.meanX;
	double yA=nucleonA->getY() - eventStat.meanY;
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
	double xB=nucleonB->getX() - eventStat.meanX;
	double yB=nucleonB->getY() - eventStat.meanY;
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
      cosPhi[n] /= eventStat.nParticipants;
      sinPhi[n] /= eventStat.nParticipants;
      rn[n] /= eventStat.nParticipants;
      if (rn[n]>0) {
	fPsiN[n] = (TMath::ATan2(sinPhi[n],cosPhi[n]) + CAP::MATH::pi())/n;
        fEccN[n] = TMath::Sqrt(sinPhi[n]*sinPhi[n]+cosPhi[n]*cosPhi[n])/rn[n];
      } else {
	fPsiN[n] = -1;
	fEccN[n] = -1;
      }
    }
    if (!kNc) { //silly test but useful to catch errors 
      double t=TMath::Sqrt(TMath::Power(eventStat.VarY-eventStat.VarX,2)+4.*eventStat.VarXY*eventStat.VarXY)/(eventStat.VarY+eventStat.VarX)/fEccN[2];
      if (t<0.99||t>1.01)
        cout << "Error: Expected t=1 but found t=" << t << endl;
    }
  }

  eventStat.B      = impactPar;
  eventStat.PhiA   = _nucleusA.phiRot();
  eventStat.ThetaA = _nucleusA.thetaRot();
  eventStat.PhiB   = _nucleusB.phiRot();
  eventStat.ThetaB = _nucleusB.thetaRot();
  eventStat.Psi1   = fPsiN[1];
  eventStat.Ecc1   = fEccN[1];
  eventStat.Psi2   = fPsiN[2];
  eventStat.Ecc2   = fEccN[2];
  eventStat.Psi3   = fPsiN[3];
  eventStat.Ecc3   = fEccN[3];
  eventStat.Psi4   = fPsiN[4];
  eventStat.Ecc4   = fEccN[4];
  eventStat.Psi5   = fPsiN[5];
  eventStat.Ecc5   = fEccN[5];

  if (_calcArea) {
    const Int_t nbins=200;
    const double ell=10;
    const double da=2*ell*2*ell/nbins/nbins;
    const double d2 = (double)_nnXSectEvent/(CAP::MATH::pi()*10); // in fm^2
    const double r2 = d2/4.;
    const double mx = eventStat.meanX;
    const double my = eventStat.meanY;
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
    eventStat.AreaO = overlap1*da;
    eventStat.AreaA = overlap2*da;
  }

  if (_calcength) {
    const double krhs = TMath::Sqrt(_nnXSectEvent/40./CAP::MATH::pi());
    const double ksg  = krhs/TMath::Sqrt(5);
    const double kDL  = 0.1;
    TF1 rad("rad","2*pi/[0]/[0]*TMath::Exp(-x*x/(2.*[0]*[0]))",0.0,5*ksg); 
    rad.setParameter(0,ksg);
    const double minval = rad.Eval(5*ksg);
    eventStat.Phi0         = gRandom->Uniform(0,TMath::TwoPi());
    double kcphi0  = TMath::Cos(eventStat.Phi0);
    double ksphi0  = TMath::Sin(eventStat.Phi0);
    double x       = eventStat.X0;
    double y       = eventStat.Y0;
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
    eventStat.Length = 2*i1a/i0a;
  }

  if (eventStat.nParticipants > _nPartMax)
    _nPartMax = eventStat.nParticipants;

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
  return (1.*eventStatents/_nTotalEvents)*CAP::MATH::pi()*_impactParMax*_impactParMax/100;
}

double GlauberMC::totXSectErr() const
{
  return totXSect()/TMath::Sqrt((double)eventStatents) * TMath::Sqrt(double(1.-eventStatents/_nTotalEvents));
}

TObjArray *GlauberMC::allNucleons()
{
  if (!_nucleonsA || !_nucleonsB) return 0;
  if (_nucleonsAB) return _nucleonsAB;

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
  if (impactPar<0) impactPar = TMath::Sqrt((_impactParMax*_impactParMax-_impactParMin*_impactParMin)*gRandom->Rndm()+_impactParMin*_impactParMin);
  _nucleusA.generateNucleons(-impactPar/2.);
  _nucleusB.generateNucleons(impactPar/2.);
  return calculateEvent(impactPar);
}

bool GlauberMC::readNextEvent(bool calc, const char *fname)
{
  static TFile *inf = 0;
  static Int_t iev  = 0;
  if (fname) {
    cout << "readNextEvent: setting up file " << fname << endl;
    delete inf;
    inf = TFile::Open(fname);
    if (!inf) 
      return 0;
    if (!_nucleonsA) {
      _nucleusA.generateNucleons();
      _nucleonsA = _nucleusA.allNucleons();
      _nucleonsA.size() = _nucleusA.nNucleons();
      for (Int_t i = 0; i<_nucleonsA.size(); ++i) {
	GlauberNucleon *nucleonA=(GlauberNucleon*)(_nucleonsA->At(i));
	nucleonA->setInNucleusA();
      }
    }
    if (!_nucleonsB) {
      _nucleusB.generateNucleons();
      _nucleonsB = _nucleusB.allNucleons();
      _nucleonsB.size() = _nucleusB.nNucleons();
      for (Int_t i = 0; i<_nucleonsB.size(); ++i) {
	GlauberNucleon *nucleonB=(GlauberNucleon*)(_nucleonsB->At(i));
	nucleonB->setInNucleusB();
      }
    }
    if (calc)
      return 1;
    fNt = dynamic_cast<TNtuple*>(inf->get(Form("nt_%s_%s",_nucleusA.name(),_nucleusB.name())));
    if (!fNt) {
      cerr << "readNextEvent: Could not find ntuple!" << endl;
      inf->ls();
      return 0;
    }
    fNt->setBranchAddress("nParticipants",&eventStat.nParticipants);
    fNt->setBranchAddress("nCollisions",&eventStat.nCollisions);
    fNt->setBranchAddress("B",&eventStat.B);
    fNt->setBranchAddress("BNN",&eventStat.BNN);
    fNt->setBranchAddress("VarX",&eventStat.VarX);
    fNt->setBranchAddress("VarY",&eventStat.VarY);
    fNt->setBranchAddress("VarXY",&eventStat.VarXY);
    fNt->setBranchAddress("nParticipantsA",&eventStat.nParticipantsA);
    fNt->setBranchAddress("nParticipantsB",&eventStat.nParticipantsB);
    fNt->setBranchAddress("nParticipants0",&eventStat.nParticipants0);
    fNt->setBranchAddress("Psi1",&eventStat.Psi1);
    fNt->setBranchAddress("Ecc1",&eventStat.Ecc1);
    fNt->setBranchAddress("Psi2",&eventStat.Psi2);
    fNt->setBranchAddress("Ecc2",&eventStat.Ecc2);
    fNt->setBranchAddress("Psi3",&eventStat.Psi3);
    fNt->setBranchAddress("Ecc3",&eventStat.Ecc3);
    fNt->setBranchAddress("Psi4",&eventStat.Psi4);
    fNt->setBranchAddress("Ecc4",&eventStat.Ecc4);
    fNt->setBranchAddress("Psi5",&eventStat.Psi5);
    fNt->setBranchAddress("Ecc5",&eventStat.Ecc5);
    return 1;
  }
  if ((!inf)||(!fNt&&!calc)) {
    cerr << "readNextEvent was not initialized" <<endl;
    return 0;
  }
  TObjArray *arr = dynamic_cast<TObjArray*>(inf->get(Form("nucleonarray%d",iev)));
  if (!arr) {
    if (iev==0) {
      cerr << "readNextEvent could not read nucleon array for event " << iev << endl;
      return 0;
    }
    iev = 0;
    cerr << "readNextEvent GlauberNucleonting to first event" << endl;
    arr = dynamic_cast<TObjArray*>(inf->get(Form("nucleonarray%d",iev)));
  }

  double impactParA=0, impactParB=0;
  Int_t inA=0, inB=0;
  const Int_t nNucls = arr->getEntries();
  for (Int_t iNucl=0; iNucl<nNucls; ++iNucl) {
    GlauberNucleon *nuclinp = static_cast<GlauberNucleon*>(arr->At(iNucl));
    GlauberNucleon *nuclout = 0;
    if (nuclinp->isInNucleusB()) { 
      nuclout = static_cast<GlauberNucleon*>(_nucleonsB->At(inB));
      impactParB += nuclinp->getX();
      ++inB;
    } else {
      nuclout = static_cast<GlauberNucleon*>(_nucleonsA->At(inA));
      impactParA += nuclinp->getX();
      ++inA;
    }
    nuclout->GlauberNucleon();
    nuclout->setXYZ(nuclinp->getX(),nuclinp->getY(),nuclinp->getZ());
    nuclout->setNucleonType(nuclinp->nucleonType());
    nuclout->setEnergy(nuclinp->energy());
    if (!calc)
      nuclout->setnCollisions(nuclinp->nCollisions());
  }
  delete arr;
  double impactPar = impactParB/inB-impactParA/inA;
  if (calc) {
    bool ret = calculateEvent(impactPar);
    if (0) 
      cout << iev << ": " << eventStat.B << " " << eventStat.nParticipants << " " << eventStat.nCollisions << " " << eventStat.nParticipants0 << endl;
    ++iev;
    return ret;
  }
  Int_t ret = fNt->getEntry(iev);
  if (ret<=0) 
    return 0;
  fEccN[1]=eventStat.Ecc1;
  fEccN[2]=eventStat.Ecc2;
  fEccN[3]=eventStat.Ecc3;
  fEccN[4]=eventStat.Ecc4;
  fEccN[5]=eventStat.Ecc5;
  if (0) 
    cout << iev << ": " << eventStat.B << " " << eventStat.nParticipants << " " << eventStat.nCollisions << " " << eventStat.nParticipants0 << endl;
  if (0) { // test ntuple values vs re-calculated values
    double nParticipants = eventStat.nParticipants;
    double nCollisions = eventStat.nCollisions;
    double ecc2  = eventStat.Ecc2;
    calculateEvent(impactPar);
    if (nParticipants!=eventStat.nParticipants)
      cout << iev << " differ in nParticipants " << nParticipants << " " << eventStat.nParticipants << endl;
    if (nCollisions!=eventStat.nCollisions)
      cout << iev << " differ in nCollisions " << nCollisions << " " << eventStat.nCollisions << endl;
    if (absolute(ecc2-eventStat.Ecc2)>0.001)
      cout << iev << " differ in ecc2 " << ecc2 << " " << eventStat.Ecc2 << endl;
  }
  ++iev;
  return 1;
}

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
      if (br) br->setCompressionLevel(9);
    }
  }

  for (Int_t i = 0; i<nevents; ++i) {
    while (!NextEvent(b)) {}
    fNt->Fill((Float_t*)(&eventStat.nParticipants));
    if ((i>0)&&(i%100)==0) 
      cout << "eventStat # " << i << " x-sect = " << totXSect() << " +- " << totXSectErr() << " b        \r" << flush;
  }
  if (nevents>99)
    cout << endl << "Done!" << endl;
}





