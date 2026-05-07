/* **********************************************************************
 * Copyright (C) 2019-2024, Claude Pruneau, Victor Gonzalez
 * Copyright (C) 2016, Chun Shen
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   04/01/2024
 *
 * *********************************************************************/
#include "ParticleType.hpp"
#include "SelectionGenerator.hpp"
#include "PhysicsConstants.hpp"
#include "PrintHelpers.hpp"

ClassImp(CAP::ParticleType);

namespace CAP
{


  //!
  //! Default CTOR
  //!
  ParticleType::ParticleType()
  :
  _index(0),
  _name(""),
  _title(""),
  _privateCode(0),
  _pdgCode(0),
  _mass(0.0),
  _massUncertainty(0.0),
  _charge(0.0),
  _width(0.0),
  _widthUncertainty(0.0),
  _spin(0.0),
  _isospin(0.0),
  _isospin3(0.0),
  _quarks(),
  _antiquarks(),
  _leptons(),
  _baryon(0),
  _enabled(1),
  _decayEnabled(0),
  _antiParticleIndex(-1),
  _decayModes(),
  _decayRndmSelector(),
  _beta(0.0)
  {
  _decayModes.clear();
  }

  //!
  //! DTOR
  //!
  ParticleType::~ParticleType()
  {   }

  //!
  //! COPY CTOR
  //!
  ParticleType::ParticleType(const ParticleType & source)
  :
  _index(source._index),
  _name(source._name),
  _title(source._title),
  _privateCode(source._privateCode),
  _pdgCode(source._pdgCode),
  _mass(source._mass),
  _massUncertainty(source._massUncertainty),
  _charge(source._charge),
  _width(source._width),
  _widthUncertainty(source._widthUncertainty),
  _spin(source._spin),
  _isospin(source._isospin),
  _isospin3(source._isospin3),
  _quarks(source._quarks),
  _antiquarks(source._antiquarks),
  _leptons(source._leptons),
  _baryon(source._baryon),
  _enabled(source._enabled),
  _decayEnabled(source._decayEnabled),
  _antiParticleIndex(-1),
  _decayModes(source._decayModes),
  _decayRndmSelector(source._decayRndmSelector),
  _beta(source._beta)
  {
  // no ops
  }

  //!
  //! Assignment operator. Copies all the properties of the source particle into this.
  //!
  ParticleType & ParticleType::operator=(const ParticleType & rhs)
  {
  if (this!=&rhs)
    {
    _index              = rhs._index;
    _name               = rhs._name;
    _title              = rhs._title;
    _privateCode        = rhs._privateCode;
    _pdgCode            = rhs._pdgCode;
    _mass               = rhs._mass;
    _massUncertainty    = rhs._massUncertainty;
    _charge             = rhs._charge;
    _width              = rhs._width;
    _widthUncertainty   = rhs._widthUncertainty;
    _spin               = rhs._spin;
    _isospin            = rhs._isospin;
    _isospin3           = rhs._isospin3;
    _quarks             = rhs._quarks;
    _antiquarks         = rhs._antiquarks;
    _leptons            = rhs._leptons;
    _enabled            = rhs._enabled;
    _decayEnabled       = rhs._decayEnabled;
    _antiParticleIndex  = rhs._antiParticleIndex;
    _decayModes         = rhs._decayModes;
    _decayRndmSelector  = rhs._decayRndmSelector;
    _beta               = rhs._beta;
    }
  return *this;
  }

  //!
  //! Add a decay mode to this particle type based on the given branching fraction (branching ratio) and the given array of children
  //! @param branchingRatio : fraction of all decays of this particle type into the given children
  //! @param children : array of particle type _index corresponding to the particle type this particle decays into.
  //!
  void ParticleType::addDecayMode(double branchingRatio,
                                  std::vector<int> children)
  {
  if (branchingRatio<0.0  || branchingRatio>1.0)
    {
    throw Exception("branchingRatio<0.0||branchingRatio>1.0",__FUNCTION__);
    }
  if (children.size()<2)
    {
    throw Exception("Decay mode with no or only one particle",__FUNCTION__);
    }
  ParticleDecayMode decayMode;
  decayMode.setBranchingRatio(branchingRatio);
  for (int k=0; k<int(children.size()); k++) decayMode.addChild(children[k]);
  addDecayMode(decayMode);
  }

