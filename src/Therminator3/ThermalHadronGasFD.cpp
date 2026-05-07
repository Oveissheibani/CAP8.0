#include "ThermalHadronGasFD.hpp"
#include "ParticleDecayMode.hpp"
#include "PrintHelpers.hpp"
#include "NameHelpers.hpp"

ClassImp(CAP::ThermalHadronGas);

using namespace std;

namespace CAP
{

  ThermalHadronGasFD::ThermalHadronGasFD()
  :
  ThermalHadronGas(),
  _stableHadrons(),
  _stableHadronPairs()
  {  }

  ThermalHadronGasFD::ThermalHadronGasFD(const ThermalHadronGasFD & source)
  :
  ThermalHadronGas(source),
  _stableHadrons(source._stableHadrons),
  _stableHadronPairs(source._stableHadronPairs)
  {  }

  ThermalHadronGasFD & ThermalHadronGasFD::operator=(const ThermalHadronGasFD & rhs)
  {
  if (this!=&rhs)
    {
    ThermalHadronGas::operator=(rhs);
    _stableHadrons     = rhs._stableHadrons;
    _stableHadronPairs = rhs._stableHadronPairs;
    }
  return *this;
  }

  void ThermalHadronGasFD::reset()
  {
    ThermalHadronGas::reset();
    for (auto & value : _stableHadronPairs) value = 0.0;
  }

  void ThermalHadronGasFD::clear()
  {
  ThermalHadronGas::clear();
  _stableHadronPairs.clear();
  }

