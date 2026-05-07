#include "GlauberDb.hpp"
#include "MathBasicFunctions.hpp"
#include "NameHelpers.hpp"
#include "PrintHelpers.hpp"
#include "StreamHelpers.hpp"

ClassImp(CAP::Manager<CAP::GlauberDb>);
ClassImp(CAP::GlauberDb)

using CAP::MATH::power;

namespace CAP
{

  GlauberDb::GlauberDb()
  {
  // no ops
  }

  GlauberDb::GlauberDb(const GlauberDb & src __attribute__((unused)))
  {
  // no ops
  }

  GlauberDb GlauberDb::operator=(const GlauberDb & rhs __attribute__((unused)))
  {
  if (this!=&rhs)
    {
    //
    }
  return *this;
  }

  //!
  //! Find the given nucleus and set the GlauberNucleusType pbject accordingly.
  //!
  //!nNucleons: number of nucleons (atomic mass)
  //!nProtons: atomic number
  //!pRadius: radius of the protons
  //!nRadius: radius of the neutrons
  //!pSkin: skin thickness parameter of the protons
  //!nSkin: skin thickness parameter of the neutrons
  //!pWPar: central deformation parameter (protons)
  //!nWPar: central deformation parameter (neutrons)
  //!beta2: deformation parameter - 2nd harmonic
  //!beta4: deformation parameter - 4th harmonic
  //!rType: type of radial function used in the construction of the nuclei
  //!r0, r1, r2: parameters used in special radial profiles.
  //!
  //!Uranium description taken from Heinz & Kuhlman, nucl-th/0411054.  In this code, pRadius is defined as 6.8*0.91, pWPar=6.8*0.26
  //!
  void GlauberDb::findNucleus(const String & name, GlauberNucleusType & nucleusType)
  {
  int nNucleons  = 0;
  int nProtons   = 0;
  double pRadius    = 0;
  double nRadius    = 0;
  double pSkin      = 0;
  double nSkin      = 0;
  double pWPar      = 0;
  double nWPar      = 0;
  int    rType      = 0;
  double aPar       = 0;
  double bPar       = 0;
  double r0         = 0;
  double r1         = 0;
  double r2         = 0;
  double beta2      = 0;
  double beta4      = 0;

  if      (name.EqualTo( "p" ))      {nNucleons = 1;   pRadius = 0.234;  pSkin = 0;      pWPar =  0;       rType = 0;  nProtons=1;}
  else if (name.EqualTo( "pg" ))     {nNucleons = 1;   pRadius = 0.514;  pSkin = 0;      pWPar =  0;       rType = 9;  nProtons=1;}
  else if (name.EqualTo( "pdg" ))    {nNucleons = 1;   pRadius = 1;      pSkin = 0;      pWPar =  0;       rType = 10; nProtons=1;} // from arXiv:1101.5953
  else if (name.EqualTo( "dpf" ))    {nNucleons = 2;   pRadius = 0.01;   pSkin = 0.5882; pWPar =  0;       rType = 1;  nProtons=1;} // deuteron 2pf (tuned to useHulthen)
  else if (name.EqualTo( "dh" ))     {nNucleons = 2;   pRadius = 0.2283; pSkin = 1.1765; pWPar =  0;       rType = 3;  nProtons=1;} // deuteron useHulthen free
  else if (name.EqualTo( "d" ))      {nNucleons = 2;   pRadius = 0.2283; pSkin = 1.1765; pWPar =  0;       rType = 4;  nProtons=1;} // deuteron useHulthen constrained
  else if (name.EqualTo( "He3" ))    {nNucleons = 3;   pRadius = 0.00;   pSkin = 0.0000; pWPar =  0;       rType = 6;  nProtons=1;} // read configurations from file
  else if (name.EqualTo( "H3" ))     {nNucleons = 3;   pRadius = 0.00;   pSkin = 0.0000; pWPar =  0;       rType = 6;  nProtons=2;} // read configurations from file
  else if (name.EqualTo( "He4" ))    {nNucleons = 4;   pRadius = 0.00;   pSkin = 0.0000; pWPar =  0;       rType = 6;  nProtons=2;} // read configurations from file
  else if (name.EqualTo( "C" ))      {nNucleons = 12;  pRadius = 2.608;  pSkin = 0.513;  pWPar = -0.051;   rType = 6;  nProtons=6;} // read configurations from file
  else if (name.EqualTo( "O"))       {nNucleons = 16;  pRadius = 2.608;  pSkin = 0.513;  pWPar = -0.051;   rType = 6;  nProtons=8;} // read configurations from file
  else if (name.EqualTo( "Opar"))    {nNucleons = 16;  pRadius = 2.608;  pSkin = 0.513;  pWPar = -0.051;   rType = 1;  nProtons=8;} // WS parameterization
  else if (name.EqualTo( "Oho" ))    {nNucleons = 16;  pRadius = 1.833;  pSkin = 1.544;  pWPar =  0;       rType = 15; nProtons=8;} // Harmonic oscillator parameterization
  else if (name.EqualTo( "Al" ))     {nNucleons = 27;  pRadius = 3.34;   pSkin = 0.580;  pWPar = 0.0;      rType = 8;  nProtons=13; beta2=-0.448; beta4=0.239;}
  else if (name.EqualTo( "Si" ))     {nNucleons = 28;  pRadius = 3.34;   pSkin = 0.580;  pWPar = -0.233;   rType = 1;  nProtons=14;}
  else if (name.EqualTo( "Si2"))     {nNucleons = 28;  pRadius = 3.34;   pSkin = 0.580;  pWPar =  0;       rType = 8;  nProtons=14; beta2=-0.478; beta4=0.250;}
  else if (name.EqualTo( "S" ))      {nNucleons = 32;  pRadius = 2.54;   pSkin = 2.191;  pWPar =  0.16;    rType = 2;  nProtons=16;}
  else if (name.EqualTo( "Ar"))      {nNucleons = 40;  pRadius = 3.53;   pSkin = 0.542;  pWPar =  0;       rType = 1;  nProtons=18;}
  else if (name.EqualTo( "Ca"))      {nNucleons = 40;  pRadius = 3.766;  pSkin = 0.586;  pWPar = -0.161;   rType = 1;  nProtons=20;}
  else if (name.EqualTo( "Ni"))      {nNucleons = 58;  pRadius = 4.309;  pSkin = 0.517;  pWPar = -0.1308;  rType = 1;  nProtons=28;}
  else if (name.EqualTo( "Cu"))      {nNucleons = 63;  pRadius = 4.20;   pSkin = 0.596;  pWPar =  0;       rType = 1;  nProtons=29;}
  else if (name.EqualTo( "Curw"))    {nNucleons = 63;  pRadius = 4.20;   pSkin = 0.596;  pWPar =  0;       rType = 12; nProtons=29; r0=1.00898; r1=-0.000790403; r2=-0.000389897;}
  else if (name.EqualTo( "Cu2"))     {nNucleons = 63;  pRadius = 4.20;   pSkin = 0.596;  pWPar =  0;       rType = 8;  nProtons=29; beta2=0.162; beta4=-0.006;}
  else if (name.EqualTo( "Cu2rw"))   {nNucleons = 63;  pRadius = 4.20;   pSkin = 0.596;  pWPar =  0;       rType = 14; nProtons=29; beta2=0.162; beta4=-0.006; r0=1.01269; r1=-0.00298083; r2=-9.97222e-05;}
  else if (name.EqualTo( "CuHN"))    {nNucleons = 63;  pRadius = 4.28;   pSkin = 0.5;    pWPar =  0;       rType = 1;  nProtons=29;} // from arXiv:0904.4080v1
  else if (name.EqualTo( "Xe"))      {nNucleons = 129; pRadius = 5.36;   pSkin = 0.59;   pWPar =  0;       rType = 1;  nProtons=54;} // adapted from arXiv:1703.04278
  else if (name.EqualTo( "Xes"))     {nNucleons = 129; pRadius = 5.42;   pSkin = 0.57;   pWPar =  0;       rType = 1;  nProtons=54;} // scale from Sb (Antimony, A=122, r=5.32) by 1.019 = (129/122)**0.333
  else if (name.EqualTo( "Xe2"))     {nNucleons = 129; pRadius = 5.36;   pSkin = 0.59;   pWPar =  0;       rType = 8;  nProtons=54; beta2=0.161; beta4=-0.003;} // adapted from arXiv:1703.04278 and Z. Physik (1974) 270: 113
  else if (name.EqualTo( "Xe2a"))    {nNucleons = 129; pRadius = 5.36;   pSkin = 0.59;   pWPar =  0;       rType = 8;  nProtons=54; beta2=0.18; beta4=0;} // ALICE parameters (see public note from 2018 at https://cds.cern.ch/collection/ALICE%20Public%20Notes?ln=en)
  else if (name.EqualTo( "Xerw"))    {nNucleons = 129; pRadius = 5.36;   pSkin = 0.59;   pWPar =  0;       rType = 12; nProtons=54; r0=1.00911; r1=-0.000722999; r2=-0.0002663;}
  else if (name.EqualTo( "Xesrw"))   {nNucleons = 129; pRadius = 5.42;   pSkin = 0.57;   pWPar =  0;       rType = 12; nProtons=54; r0=1.0096; r1=-0.000874123; r2=-0.000256708;}
  else if (name.EqualTo( "Xe2arw"))  {nNucleons = 129; pRadius = 5.36;   pSkin = 0.59;   pWPar =  0;       rType = 14; nProtons=54; beta2=0.18; beta4=0; r0=1.01246; r1=-0.0024851; r2=-5.72464e-05;}
  else if (name.EqualTo( "W"))       {nNucleons = 186; pRadius = 6.58;   pSkin = 0.480;  pWPar =  0;       rType = 1;  nProtons=74;}
  else if (name.EqualTo( "Au"))      {nNucleons = 197; pRadius = 6.38;   pSkin = 0.535;  pWPar =  0;       rType = 1;  nProtons=79;}
  else if (name.EqualTo( "Aurw"))    {nNucleons = 197; pRadius = 6.38;   pSkin = 0.535;  pWPar =  0;       rType = 12; nProtons=79; r0=1.00899; r1=-0.000590908; r2=-0.000210598;}
  else if (name.EqualTo( "Au2"))     {nNucleons = 197; pRadius = 6.38;   pSkin = 0.535;  pWPar =  0;       rType = 8;  nProtons=79; beta2=-0.131; beta4=-0.031; }
  else if (name.EqualTo( "Au2rw"))   {nNucleons = 197; pRadius = 6.38;   pSkin = 0.535;  pWPar =  0;       rType = 14; nProtons=79; beta2=-0.131; beta4=-0.031; r0=1.01261; r1=-0.00225517; r2=-3.71513e-05;}
  else if (name.EqualTo( "AuHN"))    {nNucleons = 197; pRadius = 6.42;   pSkin = 0.44;   pWPar =  0;       rType = 1;  nProtons=79;} // from arXiv:0904.4080v1
  else if (name.EqualTo( "Pb"))      {nNucleons = 208; pRadius = 6.62;   pSkin = 0.546;  pWPar =  0;       rType = 1;  nProtons=82;}
  else if (name.EqualTo( "Pbrw"))    {nNucleons = 208; pRadius = 6.62;   pSkin = 0.546;  pWPar =  0;       rType = 12; nProtons=82; r0=1.00863; r1=-0.00044808; r2=-0.000205872;} //only Pb 207 was tested but should be the same for 208
  else if (name.EqualTo( "Pb*"))     {nNucleons = 208; pRadius = 6.624;  pSkin = 0.549;  pWPar =  0;       rType = 1;  nProtons=82;}
  else if (name.EqualTo( "PbHN"))    {nNucleons = 208; pRadius = 6.65;   pSkin = 0.460;  pWPar =  0;       rType = 1;  nProtons=82;}
  else if (name.EqualTo( "Pbpn"))    {nNucleons = 208; pRadius = 6.68;   pSkin = 0.447;  pWPar =  0;       rType = 11; nProtons=82; nRadius=6.69; nSkin=0.56; nWPar=0;}
  else if (name.EqualTo( "Pbpnrw"))  {nNucleons = 208; pRadius = 6.68;   pSkin = 0.447;  pWPar =  0;       rType = 13; nProtons=82; nRadius=6.69; nSkin=0.56; nWPar=0;}
  else if (name.EqualTo( "U"))       {nNucleons = 238; pRadius = 6.188;  pSkin = 0.54;   pWPar =  1.77;    rType = 5;  nProtons=92;}
  else if (name.EqualTo( "U2"))      {nNucleons = 238; pRadius = 6.67;   pSkin = 0.44;   pWPar =  0;       rType = 8;  nProtons=92; beta2=0.280; beta4=0.093; }
  else
    throw NucleusNotFoundGlauberException(name,"Nucleus name not found in DB",__FUNCTION__);

  nucleusType.setName(name);
  nucleusType.setNProtons(nProtons);
  nucleusType.setNNeutrons(nNucleons-nProtons);
  nucleusType.setBeta2(beta2);
  nucleusType.setBeta4(beta4);
  nucleusType.setMaxRadius(3.0*1.2*power(nNucleons,0.33333));
  nucleusType.setRadiusProton(pRadius);
  nucleusType.setSkinProton(pSkin);
  nucleusType.setWProton(pWPar);
  nucleusType.setBeta2(beta2);
  nucleusType.setBeta4(beta4);
  nucleusType.setA(aPar);
  nucleusType.setB(bPar);
  nucleusType.setR0(r0);
  nucleusType.setR1(r1);
  nucleusType.setR2(r2);
  nucleusType.setRadialDistType(rType);
  if (nRadius>0 || nSkin>0 || nWPar>0)
    {
    nucleusType.setRadiusNeutron(nRadius);
    nucleusType.setSkinNeutron(nSkin);
    nucleusType.setWNeutron(nWPar);
    }
  else
    {
    nucleusType.setRadiusNeutron(pRadius);
    nucleusType.setSkinNeutron(pSkin);
    nucleusType.setWNeutron(pWPar);
    }
  }


}  // namespace CAP