  //!
  //! Add a decay mode to this particle type based on the given decayModel object
  //! @param decayMode : decay mode object encapsulating the branching fraction and the list of particle types this particle decays into.
  //!
  //!  When a decay is added to a particle type, that type is automatically
  //!  considered unstable and decay _enabled. Both flags can however be altered
  //!  as needed.
  //!
  void ParticleType::addDecayMode(ParticleDecayMode & decayMode)
  {
  _decayModes.push_back(decayMode);
  _decayEnabled = true;
  }

  void ParticleType::setupDecayGenerator()
  {
  int nDecayModes = _decayModes.size();
  if (nDecayModes<1)
    throw Exception("nDecayModes<1",__FUNCTION__);
  vector<double> decayBranchingRatios;
  for (int k=0; k<nDecayModes; k++)
    {
    decayBranchingRatios.push_back(_decayModes[k].branchingRatio());
    }
  _decayRndmSelector.initializeWith(decayBranchingRatios);
  }

  //!
  //! Generate decay mode of  this particle
  //!
  ParticleDecayMode & ParticleType::generateDecayMode()
  {
  int _index = _decayRndmSelector.generate();
  if (_index<0 || _index>= int(_decayModes.size()))
    {
    printString("ParticleType::generateDecayMode() _index<0 ");
    printValue("Bad index",_index);
    printValue("particle _name",_name);
    printValue("_decayEnabled",_decayEnabled);
    printValue("_width",_width);
    printValue("n modes",_decayModes.size());
    throw Exception("Bad index",__FUNCTION__);
    }
  return _decayModes[_index];
  }


  //!
  //! Print the properties of this particle on the given stream and return a reference to that stream.
  //!
  void ParticleType::print()
    {
    unsigned int size = gOutputLength;
    std::ostream & os = *gOutputStream;
    unsigned int style = 0;
    switch (style)
      {
        case 0:
        {
        //scientific
        os <<  fixed << setw(10) << setprecision(8) << _pdgCode;         // Monte-Carlo number according PDG
        os <<  setw(size) << _name;   // ParticleType _name
        os <<  setw(size) << _title;   // ParticleType _title
        os <<  scientific << setw(size) << setprecision(4) <<  _mass;
        os <<  scientific << setw(size) << setprecision(4) <<  _width;
        os <<  fixed << setw(9) << setprecision(2) <<  _charge;
        os <<  fixed << setw(9) << setprecision(2) <<  _spin;
        os <<  fixed << setw(9) << setprecision(2) <<  _isospin;
        os <<  fixed << setw(9) << setprecision(2) <<  _isospin3;
        os <<  fixed << setw(8) << setprecision(6) <<  _quarks;
        os <<  fixed << setw(8) << setprecision(6) <<  _antiquarks;
        os <<  fixed << setw(8) << setprecision(6) <<  _leptons;
        os <<  fixed << setw(5) << setprecision(1) <<  _enabled;
        os <<  fixed << setw(5) << setprecision(1) <<  _decayEnabled;
        os <<  endl;
        for (int k=0;k<int(_decayModes.size());k++)
          {
          _decayModes[k].print();
          }
        break;
        }
        case 1:
        {
        printCR();
        printValue("Name",_name);
        printValue("Title",_title);   // ParticleType _title
        printValue("pdgCode",_pdgCode);         // Monte-Carlo number according PDG
        printValue("Mass",_mass);
        printValue("MassUncertainty",_massUncertainty);
        printValue("Width",_width);
        printValue("WidthUncertainty",_widthUncertainty);
        printValue("Charge",_charge);
        printValue("Spin",_spin);
        printValue("_decayEnabled",_decayEnabled);
        printValue("_enabled",_enabled);
        if (isHadron() || isQuark())
          {
          printValue("Isospin",_isospin);
          printValue("Isospin3",_isospin3);
          printValue("d",quarkNumber(0));
          printValue("u",quarkNumber(1));
          printValue("s",quarkNumber(2));
          printValue("c",quarkNumber(3));
          printValue("b",quarkNumber(4));
          printValue("t",quarkNumber(5));
          printValue("d-bar",antiQuarkNumber(0));
          printValue("u-bar",antiQuarkNumber(1));
          printValue("s-bar",antiQuarkNumber(2));
          printValue("c-bar",antiQuarkNumber(3));
          printValue("b-bar",antiQuarkNumber(4));
          printValue("t-bar",antiQuarkNumber(5));
          }
        if (isLepton())
          {
          printValue("ElectronLeptonNumber",electronLeptonNumber());
          printValue("leMuonLeptonNumberptonMuon",muonLeptonNumber());
          printValue("TauLeptonNumber",tauLeptonNumber());
          }
         printLine();
        break;
        }
      }
    }

