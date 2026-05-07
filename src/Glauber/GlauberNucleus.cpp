#include "GlauberNucleus.hpp"
#include "PrintHelpers.hpp"
#include "TRandom.h"

ClassImp(CAP::GlauberNucleus)

namespace CAP
{
  using CAP::MATH::pi;
  using CAP::MATH::twoPi;

  GlauberNucleus::GlauberNucleus()
  :
  _nucleusType(),
  _nucleons(),
  _allowOverlap(true),
  _useLattice(false),
  _latticeType(0),
  _nTrialsMax(20),
  _nnDistanceMin(-1.0),
  _nnDistanceMinSq(-1.0),
  _nodeDistance(0.01),
  _recenteringMode(0),
  _overlapMode(0),
  _userSigmaMode(0),
  _protonDistribution(),
  _neutronDistribution()
  {   }

  GlauberNucleus::GlauberNucleus(const GlauberNucleus & src)
  :
  _nucleusType(src._nucleusType),
  _nucleons(src._nucleons),
  _allowOverlap(src._allowOverlap),
  _useLattice(src._useLattice),
  _latticeType(src._latticeType),
  _nTrialsMax(src._nTrialsMax),
  _nnDistanceMin(src._nnDistanceMin),
  _nnDistanceMinSq(src._nnDistanceMinSq),
  _nodeDistance(src._nodeDistance),
  _recenteringMode(src._recenteringMode),
  _overlapMode(src._overlapMode),
  _userSigmaMode(src._userSigmaMode),
  _protonDistribution(src._protonDistribution),
  _neutronDistribution(src._neutronDistribution)
  {   }

  GlauberNucleus GlauberNucleus::operator=(const GlauberNucleus & rhs)
  {
  if (this!=&rhs)
    {
    _nucleusType        = rhs._nucleusType;
    _nucleons           = rhs._nucleons;
    _allowOverlap       = rhs._allowOverlap;
    _useLattice         = rhs._useLattice;
    _latticeType        = rhs._latticeType;
    _nTrialsMax         = rhs._nTrialsMax;
    _nnDistanceMin      = rhs._nnDistanceMin;
    _nnDistanceMinSq    = rhs._nnDistanceMinSq;
    _nodeDistance       = rhs._nodeDistance;
    _recenteringMode    = rhs._recenteringMode;
    _overlapMode        = rhs._overlapMode;
    _userSigmaMode      = rhs._userSigmaMode;
    _protonDistribution = rhs._protonDistribution;
    _neutronDistribution = rhs._neutronDistribution;
    }
  return *this;
  }

  GlauberNucleus::~GlauberNucleus()
  {
  clear();
  }
  
  //!
  //!Returns the number of wounded nucleons in this nucleus
  //!
  int GlauberNucleus::nWoundedNucleons() const
  {
  int n = 0;
  for (auto & nucleon : _nucleons)
    {
    if (nucleon.isWounded()) n++;
    }
  return n;
  }

  //!
  //!Returns the number of wounded protons in this nucleus
  //!
  int GlauberNucleus::nWoundedProtons() const
  {
  int n = 0;
  for (auto & nucleon : _nucleons)
    {
    if (nucleon.isWounded()  &&  nucleon.isProton()) n++;
    }
  return n;
  }

  //!
  //!Returns the number of wounded neutrons in this nucleus
  //!
  int GlauberNucleus::nWoundedNeutrons() const
  {
  int n = 0;
  for (auto & nucleon : _nucleons)
    {
    if (nucleon.isWounded()  &&  nucleon.isNeutron()) n++;
    }
  return n;
  }


  //!
  //!Returns the number of hard wounded nucleons in this nucleus
  //!
  int GlauberNucleus::nWoundedHardNucleons() const
  {
  int n = 0;
  for (auto & nucleon : _nucleons)
    {
    if (nucleon.isWoundedHard()) n++;
    }
  return n;
  }

  //!
  //!Returns the number of hard wounded protons in this nucleus
  //!
  int GlauberNucleus::nWoundedHardProtons() const
  {
  int n = 0;
  for (auto & nucleon : _nucleons)
    {
    if (nucleon.isWoundedHard()  &&  nucleon.isProton() ) n++;
    }
  return n;
  }

  //!
  //!Returns the number of hard wounded neutrons in this nucleus
  //!
  int GlauberNucleus::nWoundedHardNeutrons() const
  {
  int n = 0;
  for (auto & nucleon : _nucleons)
    {
    if (nucleon.isWoundedHard()  &&  nucleon.isNeutron() ) n++;
    }
  return n;
  }

  int GlauberNucleus::nCollisions() const
  {
  int n = 0;
  for (auto & nucleon : _nucleons) n += nucleon.nCollisions();
  return n;
  }

  int GlauberNucleus::nPCollisions() const
  {
  int n = 0;
  for (auto & nucleon : _nucleons)
    if (nucleon.isProton()) n += nucleon.nCollisions();
  return n;
  }

  int GlauberNucleus::nNCollisions() const
  {
  int n = 0;
  for (auto & nucleon : _nucleons)
    if (nucleon.isNeutron()) n += nucleon.nCollisions();
  return n;
  }

