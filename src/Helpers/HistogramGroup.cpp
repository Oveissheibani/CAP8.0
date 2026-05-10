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
//#include "TSystem.h"
#include "TROOT.h"
#include "TKey.h"
#include "TSystem.h"
#include "HistogramGroup.hpp"
#include "Task.hpp"

ClassImp(CAP::Histo  gramGroup);

namespace CAP
{
  
  HistogramGroup::HistogramGroup()
  :
  Object(),
  _histogramBaseName(),
  _parentTask(nullptr),
  _objects()
  {
  setClassName("HistogramGroup");
  setName("HistogramGroup");
  setTitle("HistogramGroup");
  }
  
  HistogramGroup::HistogramGroup(const HistogramGroup & source)
  :
  Object(source),
  _histogramBaseName(source._histogramBaseName),
  _parentTask(source._parentTask),
  _objects()
  {  }
  
  HistogramGroup & HistogramGroup::operator=(const HistogramGroup & rhs)
  {
  if (this!=&rhs)
    {
    Object::operator=(rhs);
    _histogramBaseName = rhs._histogramBaseName;
    _parentTask = rhs._parentTask;
    }
  return *this;
  }
  
  void HistogramGroup::configure(const String & taskName,
                                 const String & objectType,
                                 const Configuration & configuration,
                                 unsigned int index)
  {
  if (reportDebug(__FUNCTION__)) { printCR(); }
  _histogramBaseName  = configuration.valueString(createKey(taskName,objectType,index,"BASE_NAME"));
  }
  
  const String HistogramGroup::parentName() const
  {
  if (!_parentTask) throw Exception("!_parentTask",__FUNCTION__);
  return _parentTask->name();
  }
  
  const String HistogramGroup::parentPathName() const
  {
  if (!_parentTask) throw Exception("!_parentTask",__FUNCTION__);
  return _parentTask->fullTaskPath();
  }
  
  void HistogramGroup::saveTo(TFile & file)
  {
  if (reportDebug(__FUNCTION__)) { printCR(); }
  file.cd();
  // See sibling HistogramGroup in src/Particles for rationale.
  for (auto & object  : _objects)
    {
    if (!object) continue;
    object->SetDirectory(&file);
    object->Write();
    }
  }
  
  void HistogramGroup::scaleHistograms(double a)
  {
  if (reportDebug(__FUNCTION__)) { printCR(); }
  for (auto & object  : _objects)
    {
    if ((object ->IsA()==TProfile::Class()) ||
        (object ->IsA()==TProfile2D::Class()) ) continue;
    object ->Scale(a);
    }
  }
  
  HistogramGroup & HistogramGroup::operator+=(const HistogramGroup & rhs)
  {
  if (!sameSizeAs(rhs))
    {
    printIncompatibleLists("lhs and rhs have different histograms",*this,rhs);
    throw IncompatibleHistogramGroupException("lhs and rhs have different histograms",__FUNCTION__);
    }
  const std::vector<TH1*> & rhsHistos = rhs.histograms();
  for (unsigned int k=0; k<_objects.size(); k++) _objects[k]->Add(rhsHistos[k],1.0);
  return *this;
  }
  
  HistogramGroup & HistogramGroup::operator-=(const HistogramGroup & rhs)
  {
  if (!sameSizeAs(rhs))
    {
    printIncompatibleLists("lhs and rhs have different histograms",*this,rhs);
    throw IncompatibleHistogramGroupException("lhs and rhs have different histograms",__FUNCTION__);
    }
  const std::vector<TH1*> & rhsHistos = rhs.histograms();
  for (unsigned int k=0; k<_objects.size(); k++) _objects[k]->Add(rhsHistos[k],-1.0);
  return *this;
  }
  
  
  void HistogramGroup::sumHistogramGroups(double a1, const HistogramGroup & group1,
                                          HistogramGroup & sum)
  {
  if (!sum.sameSizeAs(group1))
    {
    printIncompatibleLists("sumHistogramGroups(1)",sum,group1);
    throw IncompatibleHistogramGroupException("Incompatible groups","sumHistogramGroups(1)");
    }
  const std::vector<TH1*> & histograms1 = group1.histograms();
  std::vector<TH1*> & histogramSum = sum.histograms();
  for (unsigned int k=0; k<histograms1.size(); k++)
    sumHistograms(a1,histograms1[k],
                  histogramSum[k]);
  }
  
