/* **********************************************************************
 * Copyright (C) 2019-2024, Claude Pruneau, Victor Gonzalez
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   04/01/2024
 *
 * *********************************************************************/
#ifndef CAP__ParticleType
#define CAP__ParticleType
#include "Aliases.hpp"
#include "SelectionGenerator.hpp"
#include "ParticleDecayMode.hpp"
#include "PhysicsConstants.hpp"
#include "Exceptions.hpp"
#include "PrintHelpers.hpp"

using namespace std;
using namespace CAP::Physics;

namespace CAP
{

class ParticleDecayMode;

//!
//!This class defines the notion of particle type used in CAP.  Virtually all elementary particle properties
//!are represented and stored. Property values are stored and read from a particle property file. Property values are based on
//!the particle data book maintained by the particle data group (PDG).
//!
class ParticleType
{
  //friend class ParticleDbXmlWriter;

protected:

  int    _index;            //!<Sequential _index on import
  String _name;             //!<ParticleType _name: e.g., pip
  String _title;            //!<ParticleType _title: e.g., #pi^{+}
  int    _privateCode;      //!<Code used in this package only (convenience)
  int    _pdgCode;          //!<Monte-Carlo number according PDG
  double _mass;             //!<ParticleType mass  uncertainty(GeV)
  double _massUncertainty;  //!<ParticleType mass (GeV)
  double _charge;           //!<charge
  double _width;            //!<decay width
  double _widthUncertainty; //!<ParticleType mass uncertainty(GeV)
  double _spin;             //!<_spin
  double _isospin;          //!<_isospin
  double _isospin3;         //!<_isospin 3rd component
  int    _quarks;           //!<number of _quarks of each flavor
  int    _antiquarks;       //!<number of anti _quarks of each flavor
  int    _leptons;          //!<number of _leptons of each flavor
  int    _baryon;           //!<_baryon number
  bool   _enabled;          //!<particle declared not used
  bool   _decayEnabled;     //!<particle decays are turned off externally
  int    _antiParticleIndex; //!< _index (in the active DB) of the antiparticle of this particle.
  std::vector<ParticleDecayMode> _decayModes; //!<Array of decay modes
  SelectionGenerator _decayRndmSelector;
  double _beta;           //! GlauberNucleus deformation parameter

public:

  enum QuarkFlavors { kDown, kUp, kStrange, kCharm, kBottom, kTop };
  enum LeptonFlavors { kElectron, kElectronNeutrino, kMuon, kMuonNeutrino, kTau, kTauNeutrino };


  ParticleType();
  ParticleType(const ParticleType & source);
  virtual ~ParticleType();
  ParticleType & operator=(const ParticleType & source);
  void setupDecayGenerator();
  void generateDecay(int & n, int * pid);

  int index() const
  {
  return _index;
  }

  //!
  //! Get the name of the particle
  //!
  const String & name() const
  {
  return _name;
  }

  //!
  //! Get the _title  of the particle
  //!
  const String & title() const
  {
  return _title;
  }

  //!
  //!Set the number of _quarks of each flavor on the given value based on a 6 digit number
  //!of the form  tbcsud where each letter represents the number of _quarks on the given flavor
  //!
  void setQuarkNumbers(int value)
  {
  if (value/1000000 > 0) throw Exception("More than 6 quark flavors??? value/1000000 > 0",__FUNCTION__);
  _quarks = value;
  }

  //!
  //!Set the number ofanti _quarks of each flavor on the given value based on a 6 digit number
  //!of the form  tbcsud where each letter represents the number of _quarks on the given flavor
  //!
  void setAntiQuarkNumbers(int value)
  {
  if (value/1000000 > 0) throw Exception("More than 6 quark flavors??? value/1000000 > 0",__FUNCTION__);
  _quarks = value;
  }

  void setLeptonNumbers(int value)
  {
  if (value/1000000 > 0) throw Exception("More than 6 lepton flavors??? value/1000000 > 0",__FUNCTION__);
  _leptons = value;
  }

  int parseNumber(int flavorIndex, int content) const
  {
  if (content/1000000 > 0) throw Exception("More than 6 quark flavors??? value/1000000 > 0",__FUNCTION__);
  int value = 0;
  switch (flavorIndex)
    {
      case 0:  value = int(content  % 10); break;
      case 1:  value = int(content/10  % 10); break;
      case 2:  value = int(content/100  % 10); break;
      case 3:  value = int(content/1000  % 10); break;
      case 4:  value = int(content/10000  % 10); break;
      case 5:  value = int(content/100000  % 10); break;
      default: throw Exception("Invalid flavor _index",__FUNCTION__);
    }
  return value;
  }