  int GlauberNucleus::nHardCollisions() const
  {
  int n = 0;
  for (auto & nucleon : _nucleons) n += nucleon.nHardCollisions();
  return n;
  }

  int GlauberNucleus::nPHardCollisions() const
  {
  int n = 0;
  for (auto & nucleon : _nucleons)
    if (nucleon.isProton()) n += nucleon.nHardCollisions();
  return n;
  }

  int GlauberNucleus::nNHardCollisions() const
  {
  int n = 0;
  for (auto & nucleon : _nucleons)
    if (nucleon.isNeutron()) n += nucleon.nHardCollisions();
  return n;
  }

  //!
  //!Returns the number of spectator nucleons in this nucleus
  //!
  int GlauberNucleus::nSpectatorNucleons() const
  {
  int n = 0;
  for (auto & nucleon : _nucleons)
    {
    if (nucleon.isSpectator()) n++;
    }
  return n;
  }

  //!
  //!Returns the number of spectator protons in this nucleus
  //!
  int GlauberNucleus::nSpectatorProtons() const
  {
  int n = 0;
  for (auto & nucleon : _nucleons)
    {
    if (nucleon.isSpectator()  &&  nucleon.isProton() ) n++;
    }
  return n;
  }

  //!
  //!Returns the number of spectator neutrons in this nucleus
  //!
  int GlauberNucleus::nSpectatorNeutrons() const
  {
  int n = 0;
  for (auto & nucleon : _nucleons)
    {
    if (nucleon.isSpectator()  &&  nucleon.isNeutron() ) n++;
    }
  return n;
  }

  //!
  //! Create the required array of nucleons
  //! which will be used and reused.
  //!
  void GlauberNucleus::generateNucleons()
  {
  int nProtons  = _nucleusType.nProtons();
  int nNeutrons = _nucleusType.nNeutrons();
//  printValue("name",name());
//  printValue("nProtons",nProtons);
//  printValue("nNeutrons",nNeutrons);
  for (int i=0; i<nProtons; ++i)
    {
    GlauberNucleon nucleon;
    nucleon.setTypeToProton();
    _nucleons.push_back(nucleon);
    }
  for (int i=0; i<nNeutrons; ++i)
    {
    GlauberNucleon nucleon;
    nucleon.setTypeToNeutron();
    _nucleons.push_back(nucleon);
    }
  }

  //!
  //! Generate random positions for all the nucleons
  //!
  void GlauberNucleus::positionNucleons()
  {
  int nNucleons = _nucleons.size();
  //printValue("nNucleons",nNucleons);
  if (nNucleons<1) throw InvalidValueGlauberException(nNucleons,"Nucleus has nNucleons<1",__FUNCTION__);
  else if (nNucleons == 1) // proton
    {
    _nucleons[0].reset();
    _nucleons[0].setRndmPositionInSphere(_protonDistribution);
    }
  else if (nNucleons == 2) // deuteron
    {

    if (_nucleusType.radialDistType() == 4)
      {
      // place neutron opposite of proton
      _nucleons[0].reset();
      _nucleons[1].reset();
      //_nucleons[0].setRndmPositionInSphereHulten(_neutronDistribution);
      _nucleons[1].position() = -_nucleons[0].position();
      }
    else
      {
      // place proton and neutron both ramdomly
      _nucleons[0].reset();
      _nucleons[1].reset();
      //_nucleons[0].setRndmPositionInSphereHulten(_protonDistribution);
      //_nucleons[1].setRndmPositionInSphereHulten(_neutronDistribution);
      }
    }
  else
    {
    // all other nuclei
    for (auto & nucleon : _nucleons)
      {
      nucleon.reset();
      bool overlaps = true;
      int  iTrialOverlap = 0;
      while (overlaps)
        {
        iTrialOverlap++;
        if (iTrialOverlap>_nTrialsMax)
          throw IterationsGlauberException(_nTrialsMax,iTrialOverlap,"Too Many Iterations In Overlap Test",__FUNCTION__);
        switch (_nucleusType.radialDistType())
          {
            case 1: // large spherical nucleus use proton profile only
            {
            nucleon.setRndmPositionInSphere(_protonDistribution);
            break;
            }
            case 5:
            case 7:
            {
            //use a cube and check the nucleon is within a desired radius
            bool notInside = true;
            int  insideTest = 0;
            while (notInside)
              {
              insideTest++;
              if (insideTest>_nTrialsMax)
                throw IterationsGlauberException(_nTrialsMax,
                                                 insideTest,
                                                 "Too Many Iterations In Inside Test",
                                                 __FUNCTION__);
              nucleon.setRndmPositionInCube(_nucleusType.maxRadius());
              //VectorLorentz & position = nucleon.position();
              //              double r     = _position.Mag();
              //              double theta = _position.Theta();
              //              double Rtheta = 0;
              //              if (_nucleusType.radialDistType == 5)
              //                Rtheta = _nucleusType._radiusProton
              //                  + _nucleusType._skinProton*TMath::Cos(theta)*TMath::Cos(theta);
              //              else if (_nucleusType.radialDistType == 7)
              //                Rtheta = _nucleusType._radiusProton*(1+_nucleusType.shapeBeta2*ROOT::MATH::sph_legendre(2,0,theta)+_nucleusType.shapeBeta4*ROOT::MATH::sph_legendre(4,0,theta));
              double prob = 1.0;// 1.0/(1.0+exp((R-Rtheta)/_nucleusType._skinProton));
              if (gRandom->Rndm()<=prob) notInside = false;
              }
            }
            break;

            case 8:
            case 14: // deformed nucleus
            {
            // throw using an r, theta two parameter function
            if (nucleon.isProton())
              nucleon.setRndmPositionInSphere(_protonDistribution);
            else
              nucleon.setRndmPositionInSphere(_neutronDistribution);
            }
            break;

            default:
            {
            // all other types are thrown while distinguishing
            // protons and neutrons
            if (_useLattice)
              {
              // "grid/lattice" mode
              throw GlauberException("Not implemented",__FUNCTION__);
              }
            else
              {
              // using a continuous mode
              if (nucleon.isProton())
                nucleon.setRndmPositionInSphere(_protonDistribution);
              else
                nucleon.setRndmPositionInSphere(_neutronDistribution);
              }
            }
          } // switch (_nucleusType.radialDistType)
        if (_allowOverlap)
          break;
        else if (hasNucleonCloseTo(nucleon,_nnDistanceMinSq))
          continue;
        }
      if (nNucleons>2)
        {
        double xRot = gRandom->Rndm()*twoPi();
        double yRot = gRandom->Rndm()*twoPi();
        double zRot = gRandom->Rndm()*twoPi();
        nucleon.rotateXYZ(xRot,yRot,zRot);
        }
      }
    }
  if (_recenteringMode>0) recenterNucleus();
  }