  void HistogramGroup::sumHistogramGroups(double a1, const HistogramGroup & group1,
                                          double a2, const HistogramGroup & group2,
                                          HistogramGroup & sum)
  {
  if (!sum.sameSizeAs(group1,group2))
    {
    printIncompatibleLists("sumHistogramGroups(2)",sum,group1,group2);
    throw IncompatibleHistogramGroupException("Incompatible groups","sumHistogramGroups(2)");
    }
  const std::vector<TH1*> & histograms1 = group1.histograms();
  const std::vector<TH1*> & histograms2 = group2.histograms();
  std::vector<TH1*> & histogramSum = sum.histograms();
  for (unsigned int k=0; k<histograms1.size(); k++)
    sumHistograms(a1,histograms1[k],
                  a2,histograms2[k],
                  histogramSum[k]);
  }
  
  void HistogramGroup::sumHistogramGroups(double a1, const HistogramGroup & group1,
                                          double a2, const HistogramGroup & group2,
                                          double a3, const HistogramGroup & group3,
                                          HistogramGroup & sum)
  {
  if (!sum.sameSizeAs(group1,group2,group3))
    {
    printIncompatibleLists("sumHistogramGroups(3)",sum,group1,group2,group3);
    throw IncompatibleHistogramGroupException("Incompatible groups","sumHistogramGroups(3)");
    }
  const std::vector<TH1*> & histograms1 = group1.histograms();
  const std::vector<TH1*> & histograms2 = group2.histograms();
  const std::vector<TH1*> & histograms3 = group3.histograms();
  std::vector<TH1*> & histogramSum = sum.histograms();
  for (unsigned int k=0; k<histograms1.size(); k++)
    sumHistograms(a1,histograms1[k],
                  a2,histograms2[k],
                  a3,histograms3[k],
                  histogramSum[k]);
  }
  
  void HistogramGroup::sumHistogramGroups(double a1, const HistogramGroup & group1,
                                          double a2, const HistogramGroup & group2,
                                          double a3, const HistogramGroup & group3,
                                          double a4, const HistogramGroup & group4,
                                          HistogramGroup & sum)
  {
  if (!sum.sameSizeAs(group1,group2,group3,group4))
    {
    printIncompatibleLists("sumHistogramGroups(4)",sum,group1,group2,group3,group4);
    throw IncompatibleHistogramGroupException("Incompatible groups","sumHistogramGroups(4)");
    }
  const std::vector<TH1*> & histograms1 = group1.histograms();
  const std::vector<TH1*> & histograms2 = group2.histograms();
  const std::vector<TH1*> & histograms3 = group3.histograms();
  const std::vector<TH1*> & histograms4 = group4.histograms();
  std::vector<TH1*> & histogramSums = sum.histograms();
  for (unsigned int k=0; k<histograms1.size(); k++)
    sumHistograms(a1,histograms1[k],
                  a2,histograms2[k],
                  a3,histograms3[k],
                  a4,histograms4[k],
                  histogramSums[k]);
  }
  
  
  //!
  //! Divide the objects of this group by those of the given group
  //! 
  void HistogramGroup::divideHistogramsLists(double a1, const HistogramGroup & numerator,
                                             double a2, const HistogramGroup & denominator,
                                             HistogramGroup & ratio)
  {
  if (!ratio.sameSizeAs(numerator,denominator))
    {
    printIncompatibleLists("sumHistogramGroups(5)",ratio,numerator,denominator);
    throw IncompatibleHistogramGroupException("Incompatible groups","sumHistogramGroups(5)");
    }
  const std::vector<TH1*> & histograms1 = numerator.histograms();
  const std::vector<TH1*> & histograms2 = denominator.histograms();
  std::vector<TH1*> & histogramsRatio = ratio.histograms();
  for (unsigned int k=0; k<histograms1.size(); k++)
    divideHistograms(a1,histograms1[k],
                     a2,histograms2[k],
                     histogramsRatio[k]);
  }
  
  
  //!
  //! Add HistogramGroup to an external group
  //!
  void HistogramGroup::addHistogramsToExtList(TList *group)
  {
  for (auto & object  : histograms()) group->Add(object );
  }
  
  //!
  //! Load objects in the given file and add them to this group.
  //!
  void  HistogramGroup::loadList(TFile & inputFile)
  {
  TIter keyList(inputFile.GetListOfKeys());
  TKey *key;
  while ((key = (TKey*)keyList()))
    {
    TClass *cl = gROOT->GetClass(key->GetClassName());
    if (!cl->InheritsFrom("TH1")) continue;
    TH1 *h = (TH1*)key->ReadObj();
    if (!h) throw HistogramException("Histogram","Histogram not read",__FUNCTION__);
    append(h);
    }
  }
  