  //!
  //!Set the number of _quarks of the given flavor _index
  //!For boson such a pion-zero, u and u-bar will be set
  //!For phi-meson, s and s-bar are set, etc.
  //!
  int quarkNumber(unsigned int flavorIndex) const
  {
  return parseNumber(flavorIndex,_quarks);
  }

  int antiQuarkNumber(unsigned int flavorIndex) const
  {
  return parseNumber(flavorIndex,_antiquarks);
  }

  int netQuarkNumber(unsigned int flavorIndex) const
  {
  return quarkNumber(flavorIndex) - antiQuarkNumber(flavorIndex);
  }

  int baryonNumber() const
  {
  int nd = quarkNumber(kDown);
  int nu = quarkNumber(kUp);
  int ns = quarkNumber(kStrange);
  int nc = quarkNumber(kCharm);
  int nb = quarkNumber(kBottom);
  int nt = quarkNumber(kTop);
  int nq  = nd + nu + ns + nc + nb + nt;
  int ndb = antiQuarkNumber(kDown);
  int nub = antiQuarkNumber(kUp);
  int nsb = antiQuarkNumber(kStrange);
  int ncb = antiQuarkNumber(kCharm);
  int nbb = antiQuarkNumber(kBottom);
  int ntb = antiQuarkNumber(kTop);
  int nqb = ndb + nub + nsb + ncb + nbb + ntb;
  return (nq - nqb)/3;
  }

  //!
  //!Get the lepton number for the given leptonIndex
  //!
  //!leptonIndex : 0    for electron or electron-neutrino
  //!leptonIndex : 1    for muon or muon-neutrino
  //!leptonIndex : 2    for tau or tau-neutrino
  //!
  int leptonNumber(unsigned int leptonIndex)  const
  {
  int _charge   =  parseNumber(2*leptonIndex,_leptons);
  if (_charge!=0) return _leptons >0 ? _charge : -_charge;
  int neutrino =  parseNumber(2*leptonIndex+1,_leptons);
  if (neutrino!=0) return _leptons >0 ? neutrino : -neutrino;
  return 0;
  }

  //!
  //! Get the PDG code of the antiparticle of this particle.
  //!
  int antiParticlePdgCode() const
  {
  return (baryonNumber()==0  && 
          _charge==0  && 
          strangeness()==0  && 
          charm()==0  && 
          bottom()==0)? _pdgCode : -_pdgCode;
  }

  //!
  //! Get the PDG code of this particle.
  //!
  int pdgCode() const
  {
  return _pdgCode;
  }

  //!
  //! Get the private  code of this particle (user defined).
  //!
  int privateCode() const
  {
  return _privateCode;
  }



  //!
  //! Get the mass of this particle in GeV/c^2.
  //!
  inline double mass() const
  {
  return _mass;  // GeV
  }

  //!
  //! Get the mass of this particle in kilograms.
  //!
  inline double massInKg() const
  {
  return _mass*1.79E-27;
  }


  //!
  //! Get the decay width of this particle in GeV/c^2.
  //!
  inline double width() const
  {
  return _width;
  }

  //!
  //! Get the decay width uncertainty of this particle in GeV/c^2.
  //!
  inline double widthUncertainty() const
  {
  return _widthUncertainty;
  }

  //!
  //! Get the _spin of this particle in h-bar units
  //!
  inline double spin() const
  {
  return _spin; // hbar units
  }

  //!
  //! Get the mean life time of this particle in seconds.
  //! hBar is in GeV.s;  width is GeV.
  //! life time is in seconds.
  //! 
  inline double lifeTime() const
  {
  return (_width>0.0) ? (hBar()/_width) : protonLifetime_SI();
  }

  //!
  //! Get the electric charge of this particle.
  //!
  inline double charge() const
  {
  return _charge;
  }

  //!
  //! Set the electric charge of this particle.
  //!
  void setCharge(int value)
  { _charge  = value;}


  //!
  //! Get the net strangeness of this particle.
  //!
  inline int strangeness() const
  {
  return netQuarkNumber(kStrange);
  }