  void ThermalHadronGasFD::calculateParticleDecayProbability()
  {
  _stableHadrons.clear();
  // find all _stable hadrons and load _stable hadron array
  for (auto & hadron : _hadrons)
    {
    if (hadron.type().isStable())
      {
      printValue("calculateParticleDecayProbability() _stable",hadron.type().name());
      _stableHadrons.push_back(&hadron);
      }
    }
  unsigned int nStableHadrons = _stableHadrons.size();
  if (nStableHadrons<1) throw Exception("_stableHadrons.size() < 1",__FUNCTION__);
  printValue("nStableHadrons",nStableHadrons);
  // create list of probability for each hadron and accumulate probabilities
  // to _stable hadrons
  for (auto & hadron : _hadrons)
    {
    hadron.createDecayProbabilityArray(nStableHadrons);
    int iStableHadron = findStableHadronIndexFor(hadron.type().pdgCode());
    if (iStableHadron>=0) hadron.setDecayProbability(iStableHadron, 1.0);
    }

  // particles have been sorted in ascending order of _mass
  // calculate  the probabilities
  // singles
  for (auto & hadron : _hadrons)
    {
    if (hadron.type().isStable()) continue;
    vector<double> temp(nStableHadrons,0.0);
    std::vector<ParticleDecayMode> & decayModes = hadron.type().decayModes();
    for (auto & decayMode : decayModes)
      {
      const vector<int> & childrenCodes = decayMode.childrenPdgCodes();
      for (auto & childCode : childrenCodes)
        {
        if (childCode == 22) continue; // dont care about photons
        ThermalHadron & childHadron = findHadronFor(childCode);
        for (unsigned int mStableHadron = 0; mStableHadron < nStableHadrons; mStableHadron++)
          {
          temp[mStableHadron] += (decayMode.branchingRatio()*childHadron.decayProbability(mStableHadron));
          }
        }
      }
    for (unsigned int iStableHadron1=0; iStableHadron1<nStableHadrons; iStableHadron1++)
      {
      hadron.setDecayProbability(iStableHadron1, temp[iStableHadron1]);
      }
    }

  // pairs
  for (auto & hadron : _hadrons)
    {
    if (hadron.type().isStable()) continue;

    //printValue("(Pairs) Hadron name",hadron.type().name());

    vector<double> tempPair(nStableHadrons*nStableHadrons,0.0);
    std::vector<ParticleDecayMode> & decayModes = hadron.type().decayModes();

    //printValue("-0- hadron name",hadron.type().name());


    for (auto & decayMode : decayModes)
      {
      const vector<int> & childrenCodes = decayMode.childrenPdgCodes();
      for (auto & childCode : childrenCodes)
        {
        if (childCode == 22) continue; // dont care about photons
        ThermalHadron & childHadron = findHadronFor(childCode);
        // printValue("(Pairs-1) Child Hadron name",childHadron.type().name());
        //printValue("-1- hadron _type",mStableHadron1);

        // pair decays from this child
        for (unsigned int mStableHadron1 = 0; mStableHadron1 < nStableHadrons; mStableHadron1++)
          {
          for (unsigned int mStableHadron2 = 0; mStableHadron2 < nStableHadrons; mStableHadron2++)
            {
            double prob = decayMode.branchingRatio()*childHadron.decayProbabilityPair(mStableHadron1,mStableHadron2);
            if (prob>0)
              {
              //              printValue("-1- mStableHadron1",mStableHadron1);
              //              printValue("-1- mStableHadron2",mStableHadron2);
              //              printValue("-1- decayMode.branchingRatio()",decayMode.branchingRatio());
              //              printValue("-1- childHadron.decayProbabilityPair(mStableHadron1,mStableHadron2)",childHadron.decayProbabilityPair(mStableHadron1,mStableHadron2));
              tempPair[mStableHadron1*nStableHadrons+mStableHadron2] += prob;
              }
            }
          }
        }
      // pairs from mixed children
      for (unsigned int iChild1=0; iChild1<childrenCodes.size(); iChild1++)
        {
        int childCode1 = childrenCodes[iChild1];
        if (childCode1 == 22) continue; // dont care about photons
        ThermalHadron & childHadron1 = findHadronFor(childCode1);
        //printValue("(Pairs-2) Child 1 Hadron name",childHadron1.type().name());
        for (unsigned int iChild2=iChild1+1; iChild2<childrenCodes.size(); iChild2++)
          {
          int childCode2 = childrenCodes[iChild2];
          if (childCode2 == 22) continue; // dont care about photons
          ThermalHadron & childHadron2 = findHadronFor(childCode2);
          //printValue("(Pairs-2) Child 2 Hadron name",childHadron2.type().name());
          for (unsigned int mStableHadron1 = 0; mStableHadron1 < nStableHadrons; mStableHadron1++)
            {
            for (unsigned int mStableHadron2 = 0; mStableHadron2 < nStableHadrons; mStableHadron2++)
              {
              double prob = decayMode.branchingRatio()*childHadron1.decayProbability(mStableHadron1)*childHadron2.decayProbability(mStableHadron2);
              if (prob>0)
                {
                //                printValue("-2- mStableHadron1",mStableHadron1);
                //                printValue("-2- mStableHadron2",mStableHadron2);
                //                printValue("-2- decayMode.branchingRatio()",decayMode.branchingRatio());
                //                printValue("-2- childHadron1.decayProbability(mStableHadron1)",childHadron1.decayProbability(mStableHadron1));
                //                printValue("-2- childHadron2.decayProbability(mStableHadron2)",childHadron2.decayProbability(mStableHadron2));
                tempPair[mStableHadron1*nStableHadrons+mStableHadron2] += prob;
                }
              }
            }
          }
        }
      }
    for (unsigned int iStableHadron1=0; iStableHadron1<nStableHadrons; iStableHadron1++)
      {
      for (unsigned int iStableHadron2=0; iStableHadron2<nStableHadrons; iStableHadron2++)
        {
        double prob = tempPair[iStableHadron1*nStableHadrons+iStableHadron2];
        //        printValue("iStableHadron1",iStableHadron1);
        //        printValue("iStableHadron2",iStableHadron2);
        //        printValue("prob",prob);
        hadron.setDecayProbabilityPair(iStableHadron1,iStableHadron2,prob );
        }
      }
    }

  }