  //!
  //!Recenter all the nucleus and all its nucleons
  //!
  void GlauberNucleus::recenterNucleus()
  {
  VectorLorentz locatedCenter;
  findNucleusCenter(locatedCenter);
  if (locatedCenter.modulus3() < 0.02) return;
  switch (_recenteringMode)
    {
      default:
      case 1: // recenter the nucleus on its current center
      {
      VectorLorentz s = -locatedCenter;
      shiftNucleus(s);
      break;
      }
      case 2:
      {
      // shift the first nucleon to make the center at (0,0,0,0)
      VectorLorentz & firstPosition = _nucleons[0].position();
      firstPosition -= locatedCenter;
      break;
      }
    }
  }

  //!
  //!Find the CM of all the nucleons of this nucleus
  //!
  void GlauberNucleus::findNucleusCenter(VectorLorentz & center)
  {
  VectorLorentz sum;
  sum.setTXYZ(0.0,0.0,0.0,0.0);
  double count = 1.0;
  for (auto & nucleon : _nucleons)
    {
    count += 1;
    sum += nucleon.position();
    }
  double x = sum.x()/count;
  double y = sum.y()/count;
  double z = sum.z()/count;
//  printValue("GlauberNucleus::findNucleusCenter() count", count);
//  printValue("GlauberNucleus::findNucleusCenter() x", x);
//  printValue("GlauberNucleus::findNucleusCenter() y", y);
//  printValue("GlauberNucleus::findNucleusCenter() z", z);
  center.setTXYZ(0.0,x,y,z);
  }

  //!
  //! Shift all the nucleons of the given nucleus in the given direction
  //!
  void GlauberNucleus::shiftNucleus(VectorLorentz & shift)
  {
  for (auto & nucleon : _nucleons)
    {
    nucleon.shiftPosition(shift);
    }
  }

  //!
  //! rotate the position of all the nucleons of this nucleus
  //! by the given azimuthal angle phi relative to the x axis.
  //!
  void GlauberNucleus::rotateX(double phi)
  {
  for (auto & nucleon : _nucleons)  nucleon.rotateX(phi);
  }

  //!
  //! rotate the position of all the nucleons of this nucleus
  //! by the given azimuthal angle phi relative to the z axis.
  //!
  void GlauberNucleus::rotateY(double phi)
  {
  for (auto & nucleon : _nucleons)  nucleon.rotateY(phi);
  }

  //!
  //! rotate the position of all the nucleons of this nucleus
  //! by the given azimuthal angle phi relative to the z axis.
  //!
  void GlauberNucleus::rotateZ(double phi)
  {
  for (auto & nucleon : _nucleons)  nucleon.rotateZ(phi);
  }

  //!
  //! rotate the position of all the nucleons of this nucleus
  //! by the given three Euler angles.
  //!
  void GlauberNucleus::rotateXYZ(double psiX, double psiY, double psiZ)
  {
  for (auto & nucleon : _nucleons)  nucleon.rotateXYZ(psiX,psiY,psiZ);
  }

  //!
  //! Reset the states wounded, hardWounded of the nucleons
  //!
  void GlauberNucleus::resetNucleons()
  {
  for (auto & nucleon : _nucleons) nucleon.reset();
  }

  //!
  //! Delete all  nucleons, and clear the nucleon container
  //!
  void GlauberNucleus::clearNucleons()
  {
  _nucleons.clear();
  }


  void GlauberNucleus::reset()
  {
  resetNucleons();
  }

  void GlauberNucleus::clear()
  {
  clearNucleons();
  }