  //!
  //! Get the net charm of this particle.
  //!
  inline double charm() const
  {
  return netQuarkNumber(kCharm);
  }

  //!
  //! Get the net bottomness  of this particle.
  //!
  inline double bottom() const
  {
  return netQuarkNumber(kBottom);
  }

  //!
  //! Get the net topness  of this particle.
  //!
  inline double top() const
  {
  return netQuarkNumber(kBottom);
  }


  //!
  //! Get the lepton number (electron)  of this particle.
  //!
  inline double leptonElectron() const
  {
  return leptonNumber(0);
  }

  //!
  //! Get the lepton number (muon)  of this particle.
  //!
  inline double leptonMuon() const
  {
  return leptonNumber(1);
  }

  //!
  //! Get the lepton number (tau)  of this particle.
  //!
  inline double leptonTau() const
  {
  return leptonNumber(2);;
  }

  //!
  //! Get the number of strange _quarks (s) composing this particle
  //!
  inline int strangeNumber()    const
  { return quarkNumber(kStrange);  }

  //!
  //! Get the number of anti-strange _quarks (s-bar) composing this particle
  //!
  inline int antiStrangeNumber()   const
  { return antiQuarkNumber(kStrange); }

  //!
  //! Get the number of charm _quarks (c) composing this particle
  //!
  inline int charmNumber()    const
  { return quarkNumber(kCharm);  }

  //!
  //! Get the number of anti-charm _quarks (c-bar) composing this particle
  //!
  inline int antiCharmNumber()    const
  { return antiQuarkNumber(kCharm);  }

  //!
  //! Get the number of bottom _quarks (b) composing this particle
  //!
  inline int bottomNumber()    const
  { return quarkNumber(kBottom);  }

  //!
  //! Get the number of anti-bottom _quarks (b-bar) composing this particle
  //!
  inline int antiBottomNumber()    const
  { return antiQuarkNumber(kBottom);  }

  //!
  //! Get the number of top _quarks (b) composing this particle
  //!
  inline int topNumber()    const
  { return quarkNumber(kTop);  }

  //!
  //! Get the number of anti-top _quarks (b-bar) composing this particle
  //!
  inline int antiTopNumber()    const
  { return antiQuarkNumber(kTop);  }

  //!
  //! Get the _spin degeneracy factor of this particle.
  //!
  inline double spinFactor() const
  {
  return 2.0*_spin + 1.0;
  }

  //!
  //! Get the isopsin  of this particle.
  //!
  inline double isospin()   const
  {
  return _isospin;
  }

  //!
  //! Get the 3rd component of the isopsin  of this particle.
  //!
  inline double isospin3()  const
  {
  return _isospin3;
  }

  //!
  //! Get the _isospin degeneracy factor of this particle.
  //!
  inline double isoSpinFactor() const
  {
  return  2.0*_isospin + 1.0;
  }

  //!
  //! Get the statistics of this particle used in the expression of the particle density (+ or -).
  //!
  inline double statistics() const
  {
  return isBoson() ? -1.0 : +1.0;
  }

  inline int antiParticleIndex() const
  {
  return _antiParticleIndex;
  }

  //!
  //! Set the private _index of this particle
  //!
  void setIndex(int value)
  {
  _index = value;
  }

  //!
  //! Set the _name of this particle.
  //!
  void setName(const String & value)
  {
  _name    = value;
  }

  //!
  //! Set the (latex) '_title'' of this particle.
  //!
  void setTitle(const String & value)
  {
  _title   = value;
  }

  //!
  //! Set the _spin, _spin factor, and statistic of this particle.
  //!
  void setSpin(double value)
  {
  _spin = value;
  }

  //!
  //! Set the _isospin  of this particle.
  //!
  void setIsospin(double value)
  {
  _isospin  = value;
  }

  void setIsospin3(double value)
  {
  _isospin3 = value;
  }

  //!
  //! Set the PDG code of this particle.
  //!
  void setPdgCode(int value)
  {
  _pdgCode = value;
  }

  //!
  //! Set the private (user defined) code of this particle.
  //!
  void setPrivateCode(int value)
  {
  _privateCode = value;
  }

  //!
  //! Set the mass of this particle in GeV/c^2.
  //!
  void setMass(double value)
  {
  _mass = value;
  }