  //!
  //!Compute the densities of _stable hadrons based on the thermal yields of more
  //!massive hadrons and their respective decay probabilities into the give _stable hadrons
  //!
  void ThermalHadronGasFD::calculateStableDensities()
  {
  int nStables = _stableHadrons.size();
  // singles
  for (int iStable = 0; iStable < nStables; iStable++)
    {
    double _stableDensity = 0.0;
    for (auto hadron : _hadrons)
      {
      _stableDensity += hadron.decayProbability(iStable) * hadron.numberDensity();
      }
    _stableHadrons[iStable]->setFeedDownNumberDensity(_stableDensity);
    }

  // pairs
  _stableHadronPairs.clear();
  for (int iStable1 = 0; iStable1 < nStables; iStable1++)
    {
    for (int iStable2 = 0; iStable2 < nStables; iStable2++)
      {
      double _stableDensity = 0.0;
      for (auto & hadron : _hadrons)
        {
        _stableDensity += hadron.decayProbabilityPair(iStable1,iStable2) * hadron.numberDensity();
        }
      _stableHadronPairs.push_back(_stableDensity);
      }
    }

  // symmetrize
  for (int k1 = 0; k1 < nStables; k1++)
    {
    for (int k2 = 0; k2 < k1; k2++)
      {
      double v1 = _stableHadronPairs[k1*nStables+k2];
      double v2 = _stableHadronPairs[k2*nStables+k1];
      double v = v1+v2;
      //std::cout << "k1,k2=" << k1 << "," << k2 << "   " << v1 << "," << v2 << " === " << v << endl;
      _stableHadronPairs[k1*nStables+k2] =v;
      _stableHadronPairs[k2*nStables+k1] =v;
      }
    }
  }

  // this needs to be "fixed"
  void ThermalHadronGasFD::setupDecayGenerator()
  {
  //hadronRndmSelector = new SelectionGenerator(particleDensities);
  }

  // this needs to be "fixed"
  ParticleType * ThermalHadronGasFD::generateRandomHadron()
  {
  //int index = 0; // hadronRndmSelector->generate();
  return nullptr;// _particleTypes->particleType(index);
  }

  // this needs to be "fixed"
  int ThermalHadronGasFD::generateRandomHadronByIndex()
  {
  return 0; // hadronRndmSelector->generate();
  }


  void ThermalHadronGasFD::print() const
  {
    // fix me...
  }

  void ThermalHadronGasFD::findStableHadrons()
  {
  _stableHadrons.clear();
  for (auto & hadron : _hadrons)
    {
    printValue("name",hadron.type().name());
    printValue("stable",hadron.type().isStable());
    if (hadron.type().isStable())
      _stableHadrons.push_back(&hadron);
    }
  }

  ThermalHadron & ThermalHadronGasFD::findStableHadronFor(int pdgCode)
  {
  for (unsigned int iHadron=0; iHadron<_stableHadrons.size(); iHadron++)
    {
    if (_stableHadrons[iHadron]->type().pdgCode() == pdgCode) return *(_stableHadrons[iHadron]);
    }
  throw Exception("Unknown PDG Code",__FUNCTION__);
  }

  int ThermalHadronGasFD::findStableHadronIndexFor(int pdgCode)
  {
  for (unsigned int iHadron=0; iHadron<_stableHadrons.size(); iHadron++)
    {
    if (_stableHadrons[iHadron]->type().pdgCode() == pdgCode)
      {
      //      printValue("findStableHadronIndexFor() pdgCode",pdgCode);
      //      printValue("findStableHadronIndexFor() iHadron",iHadron);
      return iHadron;
      }
    }
  return -1; //not found
  }

  void ThermalHadronGasFD::calculateAllProperties()
  {
  ThermalHadronGas::calculateAllProperties();
  calculateParticleDecayProbability();
  calculateStableDensities();
  }

}   // namespace CAP