  //! 
  //! Add the objects of the given group to those of this group
  //!
  HistogramGroup * HistogramGroup::clone() const
  {
  HistogramGroup * newGroup = new HistogramGroup();
  for (auto & object : _objects) newGroup->append((TH1*) object ->Clone());
  return newGroup;
  }
  
  
  void HistogramGroup::printIncompatibleLists(const String & caller,
                                              const HistogramGroup & group1,
                                              const HistogramGroup & group2)
  {
  printCR();
  printValue("Incompatible groups in call from",caller);
  printValue("HistogramGroup 1: Name",group1.name());
  printValue("HistogramGroup 1: Size",group1.size());
  printValue("HistogramGroup 2: Name",group2.name());
  printValue("HistogramGroup 2: Size",group2.size());
  }
  
  void HistogramGroup::printIncompatibleLists(const String & caller,
                                              const HistogramGroup & group1,
                                              const HistogramGroup & group2,
                                              const HistogramGroup & group3)
  {
  printCR();
  printValue("Incompatible groups in call from",caller);
  printValue("HistogramGroup 1: Name",group1.name());
  printValue("HistogramGroup 1: Size",group1.size());
  printValue("HistogramGroup 2: Name",group2.name());
  printValue("HistogramGroup 2: Size",group2.size());
  printValue("HistogramGroup 3: Name",group3.name());
  printValue("HistogramGroup 3: Size",group3.size());
  }
  
  
  void HistogramGroup::printIncompatibleLists(const String & caller,
                                              const HistogramGroup & group1,
                                              const HistogramGroup & group2,
                                              const HistogramGroup & group3,
                                              const HistogramGroup & group4)
  {
  printCR();
  printValue("Incompatible groups in call from",caller);
  printValue("HistogramGroup 1: Name",group1.name());
  printValue("HistogramGroup 1: Size",group1.size());
  printValue("HistogramGroup 2: Name",group2.name());
  printValue("HistogramGroup 2: Size",group2.size());
  printValue("HistogramGroup 3: Name",group3.name());
  printValue("HistogramGroup 4: Name",group4.name());
  printValue("HistogramGroup 4: Size",group4.size());
  }
  
  void HistogramGroup::printIncompatibleLists(const String & caller,
                                              const HistogramGroup & group1,
                                              const HistogramGroup & group2,
                                              const HistogramGroup & group3,
                                              const HistogramGroup & group4,
                                              const HistogramGroup & group5)
  {
  printCR();
  printValue("Incompatible groups in call from",caller);
  printValue("HistogramGroup 1: Name",group1.name());
  printValue("HistogramGroup 1: Size",group1.size());
  printValue("HistogramGroup 2: Name",group2.name());
  printValue("HistogramGroup 2: Size",group2.size());
  printValue("HistogramGroup 3: Name",group3.name());
  printValue("HistogramGroup 4: Name",group4.name());
  printValue("HistogramGroup 4: Size",group4.size());
  printValue("HistogramGroup 5: Name",group5.name());
  printValue("HistogramGroup 5: Size",group5.size());
  }
  
  
  //void HistogramGroup::ratioHistogramGroups(const HistogramGroup & group1,
  //                                        const HistogramGroup & group2, bool correlatedUncertainties)
  //{
  //  if (!g1.sameSizeAs(g2))
  //    {
  //      printIncompatibleGroups(__FUNCTION__,g1,g2);
  //      throw IncompatibleHistogramGroupException("Incompatible groups","sumHistogramGroups(5)");
  //    }
  //  const std:: vector<TH1*> & histograms1 = g1.histograms();
  //  const std:: vector<TH1*> & histograms2 = g2.histograms();
  //  int k = 0;
  //  for (auto & h1 : histograms1)
  //    {
  //    TH1* h2 = histograms2[k++];
  //    if (!sameDimensions(__FUNCTION__,h1,h2))  throw IncompatibleHistogramException(__FUNCTION__);
  //    TH1* hRatio = (TH1*)  h1->Clone();
  //    String name = hRatio->name();
  //    name.ReplaceAll("Reco","Ratio");
  //    hRatio->SetName(name);
  //    hRatio->SetTitle(name);
  //    ratioHistos(h1,h2,hRatio,correlatedUncertainties);
  //    append(hRatio);
  //    }
  //}
  