  //!
  //! Set the width of this particle in GeV/c^2.
  //!
  void setWidth(double value)
  {
  _width = value;
  }

  //!
  //! Set the width (GeV/c^2) of this particle based on the given value of its lifetime (seconds) .
  //!
  void setLifeTime(double value)
  {
  _width = (value<1.0E37) ? (6.582E-25/value) : 0.0;
  }

  //!
  //! Return true if this particle is a fermion
  //!
  bool isFermion() const
  {
  return fabs(_spin - static_cast<int>(_spin)) > 0.01;
  }

  //!
  //! Return true if this particle is a  boson
  //!
  bool isBoson() const
  {
  return fabs(_spin - static_cast<int>(_spin)) < 0.01;
  }

  //!
  //! Return true if this particle is a photon
  //!
  bool isPhoton() const
  {
  return _pdgCode==22;
  }

  //!
  //! Return true if this particle is a gauge boson
  //!
  bool isGaugeBoson() const
  {
  return _pdgCode==22 || _pdgCode==23  || _pdgCode==24  || _pdgCode==-24;
  }

  //!
  //! Return true if this particle is a Z boson
  //!
  bool isZBoson() const
  {
    return _pdgCode==23;
  }

  //!
  //! Return true if this particle is a W boson
  //!
  bool isWBoson() const
  {
  return _pdgCode==24 || _pdgCode==-24;
  }


  //!
  //! Return true if this particle is a weak boson (Z or W)
  //!
  bool isWeakBoson() const
  {
  return _pdgCode==23  || _pdgCode==24  || _pdgCode==-24;
  }

  //!
  //! Return true if this particle is a Higgs gauge boson
  //!
  bool isHiggsBoson() const
  {
  return _pdgCode==25;
  }

  //!
  //! Return true if this particle is a gluon
  //!
  bool isGluon() const
  {
  return _pdgCode==21;
  }

  //!
  //! Return true if this particle is a lepton
  //!
  bool isLepton() const
  {
  int absPdg = abs(_pdgCode);
  return absPdg>=11  &&  absPdg<=16;
  }

  bool isNeutrino() const
  {
  int code = fabs(_pdgCode);
  return code==12 || code==14 || code==16;
  }

  bool isElectronNeutrino() const
  {
  int code = fabs(_pdgCode);
  return code==12;
  }

  bool isMuonNeutrino() const
  {
  int code = fabs(_pdgCode);
  return code==14 ;
  }

  bool isTauNeutrino() const
  {
  int code = fabs(_pdgCode);
  return code==16;
  }

  //!
  //!Get the electron lepton number
  //!
  int electronLeptonNumber()
  {
  if (_pdgCode == 11 || _pdgCode == 12)
    return 1.0;
  else if (_pdgCode == -11 || _pdgCode == -12)
    return -1.0;
  else
    return 0.0;
  }

  //!
  //!Get the muon lepton number
  //!
  int muonLeptonNumber()
  {
  if (_pdgCode == 13 || _pdgCode == 14)
    return 1;
  else if (_pdgCode == -13 || _pdgCode == -14)
    return -1;
  else
    return 0;
  }

  //!
  //!Get the tau lepton number
  //!
  int tauLeptonNumber()
  {
  if (_pdgCode == 15 || _pdgCode == 16)
    return 1;
  else if (_pdgCode == -15 || _pdgCode == -16)
    return -1;
  else
    return 0;
  }

  //!
  //! Return true if this particle is an electron
  //!
  bool isElectron() const
  {
  return _pdgCode== 11;
  }

  //!
  //! Return true if this particle is a positron
  //!
  bool isPositron() const
  {
  return _pdgCode== -11;
  }

  //!
  //! Return true if this particle is a muon lepton
  //!
  bool isMuon() const
  {
  return _pdgCode == 13 || _pdgCode ==-13 ;
  }

  bool isMuonMinus() const
  {
  return _pdgCode == 13;
  }

  bool isMuonPlus() const
  {
  return _pdgCode == -13;
  }

  //!
  //! Return true if this particle is a  tau lepton
  //!
  bool isTau() const
  {
  return _pdgCode == 15 || _pdgCode == -15;
  }

  bool isTauMinus() const
  {
  return _pdgCode == 15;
  }

  bool isTauPlus() const
  {
  return _pdgCode == -15;
  }