  //!
  //! Print the decay properties of this particle on the given stream and return a reference to that stream.
  //!
  void ParticleType::printDecays()
    {
    std::ostream & os = *gOutputStream;
    unsigned int size = gOutputLength;
    unsigned int style = 0;
    switch (style)
      {
        case 0:
        {
        os <<  setw(size) << _name;   // ParticleType _name
        os <<  setw(size) << _title;   // ParticleType _title
        os <<  scientific << setw(size) << setprecision(4) <<  _mass;
        os <<  scientific << setw(size) << setprecision(4) <<  _width;
        os <<  fixed << setw(size) << setprecision(2) <<  _charge;
        os <<  fixed << setw(size) << setprecision(2) <<  _spin;
        unsigned int nModes = _decayModes.size();
        for (unsigned int k=0;k<nModes;k++)
          {
          if (_pdgCode != _decayModes[0].childPdgCode(0))
            {
            os << "      " << k << " Br:" << scientific << setw(size) << setprecision(4) << (100.0*_decayModes[k].branchingRatio()) << " to ";
            unsigned int nChildren = _decayModes[k].nChildren();
            for (unsigned int iChild=0; iChild<nChildren; iChild++)
              {
              ParticleType & type = _decayModes[k].childType(iChild);
              os << type.title();
              if (iChild < nChildren-1)  os << "+";
              }
            os << endl;
            }
          }
        }
        case 1:
        {
        ///
        }
      }
    }

  //!
  //! Get the binding  of this GlauberNucleus  in GeV/c^2.
  //!
  double ParticleType::bindingEnergy() const
    {
    double z = fabs(_charge);
    double a = massNumber();
    double bindingE = _mass - fabs(z*Physics::protonMass() + (a-z)*Physics::neutronMass());
    return bindingE;
    }