  //!
  //! Calculate square difference (bin by bin) of all  objects of the other  group relative to the
  //! reference group and store the result in the objects of  this group.
  //! TProfile objects have to be handled differently
  //! This function is used for sub-sample analyses..
  //! double sumWeights: sum of the weights of the n-1 group accumulated in this group
  //!     double weight: weight of the current n-th group.
  //! The means are store in the BinContent
  //! The rms are store in the BinError parts of the objects.
  //!
  void HistogramGroup::squareDifferenceLists(const HistogramGroup & group1,
                                             double sumWeights, double weight, int n)
  {
  std:: vector<TH1*> & histograms0 = _objects;
  const std:: vector<TH1*> & histograms1 = group1._objects;
  
  int k = 0;
  for (auto & h0 : histograms0)
    {
    TH1* h1 = histograms1[k++];
    squareDifferenceHistos(h0, h1, sumWeights, weight, n);
    }
  }
  
  
  TH1 * HistogramGroup::importH1(TFile & inputFile,const String & histoName)
  {
  return append(loadH1(inputFile,histoName));
  }
  
  ///Load the given 1D object  (name) from the given TFile
  ///No test is //done to verify that the file is properly opened.
  
  TH2 * HistogramGroup::importH2(TFile & inputFile,const String & histoName)
  {
  return append(loadH2(inputFile,histoName));
  }
  
  ///Load the given 3D object  (name) from the given TFile
  ///No test is //done to verify that the file is properly opened.
  TH3 * HistogramGroup::importH3(TFile & inputFile, const String & histoName)
  {
  return append(loadH3(inputFile,histoName));
  }
  
  ///Load the given 3D object  (name) from the given TFile
  ///No test is //done to verify that the file is properly opened.
  TProfile * HistogramGroup::importProfile(TFile & inputFile, const String & histoName)
  {
  return append(loadProfile(inputFile,histoName));
  }
  
  TProfile2D * HistogramGroup::importProfile2D(TFile & inputFile, const String & histoName)
  {
  return append(loadProfile2D(inputFile,histoName));
  }
  
  void HistogramGroup::loadInList(TFile & inputFile, HistogramGroup * group)
  {
  std::vector<TH1*> & _objects = group->histograms();
  for (auto & h : _objects) append(loadH1(inputFile,h->name()));
  }
  
  void HistogramGroup::sumw2All()
  {
  for (auto & h : histograms())
    {
    TClass * c = h->IsA();
    if (c==TProfile::Class() || c==TProfile2D::Class()) continue;
    if (h->GetEntries()>0) h->Sumw2();
    }
  }
  
  void HistogramGroup::scaleAllHistoByBinWidth(double scale)
  {
  for (auto & h : histograms())
    {
    TClass * c = h->IsA();
    if (c == TProfile::Class() || c == TProfile2D::Class()) continue;
    else if (c == TH1F::Class() || c == TH1D::Class()) scaleByBinWidth1D(h, scale);
    else if (c == TH2F::Class() || c == TH2D::Class()) scaleByBinWidth2D((TH2*) h, scale);
    }
  }
  
  HistogramGroup operator+(const HistogramGroup & lhs,const HistogramGroup & rhs)
  {
  if (!lhs.sameSizeAs(rhs))
    {
    //  printIncompatibleLists("lhs and rhs have different histograms",lhs,rhs);
    throw IncompatibleHistogramGroupException("lhs and rhs have different histograms",__FUNCTION__);
    }
  HistogramGroup * sumList = new HistogramGroup();
  const std::vector<TH1*> & lhsHistos = lhs.histograms();
  const std::vector<TH1*> & rhsHistos = rhs.histograms();
  for (unsigned int k=0; k<lhsHistos.size(); k++)
    {
    TH1 * sum =  (TH1*) lhsHistos[k]->Clone();
    sum->Add(rhsHistos[k]);
    sumList->append(sum);
    }
  return *sumList;
  }
  
  HistogramGroup operator-(const HistogramGroup & lhs,const HistogramGroup & rhs)
  {
  if (!lhs.sameSizeAs(rhs))
    {
    // printIncompatibleLists("lhs and rhs have different histograms",lhs,rhs);
    throw IncompatibleHistogramGroupException("lhs and rhs have different histograms",__FUNCTION__);
    }
  HistogramGroup * sumList = new HistogramGroup();
  const std::vector<TH1*> & lhsHistos = lhs.histograms();
  const std::vector<TH1*> & rhsHistos = rhs.histograms();
  for (unsigned int k=0; k<lhsHistos.size(); k++)
    {
    TH1 * sum = (TH1*) lhsHistos[k]->Clone();
    sum->Add(rhsHistos[k],-1.0);
    sumList->append(sum);
    }
  }
  
  
  
} // namespace CAP