  bool isMeson() const
  {
  int nd = quarkNumber(kDown);
  int nu = quarkNumber(kUp);
  int ns = quarkNumber(kStrange);
  int nc = quarkNumber(kCharm);
  int nb = quarkNumber(kBottom);
  int nt = quarkNumber(kTop);
  int nq  = nd + nu + ns + nc + nb + nt;
  int ndb = antiQuarkNumber(kDown);
  int nub = antiQuarkNumber(kUp);
  int nsb = antiQuarkNumber(kStrange);
  int ncb = antiQuarkNumber(kCharm);
  int nbb = antiQuarkNumber(kBottom);
  int ntb = antiQuarkNumber(kTop);
  int nqb = ndb + nub + nsb + ncb + nbb + ntb;
  return  nq==1  &&  nqb==1;
  }


  //!
  //! Return true if this particle is a  pion
  //!
  bool isPion() const
  {
  return _pdgCode ==211 || _pdgCode==111 || _pdgCode==-211;
  }

  //!
  //! Return true if this particle is a positive pion
  //!
  bool isPionPlus() const
  {
  return _pdgCode ==211;
  }

  //!
  //! Return true if this particle is a negative pion
  //!
  bool isPionMinus() const
  {
  return _pdgCode ==-211;
  }

  //!
  //! Return true if this particle is a neutral pion
  //!
  bool isPionZero() const
  {
  return _pdgCode ==111;
  }

  //!
  //! Return true if this particle is a  kaon
  //!
  bool isKaon() const
  {
    return _pdgCode ==321 || _pdgCode==-321 || _pdgCode==311 || _pdgCode==-311 || _pdgCode==130 || _pdgCode==310;
  }

  //!
  //! Return true if this particle is a positive kaon.
  //!
  bool isKaonPlus() const
  {
  return _pdgCode ==321;
  }

  //!
  //! Return true if this particle is a negative kaon
  //!
  bool isKaonMinus() const
  {
  return _pdgCode ==-321;
  }

  //!
  //! Return true if this particle is kaon (neutral)
  //!
  bool isKaonZero() const
  {
  return _pdgCode ==311;
  }

  //!
  //! Return true if this particle is anti-kaon (neutral)
  //!
  bool isKaonZeroBar() const
  {
    return _pdgCode ==-311;
  }

  //!
  //! Return true if this particle is kaon short.
  //!
  bool isKaon0Short() const
  {
  return _pdgCode ==310;
  }

  //!
  //! Return true if this particle is kaon long.
  //!
  bool isKaon0Long() const
  {
  return _pdgCode ==130;
  }

  //!
  //! Return true if this particle is a _baryon (i.e., has a non zero _baryon number)
  //!
  bool isBaryon() const
  {
  return baryonNumber() != 0;
  }

  //!
  //! Return true if this particle has  _baryon number > 0
  //! This test on the _baryon number - not the charge of the particle
  //!
  bool isBaryonPlus() const
  {
  return baryonNumber()>0;
  }

  //!
  //! Return true if this particle has  _baryon number ><0
  //! This test on the _baryon number - not the charge of the particle
  //!
  bool isBaryonMinus() const
  {
  return baryonNumber()<0;
  }

  //!
  //! Return true if this particle is electrically neutral (zero net charge).
  //!
  bool isCharged() const
  {
  return _charge != 0;
  }

  //!
  //! Return true if this particle is electrically neutral (zero net charge).
  //!
  bool isNeutral() const
  {
    return _charge == 0;
  }

  //!
  //! Return true if this particle is a hadron, i.e., a meson or a _baryon
  //!
  bool isHadron() const
  {
  return isBaryon() || isMeson();
  }


  //!
  //! Return true if this particle is a quark
  //!
  bool isQuark() const
  {
  return _pdgCode==1 || _pdgCode==2 || _pdgCode==3 || _pdgCode==4 || _pdgCode==5 || _pdgCode==6;
  }

  //!
  //! Return true if this particle is an anti- quark
  //!
  bool isAntiQuark() const
  {
  return _pdgCode==-1 || _pdgCode==-2 || _pdgCode==-3 || _pdgCode==-4 || _pdgCode==-5 || _pdgCode==-6;
  }

  //!
  //! Return true if this particle is a proton
  //!
  bool isProton() const
  { return _pdgCode==2212;  }