  ParticleType * ParticleType::createNucleus(const String & _name)
    {
    ParticleType * type = new ParticleType;
    if (_name.EqualTo("Proton"))
      {
      type->setName("Proton");
      type->setTitle("p");
      type->setAtomicNumber(1);
      type->setMassNumber(1);
      type->setMass(Physics::protonMass() );
      type->setSpin(0.5);
      type->setLifeTime(Physics::protonLifetime_SI());
      type->setBeta(0.0);

      }
    else if (_name.EqualTo("Neutron"))
      {
      type->setName("Neutron");
      type->setTitle("n");
      type->setAtomicNumber(0);
      type->setMassNumber(1);
      type->setMass(Physics::neutronMass() );
      type->setSpin(0.5);
      type->setLifeTime(887.7); // seconds
      type->setBeta(0.0);

      }
    else if (_name.EqualTo("Deuteron"))
      {
      type->setName("Deuteron");
      type->setTitle("d");
      type->setAtomicNumber(1);
      type->setMassNumber(2);
      type->setMass(Physics::deuteronMass() );
      type->setSpin(1.0);
      type->setLifeTime(Physics::protonLifetime_SI()); // seconds
      type->setBeta(0.0);
      }
    else if (_name.EqualTo("Triton"))
      {
      type->setName("Triton");
      type->setTitle("t");
      type->setAtomicNumber(1);
      type->setMassNumber(3);
      type->setMass(Physics::tritonMass() );
      type->setSpin(0.5);
      type->setLifeTime(12.32*Physics::yearToSecond()); // seconds
      type->setBeta(0.0);
      }
    else if (_name.EqualTo("Helium3"))
      {
      type->setName("Helium3");
      type->setTitle("3He");
      type->setAtomicNumber(2);
      type->setMassNumber(3);
      type->setMass(Physics::helium3Mass() );
      type->setSpin(0.5);
      type->setLifeTime(Physics::protonLifetime_SI()); // seconds
      type->setBeta(0.0);
      }
    else if (_name.EqualTo("Helium4"))
      {
      type->setName("Helium4");
      type->setTitle("4He");
      type->setAtomicNumber(2);
      type->setMassNumber(4);
      type->setMass(Physics::helium4Mass() );
      type->setSpin(0.0);
      type->setLifeTime(Physics::protonLifetime_SI()); // seconds
      type->setBeta(0.0);
      }
    else if (_name.EqualTo("Gold197"))
      {
      type->setName("Gold197");
      type->setTitle("197Au");
      type->setAtomicNumber(79);
      type->setMassNumber(100);
      type->setMass(Physics::au197Mass() );
      type->setSpin(1.5);
      type->setLifeTime(Physics::protonLifetime_SI()); // seconds
      type->setBeta(0.0);
      }
    else if (_name.EqualTo("Lead208"))
      {
      type->setName("Lead208");
      type->setTitle("208Pb");
      type->setAtomicNumber(82);
      type->setMassNumber(126);
      type->setMass(Physics::pb208Mass() );
      type->setSpin(1.5);
      type->setLifeTime(Physics::protonLifetime_SI()); // seconds
      type->setBeta(0.0);
      }
    if (_name.EqualTo("AntiProton"))
      {
      type->setName("AntiProton");
      type->setTitle("#bar{p}");
      type->setAtomicNumber(-1);
      type->setMassNumber(-1);
      type->setMass(Physics::protonMass() );
      type->setSpin(0.5);
      type->setLifeTime(1.0E34);
      type->setBeta(0.0);

      }
    else if (_name.EqualTo("AntiNeutron"))
      {
      type->setName("AntiNeutron");
      type->setTitle("#bar{n}");
      type->setAtomicNumber(0);
      type->setMassNumber(-1);
      type->setMass(Physics::neutronMass() );
      type->setSpin(0.5);
      type->setLifeTime(887.7); // seconds
      type->setBeta(0.0);

      }
    else if (_name.EqualTo("AntiDeuteron"))
      {
      type->setName("AntiDeuteron");
      type->setTitle("#bar{d}");
      type->setAtomicNumber(-1);
      type->setMassNumber(-2);
      type->setMass(Physics::deuteronMass() );
      type->setSpin(1.0);
      type->setLifeTime(Physics::protonLifetime_SI()); // seconds
      type->setBeta(0.0);
      }
    else if (_name.EqualTo("AntiTriton"))
      {
      type->setName("AntiTriton");
      type->setTitle("#bar{t}");
      type->setAtomicNumber(-1);
      type->setMassNumber(-3);
      type->setMass(Physics::tritonMass() );
      type->setSpin(0.5);
      type->setLifeTime(12.32*Physics::yearToSecond()); // seconds
      type->setBeta(0.0);
      }
    else if (_name.EqualTo("AntiHelium3"))
      {
      type->setName("AntiHelium3");
      type->setTitle("3#bar{He}");
      type->setAtomicNumber(-2);
      type->setMassNumber(-3);
      type->setMass(Physics::helium3Mass() );
      type->setSpin(0.5);
      type->setLifeTime(Physics::protonLifetime_SI()); // seconds
      type->setBeta(0.0);
      }
    else if (_name.EqualTo("AntiHelium4"))
      {
      type->setName("AntiHelium4");
      type->setTitle("4#bar{He}");
      type->setAtomicNumber(-2);
      type->setMassNumber(-4);
      type->setMass(Physics::helium4Mass() );
      type->setSpin(0.0);
      type->setLifeTime(Physics::protonLifetime_SI()); // seconds
      type->setBeta(0.0);
      }
    return type;
    }

  ParticleType * ParticleType::protonType   = ParticleType::createNucleus("Proton");
  ParticleType * ParticleType::neutronType  = ParticleType::createNucleus("Neutron");
  ParticleType * ParticleType::deuteronType = ParticleType::createNucleus("Deuteron");
  ParticleType * ParticleType::tritonType   = ParticleType::createNucleus("Triton");
  ParticleType * ParticleType::he3Type      = ParticleType::createNucleus("Helium3");
  ParticleType * ParticleType::he4Type      = ParticleType::createNucleus("Helium4");
  ParticleType * ParticleType::auType       = ParticleType::createNucleus("Gold");
  ParticleType * ParticleType::pbType       = ParticleType::createNucleus("Lead");

  ParticleType * ParticleType::antiProtonType   = ParticleType::createNucleus("AntiProton");
  ParticleType * ParticleType::antiNeutronType  = ParticleType::createNucleus("AntiNeutron");
  ParticleType * ParticleType::antiDeuteronType = ParticleType::createNucleus("AntiDeuteron");
  ParticleType * ParticleType::antiTritonType   = ParticleType::createNucleus("AntiTriton");
  ParticleType * ParticleType::antiHe3Type      = ParticleType::createNucleus("AntiHelium3");
  ParticleType * ParticleType::antiHe4Type      = ParticleType::createNucleus("AntiHelium4");

  } // namespace CAP