  void GlauberNucleus::print() const
  {
  printValue("Type",         _nucleusType.name());
  printValue("nProtons",     _nucleusType.nProtons());
  printValue("nNeutrons",    _nucleusType.nNeutrons());
  printValue("nNucleons",    _nucleusType.nNucleons());
  printValue("nWoundedNucleons",     nWoundedNucleons());
  printValue("nWoundedProtons",      nWoundedProtons());
  printValue("nWoundedNeutrons",     nWoundedNeutrons());
  printValue("nWoundedHardNucleons", nWoundedHardNucleons());
  printValue("nWoundedHardProtons",  nWoundedHardProtons());
  printValue("nWoundedHardNeutrons", nWoundedHardNeutrons());
  printValue("nSpectatorNucleons",   nSpectatorNucleons());
  printValue("nSpectatorProtons",    nSpectatorProtons());
  printValue("nSpectatorNeutrons",   nSpectatorNeutrons());
  }

  //!
  //!Determine whether this nucleus has a nucleons with dSqMax of  to the given position (x,y,z)
  //!
  bool GlauberNucleus::hasNucleonCloseTo(double x, double y, double z, double dSqMax) const
  {
  if (dSqMax<=0) return false;
  for (auto & nucleon : _nucleons)
    {
    if (nucleon.isCloseTo(x,y,z,dSqMax)) return true;
    }
  return false;
  }

  //!
  //!Determine whether this nucleus has a nucleons with dSqMax of  to the given position reference.
  //!
  bool GlauberNucleus::hasNucleonCloseTo(const VectorLorentz & reference, double dSqMax) const
  {
  if (dSqMax<=0) return false;
  for (auto & nucleon : _nucleons)
    {
    if (nucleon.isCloseTo(reference,dSqMax)) return true;
    }
  return false;
  }

  //!
  //!Determine whether this nucleus has a nucleons with dSqMax of  to the given position reference.
  //!
  bool GlauberNucleus::hasNucleonCloseTo(const GlauberNucleon & reference, double dSqMax) const
  {
  if (dSqMax<=0) return false;
  for (auto & nucleon : _nucleons)
    {
    if (nucleon.isCloseTo(reference,dSqMax)) return true;
    }
  return false;
  }


  //!
  //!Returns a function that computes the probability of finding a proton/neutron a
  //!given distance from the nucleus center using the radial distribution type "radialDistType"
  //!
  void GlauberNucleus::setNucleus()
  {
  _protonDistribution.setType(  _nucleusType.radialDistType());
  _protonDistribution.setRadius(_nucleusType.radiusProton());
  _protonDistribution.setSkin(  _nucleusType.skinProton());
  _protonDistribution.setW(     _nucleusType.wProton());
  _protonDistribution.setA(     _nucleusType.a());
  _protonDistribution.setB(     _nucleusType.b());
  _protonDistribution.setBeta2( _nucleusType.beta2());
  _protonDistribution.setBeta4( _nucleusType.beta4());
  _protonDistribution.setR0(    _nucleusType.r0());
  _protonDistribution.setR1(    _nucleusType.r1());
  _protonDistribution.setR2(    _nucleusType.r2());
  _protonDistribution.setMaxRadius(_nucleusType.maxRadius());

  _neutronDistribution.setType(  _nucleusType.radialDistType());
  _neutronDistribution.setRadius(_nucleusType.radiusNeutron());
  _neutronDistribution.setSkin(  _nucleusType.skinNeutron());
  _neutronDistribution.setW(     _nucleusType.wNeutron());
  _neutronDistribution.setA(     _nucleusType.a());
  _neutronDistribution.setB(     _nucleusType.b());
  _neutronDistribution.setBeta2( _nucleusType.beta2());
  _neutronDistribution.setBeta4( _nucleusType.beta4());
  _neutronDistribution.setR0(    _nucleusType.r0());
  _neutronDistribution.setR1(    _nucleusType.r1());
  _neutronDistribution.setR2(    _nucleusType.r2());
  _neutronDistribution.setMaxRadius(_nucleusType.maxRadius());
  }

} // namespace CAP