  //!
  //! Return true if this particle is a neutron
  //!
  bool isNeutron() const
  { return _pdgCode==2112;  }

  //!
  //! Return true if this particle is a nucleon (i.e., either a proton or neutron)
  //!
  bool isNucleon() const
  { return _pdgCode==2212 || _pdgCode==2112;  }

  //!
  //! Return true if this particle is an anti-proton
  //!
  bool isAntiProton() const
  { return _pdgCode==-2212;  }

  //!
  //! Return true if this particle is a anti-neutron
  //!
  bool isAntiNeutron() const
  { return _pdgCode==-2112;  }

  //!
  //! Return true if this particle is an anti-nucleon
  //!
  bool isAntiNucleon() const
  { return _pdgCode==-2212 || _pdgCode==-2112;  }



  //!
  //! Return true if this particle is a Lambda _baryon
  //!
  bool isLambda() const
  { return _pdgCode== 3122;    }

  //!
  //! Return true if this particle is a anti-lambda _baryon
  //!
  bool isAntiLambda() const
  { return _pdgCode==-3122;    }

  //!
  //! Return true if this particle is a Sigma+ _baryon
  //!
  bool isSigmaPlus() const
  { return _pdgCode== 3222;    }

  //!
  //! Return true if this particle is a Sigma0 _baryon
  //!
  bool isSigmaZero() const
  { return _pdgCode== 3212;    }

  //!
  //! Return true if this particle is a Sigma- _baryon
  //!
  bool isSigmaMinus() const
  { return _pdgCode== 3112;    }

  //!
  //! Return true if this particle is a anti-Sigma+ (anti)_baryon
  //!
  bool isAntiSigmaPlus() const
  { return _pdgCode== -3222;   }

  //!
  //! Return true if this particle is a anti-Sigma0 (anti)_baryon
  //!
  bool isAntiSigmaZero() const
  { return _pdgCode== -3212;   }

  //!
  //! Return true if this particle is a anti-Sigma- (anti)_baryon
  //!
  bool isAntiSigmaMinus() const
  { return _pdgCode== -3112;   }

  //!
  //! Return true if this particle is a Xi0 _baryon (Neutral Cascade)
  //!
  bool isXiZero() const
  { return _pdgCode== 3322;    }

  //!
  //! Return true if this particle is a Xi- _baryon (Negative Cascade)
  //!
  bool isXiMinus() const
  { return _pdgCode== 3312;    }

  //!
  //! Return true if this particle is a anti-Xi- _baryon (Neutral anti-cascade)
  //!
  bool isAntiXiZero() const
  { return _pdgCode== -3322;    }

  //!
  //! Return true if this particle is a Xi- _baryon (Negative Cascade)
  //!
  bool isAntiXiMinus() const
  { return _pdgCode== -3312;    }


  //!
  //! Return true if this particle is an Omega- _baryon
  //!
  bool isOmegaMinus() const
  { return _pdgCode== 3334;     }

  //!
  //! Return true if this particle is an anti-Omega- (anti)_baryon
  //!
  inline bool isAntiOmegaMinus() const
  { return _pdgCode== -3334;    }



  //!
  //! Return true if this particle is 'stable'
  //!
  inline bool isStable() const
  {
//    if (_name.Contains("Lambda2100zero"))
//      {
//      printValue("Found ",_name);
//      printValue("isStable() _decayModes.size()",_decayModes.size());
//      printValue("isStable() _decayEnabled",_decayEnabled);
//      }
  return _decayModes.size()<1 || !_decayEnabled;
  }


  //!
  //! Return true if this particle has been declared disabled.
  //!
  inline bool isDisabled() const
  {
  return !_enabled;
  }

  //!
  //! Return true if this particle is "_enabled"
  //!
  inline bool isEnabled() const
  {
  return _enabled;
  }

  //!
  //! Set the state of this particle as  "_enabled"
  //!
  inline void enable()
  {
  _enabled = true;
  }

  //!
  //! Set the state of this particle as  "disabled"
  //!
  inline void disable()
  {
  _enabled = false;
  }

  //!
  //! Return true if this particle is "decay disabled"
  //!
  inline  bool isDecayDisabled() const
  {
  return !_decayEnabled;
  }

  //!
  //! Return true if this particle is "decay enabled"
  //!
  inline bool isDecayEnabled() const
  {
  return _decayEnabled;
  }