//TVector3 & TGlauNucleus::ThrowNucleons(double xshift)
//{
//
//  if (fNucleons==0)
//    {
//    fNucleons=new TObjArray(fN);
//    fNucleons->SetOwner();
//    for (Int_t i=0; i<fN; ++i)
//      {
//      TGlauNucleon *nucleon=new TGlauNucleon();
//      nucleon->SetType(0);
//      if (i<fZ)
//        nucleon->SetType(1);
//      fNucleons->Add(nucleon);
//      }
//    }
//  if (1)
//    { //randomize p and n in nucleus
//      for (Int_t i=0,iz=0; i<fN; ++i)
//        {
//        TGlauNucleon *nucleon=(TGlauNucleon*)fNucleons->At(i);
//        double frac=double(fZ-iz)/double(fN-i);
//        double rn=gRandom->Uniform(0,1);
//        if (rn<frac)
//          {
//          nucleon->SetType(1);
//          ++iz;
//          }
//        else
//          {
//          nucleon->SetType(0);
//          }
//        }
//    }
//
//  cmscheck: /* start over here in case shift was too large */
//
//  fTrials = 0;
//  fNonSmeared = 0;
//  fPhiRot = gRandom->Rndm()*2*TMath::Pi();
//  const double cosThetaRot = 2*gRandom->Rndm()-1;
//  fThetaRot = TMath::ACos(cosThetaRot);
//  fXRot = gRandom->Rndm()*2*TMath::Pi();
//  fYRot = gRandom->Rndm()*2*TMath::Pi();
//  fZRot = gRandom->Rndm()*2*TMath::Pi();
//
//  const Bool_t hulthen = (fF==3||fF==4);
//  TString tmpname(name());
//  Bool_t _nucleonsfromfile = false;
//  if ((tmpname=="He3") || (tmpname=="H3") ||
//      (tmpname=="He4") || (tmpname=="C")   ||
//      (tmpname=="O"))
//    _nucleonsfromfile = true;
//
//  if (fN==1)
//    { //special treatment for proton
//      double r = fFunc1->GetRandom();
//      double phi = gRandom->Rndm() * 2 * TMath::Pi();
//      double ctheta = 2*gRandom->Rndm() - 1;
//      double stheta = TMath::Sqrt(1-ctheta*ctheta);
//      TGlauNucleon *nucleon=(TGlauNucleon*)(fNucleons->At(0));
//      nucleon->Reset();
//      nucleon->SetXYZ(r * stheta * TMath::Cos(phi),
//                      r * stheta * TMath::Sin(phi),
//                      r * ctheta);
//      fTrials = 1;
//
//    } else if (fN==2  &&  hulthen) { //special treatment for Hulten
//
//      double r = fFunc1->GetRandom()/2;
//      double phi = gRandom->Rndm() * 2 * TMath::Pi();
//      double ctheta = 2*gRandom->Rndm() - 1;
//      double stheta = TMath::Sqrt(1-ctheta*ctheta);
//
//      TGlauNucleon *nucleon1=(TGlauNucleon*)(fNucleons->At(0));
//      TGlauNucleon *nucleon2=(TGlauNucleon*)(fNucleons->At(1));
//      nucleon1->Reset();
//      nucleon1->SetXYZ(r * stheta * TMath::Cos(phi),
//                       r * stheta * TMath::Sin(phi),
//                       r * ctheta);
//      nucleon2->Reset();
//      if (fF==4) { // place opposite of 1
//        nucleon2->SetXYZ(-nucleon1->GetX(),
//                         -nucleon1->GetY(),
//                         -nucleon1->GetZ());
//      } else {
//        r = fFunc1->GetRandom()/2;
//        phi = gRandom->Rndm() * 2 * TMath::Pi();
//        ctheta = 2*gRandom->Rndm() - 1;
//        stheta = TMath::Sqrt(1-ctheta*ctheta);
//        nucleon2->SetXYZ(r * stheta * TMath::Cos(phi),
//                         r * stheta * TMath::Sin(phi),
//                         r * ctheta);
//      }
//      fTrials = 1;
//
//    }
//    else if (fN > 2  &&  fN < 20  &&  _nucleonsfromfile)
//      {
//
//      // if the first call, then read in the file configurations
//      if (fNucCounter == -1) {
//        // read in the ascii file into the array and step through the counter
//        char filename[100] = "foo.dat";
//        if (tmpname=="He3") {
//          sprintf(filename,"he3_plaintext.dat");
//        } else if (tmpname=="H3") {
//          sprintf(filename,"h3_plaintext.dat");
//        } else if (tmpname=="He4") {
//          sprintf(filename,"he4_plaintext.dat");
//        } else if (tmpname=="C") {
//          sprintf(filename,"carbon_plaintext.dat");
//        } else if (tmpname=="O") {
//          sprintf(filename,"oxygen_plaintext.dat");
//        }
//        cout << "Reading in " << filename << " for nucleon configurations with fN = " << fN << endl;
//        ifstream myfile;
//        myfile.open(filename);
//        if (!myfile) {
//          cout << "ERROR:  no file for nucleon configurations found with name = " << filename << endl;
//          gSystem->Exit(123);
//        }
//
//        Int_t inputcounter = 0;
//        while (myfile) {
//          if (inputcounter > 5999) break;
//          double foo;
//          if (fN == 3) {
//            myfile >> fNucArr[inputcounter][0][0] >> fNucArr[inputcounter][0][1] >> fNucArr[inputcounter][0][2]
//            >> fNucArr[inputcounter][1][0] >> fNucArr[inputcounter][1][1] >> fNucArr[inputcounter][1][2]
//            >> fNucArr[inputcounter][2][0] >> fNucArr[inputcounter][2][1] >> fNucArr[inputcounter][2][2]
//            >> foo >> foo >> foo >> foo;
//          } else if (fN == 4) {
//            // no extra data with isospin information at the end of the nucleon configurations
//            myfile >> fNucArr[inputcounter][0][0] >> fNucArr[inputcounter][0][1] >> fNucArr[inputcounter][0][2]
//            >> fNucArr[inputcounter][1][0] >> fNucArr[inputcounter][1][1] >> fNucArr[inputcounter][1][2]
//            >> fNucArr[inputcounter][2][0] >> fNucArr[inputcounter][2][1] >> fNucArr[inputcounter][2][2]
//            >> fNucArr[inputcounter][3][0] >> fNucArr[inputcounter][3][1] >> fNucArr[inputcounter][3][2];
//          } else if (fN == 12) {
//            // no extra data with isospin information at the end of the nucleon configurations
//            // two extra words at the beginning --> foo foo
//            myfile >> foo >> foo
//            >> fNucArr[inputcounter][0][0] >> fNucArr[inputcounter][0][1] >> fNucArr[inputcounter][0][2]
//            >> fNucArr[inputcounter][1][0] >> fNucArr[inputcounter][1][1] >> fNucArr[inputcounter][1][2]
//            >> fNucArr[inputcounter][2][0] >> fNucArr[inputcounter][2][1] >> fNucArr[inputcounter][2][2]
//            >> fNucArr[inputcounter][3][0] >> fNucArr[inputcounter][3][1] >> fNucArr[inputcounter][3][2]
//            >> fNucArr[inputcounter][4][0] >> fNucArr[inputcounter][4][1] >> fNucArr[inputcounter][4][2]
//            >> fNucArr[inputcounter][5][0] >> fNucArr[inputcounter][5][1] >> fNucArr[inputcounter][5][2]
//            >> fNucArr[inputcounter][6][0] >> fNucArr[inputcounter][6][1] >> fNucArr[inputcounter][6][2]
//            >> fNucArr[inputcounter][7][0] >> fNucArr[inputcounter][7][1] >> fNucArr[inputcounter][7][2]
//            >> fNucArr[inputcounter][8][0] >> fNucArr[inputcounter][8][1] >> fNucArr[inputcounter][8][2]
//            >> fNucArr[inputcounter][9][0] >> fNucArr[inputcounter][9][1] >> fNucArr[inputcounter][9][2]
//            >> fNucArr[inputcounter][10][0] >> fNucArr[inputcounter][10][1] >> fNucArr[inputcounter][10][2]
//            >> fNucArr[inputcounter][11][0] >> fNucArr[inputcounter][11][1] >> fNucArr[inputcounter][11][2];
//          } else if (fN == 16) {
//            // no extra data with isospin information at the end of the nucleon configurations
//            myfile >> fNucArr[inputcounter][0][0] >> fNucArr[inputcounter][0][1] >> fNucArr[inputcounter][0][2]
//            >> fNucArr[inputcounter][1][0] >> fNucArr[inputcounter][1][1] >> fNucArr[inputcounter][1][2]
//            >> fNucArr[inputcounter][2][0] >> fNucArr[inputcounter][2][1] >> fNucArr[inputcounter][2][2]
//            >> fNucArr[inputcounter][3][0] >> fNucArr[inputcounter][3][1] >> fNucArr[inputcounter][3][2]
//            >> fNucArr[inputcounter][4][0] >> fNucArr[inputcounter][4][1] >> fNucArr[inputcounter][4][2]
//            >> fNucArr[inputcounter][5][0] >> fNucArr[inputcounter][5][1] >> fNucArr[inputcounter][5][2]
//            >> fNucArr[inputcounter][6][0] >> fNucArr[inputcounter][6][1] >> fNucArr[inputcounter][6][2]
//            >> fNucArr[inputcounter][7][0] >> fNucArr[inputcounter][7][1] >> fNucArr[inputcounter][7][2]
//            >> fNucArr[inputcounter][8][0] >> fNucArr[inputcounter][8][1] >> fNucArr[inputcounter][8][2]
//            >> fNucArr[inputcounter][9][0] >> fNucArr[inputcounter][9][1] >> fNucArr[inputcounter][9][2]
//            >> fNucArr[inputcounter][10][0] >> fNucArr[inputcounter][10][1] >> fNucArr[inputcounter][10][2]
//            >> fNucArr[inputcounter][11][0] >> fNucArr[inputcounter][11][1] >> fNucArr[inputcounter][11][2]
//            >> fNucArr[inputcounter][12][0] >> fNucArr[inputcounter][12][1] >> fNucArr[inputcounter][12][2]
//            >> fNucArr[inputcounter][13][0] >> fNucArr[inputcounter][13][1] >> fNucArr[inputcounter][13][2]
//            >> fNucArr[inputcounter][14][0] >> fNucArr[inputcounter][14][1] >> fNucArr[inputcounter][14][2]
//            >> fNucArr[inputcounter][15][0] >> fNucArr[inputcounter][15][1] >> fNucArr[inputcounter][15][2];
//          }
//          ++inputcounter;
//        }
//        myfile.close();
//        fNucCounter=0;
//      } // done reading in the file the first time
//
//      if (fNucCounter > 5999)
//        fNucCounter = 0;
//
//      // change to loop over fN _nucleons!
//      for (Int_t i = 0; i<fN; ++i) {
//        TGlauNucleon *nucleon=(TGlauNucleon*)(fNucleons->At(i));
//        nucleon->Reset();
//        nucleon->SetXYZ(fNucArr[fNucCounter][i][0],
//                        fNucArr[fNucCounter][i][1],
//                        fNucArr[fNucCounter][i][2]);
//        nucleon->RotateXYZ(fPhiRot,fThetaRot);
//      }
//
//      ++fNucCounter;
//      fTrials = 1;
//      }
//    else
//      { // all other nuclei
//
//        const double startingEdge  = 20; // throw nucleons within a cube of this size (fm)
//        const double startingEdgeX = startingEdge + fNodeDist*gRandom->Rndm() - 0.5*fNodeDist;
//        const double startingEdgeY = startingEdge + fNodeDist*gRandom->Rndm() - 0.5*fNodeDist;
//        const double startingEdgeZ = startingEdge + fNodeDist*gRandom->Rndm() - 0.5*fNodeDist;
//        const Int_t nslots = 2*startingEdge/fNodeDist+1;
//        if (fNodeDist>0)
//          {
//          if (fMinDist>fNodeDist)
//            {
//            cout << "Minimum distance (nucleon hard core diameter) ["
//            << fMinDist << "] cannot be larger than the nodal spacing of the grid ["
//            << fNodeDist << "]." << endl;
//            cout << "Quitting...." << endl;
//            gSystem->Exit(123);
//            }
//          if (!fIsUsed)
//            fIsUsed = new TBits(nslots*nslots*nslots);
//          else
//            fIsUsed->ResetAllBits();
//          }
//        for (Int_t i = 0; i<fN; ++i)
//          {
//          TGlauNucleon *nucleon=(TGlauNucleon*)(fNucleons->At(i));
//          nucleon->Reset();
//          while (1)
//            {
//            ++fTrials;
//            Bool_t nucleon_inside = 0;
//            double x=999, xsmeared=999;
//            double y=999, ysmeared=999;
//            double z=999, zsmeared=999;
//            if (fF==5||fF==7)
//              { // the extended way, throw in a box and test the weight
//                while (!nucleon_inside)
//                  {
//                  x = (fR*2)*(gRandom->Rndm() * 2 - 1);
//                  y = (fR*2)*(gRandom->Rndm() * 2 - 1);
//                  z = (fR*2)*(gRandom->Rndm() * 2 - 1);
//                  double r = TMath::Sqrt(x*x+y*y);
//                  double theta = TMath::ATan2(r,z);
//                  double R = TMath::Sqrt(x*x+y*y+z*z);
//                  double Rtheta = fR;
//                  if (fF==5)
//                    Rtheta= fR + fW*TMath::Cos(theta)*TMath::Cos(theta);
//                  if (fF==7)
//#ifdef HAVE_MATHMORE
//                    Rtheta = fR*(1+fBeta2*ROOT::MATH::sph_legendre(2,0,theta)+fBeta4*ROOT::MATH::sph_legendre(4,0,theta));
//#else
//                  cerr << "Should not end here because you do not have libMathMore" << endl;
//#endif
//                  double prob = 1/(1+TMath::Exp((R-Rtheta)/fA));
//                  if (gRandom->Rndm()<prob)
//                    nucleon_inside=1;
//                  }
//              } else if ((fF==8) || (fF==14)) { // use TF2
//                double r;
//                double theta;
//                fFunc3->GetRandom2(r,theta);
//                double phi = 2*TMath::Pi()*gRandom->Rndm();
//                x = r * TMath::Sin(phi) * TMath::Sin(theta);
//                y = r * TMath::Cos(phi) * TMath::Sin(theta);
//                z = r *                   TMath::Cos(theta);
//              } else { // all other types
//                TF1 *ff = fFunc1;
//                if ((fFunc2)  &&  (nucleon->type()==0))
//                  ff = fFunc2;
//                if (fNodeDist<=0) { // "continuous" mode
//                  double r = ff->GetRandom();
//                  double phi = 2*TMath::Pi()*gRandom->Rndm();
//                  double ctheta = 2*gRandom->Rndm() - 1 ;
//                  double stheta = TMath::Sqrt(1-ctheta*ctheta);
//                  x = r * stheta * TMath::Cos(phi);
//                  y = r * stheta * TMath::Sin(phi);
//                  z = r * ctheta;
//                } else { // "grid/lattice" mode
//                  Int_t iNode = Int_t((2*startingEdge/fNodeDist)*gRandom->Rndm());
//                  Int_t jNode = Int_t((2*startingEdge/fNodeDist)*gRandom->Rndm());
//                  Int_t kNode = Int_t((2*startingEdge/fNodeDist)*gRandom->Rndm());
//                  Int_t index=iNode*nslots*nslots+jNode*nslots+kNode;
//                  if (fIsUsed->TestBitNumber(index))
//                    continue;
//                  if (fLattice==1) {       // Primitive cubic system (PCS) -> https://en.wikipedia.org/wiki/Cubic_crystal_system
//                    x = fNodeDist*(iNode) - startingEdgeX;
//                    y = fNodeDist*(jNode) - startingEdgeY;
//                    z = fNodeDist*(kNode) - startingEdgeZ;
//                  } else if (fLattice==2) { //Body centered cubic (BCC) -> http://mathworld.wolfram.com/CubicClosePacking.html
//                    x = 0.5*fNodeDist*(-iNode+jNode+kNode) - 0.5*startingEdgeX;
//                    y = 0.5*fNodeDist*(+iNode-jNode+kNode) - 0.5*startingEdgeY;
//                    z = 0.5*fNodeDist*(+iNode+jNode-kNode) - 0.5*startingEdgeZ;
//                  } else if (fLattice==3) { //Face Centered Cubic (FCC) -> http://mathworld.wolfram.com/CubicClosePacking.html
//                    x = 0.5*fNodeDist*(jNode+kNode) - startingEdgeX;
//                    y = 0.5*fNodeDist*(iNode+kNode) - startingEdgeY;
//                    z = 0.5*fNodeDist*(iNode+jNode) - startingEdgeZ;
//                  } else {                  //Hexagonal close packing (HCP) -> https://en.wikipedia.org/wiki/Close-packing_of_equal_spheres
//                    x = 0.5*fNodeDist*(2*iNode+((jNode+kNode)%2))          - startingEdgeX;
//                    y = 0.5*fNodeDist*(TMath::Sqrt(3)*(jNode+(kNode%2)/3)) - startingEdgeY;
//                    z = 0.5*fNodeDist*(kNode*2*TMath::Sqrt(6)/3)           - startingEdgeZ;
//                  }
//                  const double r2 = x*x + y*y + z*z;
//                  const double r  = TMath::Sqrt(r2);
//                  if ((r>fMaxR)||(r2*gRandom->Rndm()>ff->Eval(r)))
//                    continue;
//                  if (fSmearing>0.0) {
//                    Int_t nAttemptsToSmear = 0;
//                    while (1) {
//                      xsmeared = x*gRandom->Gaus(1.0,fSmearing);
//                      ysmeared = y*gRandom->Gaus(1.0,fSmearing);
//                      zsmeared = z*gRandom->Gaus(1.0,fSmearing);
//                      nAttemptsToSmear++;
//                      if (TestMinDist(i,xsmeared,ysmeared,zsmeared)) {
//                        x = xsmeared;
//                        y = ysmeared;
//                        z = zsmeared;
//                        break;
//                      }
//                      if (nAttemptsToSmear>=99) {
//                        cerr << "Could not place on this node :: [" << x <<","<< y <<","<< z <<"] r = " << TMath::Sqrt(x*x+y*y+z*z) << " fm; "
//                        << "Node (" << iNode << "," << jNode << "," << kNode << ") not smeared !!!" << endl;
//                        ++fNonSmeared;
//                        break;
//                      }
//                    }
//                  }
//                  fIsUsed->SetBitNumber(index);
//                } /* end "grid/lattice mode" */
//              }
//            nucleon->SetXYZ(x,y,z);
//            if (fF==5||fF==7||fF==8||fF==14)
//              nucleon->RotateXYZ(fPhiRot,fThetaRot); // Uranium etc.
//            if (fNodeDist>0) {
//              nucleon->rotateXYZ(fXRot,fYRot,fZRot);
//              break;
//            }
//            if (TestMinDist(i,x,y,z))
//              break;
//            }
//          }
//      }
//
//  // calculate center of mass
//  double sumx=0;
//  double sumy=0;
//  double sumz=0;
//  for (Int_t i = 0; i<fN; ++i) {
//    TGlauNucleon *nucleon=(TGlauNucleon*)(fNucleons->At(i));
//    sumx += nucleon->GetX();
//    sumy += nucleon->GetY();
//    sumz += nucleon->GetZ();
//  }
//  sumx = sumx/fN;
//  sumy = sumy/fN;
//  sumz = sumz/fN;
//
//  static TVector3 finalShift;
//  finalShift.SetXYZ(sumx,sumy,sumz);
//  if (finalShift.Mag()>fSmax)
//    goto cmscheck;
//  double fsumx = 0;
//  double fsumy = 0;
//  double fsumz = 0;
//  if (fRecenter==1) {
//    fsumx = sumx;
//    fsumy = sumy;
//    fsumz = sumz;
//  } else if (fRecenter==2) {
//    TGlauNucleon *nucleon=(TGlauNucleon*)(fNucleons->At(fN-1));
//    double x = nucleon->GetX() - fN*sumx;
//    double y = nucleon->GetY() - fN*sumy;
//    double z = nucleon->GetZ() - fN*sumz;
//    nucleon->SetXYZ(x,y,z);
//  } else if ((fRecenter==3)||(fRecenter==4)) {
//    TVector3 zVec;
//    zVec.SetXYZ(0,0,1);
//    TVector3 shiftVec;
//    shiftVec.SetXYZ(sumx,sumy,sumz);
//    TVector3 orthVec;
//    orthVec = shiftVec.Cross(zVec);
//    TRotation myRot;
//    myRot.Rotate(shiftVec.Angle(zVec),orthVec);
//    TVector3 myNuc;
//    for (Int_t i = 0; i<fN; ++i) {
//      TGlauNucleon *nucleon=(TGlauNucleon*)(fNucleons->At(i));
//      myNuc.SetXYZ(nucleon->GetX(),nucleon->GetY(),nucleon->GetZ());
//      myNuc.Transform(myRot);
//      nucleon->SetXYZ(myNuc.X(), myNuc.Y(), myNuc.Z());
//    }
//    if (fRecenter==3)
//      fsumz = shiftVec.Mag();
//  }
//
//  // recenter and shift
//  for (Int_t i = 0; i<fN; ++i) {
//    TGlauNucleon *nucleon=(TGlauNucleon*)(fNucleons->At(i));
//    nucleon->SetXYZ(nucleon->GetX()-fsumx + xshift,
//                    nucleon->GetY()-fsumy,
//                    nucleon->GetZ()-fsumz);
//  }
//
//  return finalShift;
//
//}