  //!
  //! Set the state of this particle as  "_enabled"
  //!
  inline void enableDecay()
  {
  _decayEnabled = true;
//  printValue("enableDecay() _decayModes.size()",_decayModes.size());
//  if (_decayModes.size()>0) _decayEnabled = true;
//  printValue("_decayEnabled)",_decayEnabled);
  }

  //!
  //! Set the state of this particle as  "disabled"
  //!
  void disableDecay()
  {
  _decayEnabled = false;
  }

  //!
  //! Return the number of distinct decay modes of this particle
  //!
  int nDecayModes() const
  {
  return _decayModes.size();
  }

  //!
  //! Return true if this particle has known decay modes
  //!
  bool  hasDecayModes() const
  {
  return _decayModes.size()>0;
  }

  ParticleDecayMode & getDecayMode(unsigned int _index)
  {
  if (_index >= _decayModes.size()) throw Exception("_index >= _decayModes.size()",__FUNCTION__);
  return _decayModes[_index];
  }

  void  setAntiParticleIndex(unsigned int value)
  {
  _antiParticleIndex = value;
  }

  //!
  //! Get atomic number
  //!
  double getZ() const
  {
  return atomicNumber();
  }

  //!
  //! Get atomic number
  //! Must contain at least one proton or antiproton
  //!
  double atomicNumber() const
  {
  if (isBaryon())
    return fabs(_charge);
  else
    return 0;
  }

  //!
  //! Get mass number
  //!  Must contain at least one proton or antiproton
  //!
  double massNumber() const
  {
  return fabs(baryonNumber());
  }

  //!
  //! Get neutron number
  //!
  double neutronNumber() const
  {
  double z = atomicNumber();
  double a = massNumber();
  return a-z;
  }

  double beta() const
  {
  return _beta;
  }

  double deformationBeta() const
  {
  return _beta;
  }

  void setBeta(double _deformationBeta)
  {
  _beta = _deformationBeta;
  }

  void setDeformationBeta(double _deformationBeta)
  {
  _beta = _deformationBeta;
  }

  double bindingEnergy() const;

  void addDecayMode(double branchingRatio, std::vector<int> children);
  void addDecayMode(ParticleDecayMode & decayMode);

  ParticleDecayMode & generateDecayMode();

  //!
  //!Return an array of decay modes for this particle
  //!
  const std::vector<ParticleDecayMode> & decayModes() const
  {
  return _decayModes;
  }

  std::vector<ParticleDecayMode> & decayModes()
  {
  return _decayModes;
  }

  void setZ(int value)
  {
  _charge = value;
  }

  void setAtomicNumber(double value)
  {
  _charge = value;
  }

  void setA(int value)
  {
  _baryon = value;
  }

  void setMassNumber(int value)
  {
  _baryon = value;
  }


  void print();
  void printDecays();

  static  ParticleType * createNucleus(const String & name);
  static  ParticleType * protonType;      //!< Pointer to a singleton object coding a proton type
  static  ParticleType * neutronType;     //!< Pointer to a singleton object coding a neutron type
  static  ParticleType * deuteronType;    //!< Pointer to a singleton object coding a deuteron type
  static  ParticleType * tritonType;      //!< Pointer to a singleton object coding a triton type
  static  ParticleType * he3Type;         //!< Pointer to a singleton object coding a 3He GlauberNucleus type
  static  ParticleType * he4Type;         //!< Pointer to a singleton object coding a 4He GlauberNucleus type
  static  ParticleType * auType;          //!< Pointer to a singleton object coding an 197Au (gold) GlauberNucleus type
  static  ParticleType * pbType;          //!< Pointer to a singleton object coding a 208Pb  (lead) GlauberNucleus type

  static  ParticleType * antiProtonType;      //!< Pointer to a singleton object coding a proton type
  static  ParticleType * antiNeutronType;     //!< Pointer to a singleton object coding a neutron type
  static  ParticleType * antiDeuteronType;    //!< Pointer to a singleton object coding a deuteron type
  static  ParticleType * antiTritonType;      //!< Pointer to a singleton object coding a triton type
  static  ParticleType * antiHe3Type;         //!< Pointer to a singleton object coding a 3He GlauberNucleus type
  static  ParticleType * antiHe4Type;

  ClassDef(ParticleType,0)
};

}
#endif  

