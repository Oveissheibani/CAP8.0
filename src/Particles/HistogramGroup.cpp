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
#include "TROOT.h"
#include "TKey.h"
#include "TSystem.h"
#include "HistogramGroup.hpp"
#include "PrintHelpers.hpp"
#include "NameHelpers.hpp"
#include "RootHistogramHelpers.hpp"
#include "Task.hpp"

ClassImp(CAP::HistogramGroup);

namespace CAP
{
  
  HistogramGroup::HistogramGroup()
  :
  ManagedObject(),
  _histogramBaseName("NOTSET"),
  _parentTask(nullptr),
  _objects()
  {
  setClassName("HistogramGroup");
  setName("HistogramGroup");
  setTitle("HistogramGroup");
  setClassName("HistogramGroup");
  }
  
  HistogramGroup::HistogramGroup(const HistogramGroup & source)
  :
  ManagedObject(source),
  _histogramBaseName(source._histogramBaseName),
  _parentTask(source._parentTask),
  _objects()
  {  }
  
  HistogramGroup & HistogramGroup::operator=(const HistogramGroup & rhs)
  {
  if (this!=&rhs)
    {
    ManagedObject::operator=(rhs);
    _histogramBaseName = rhs._histogramBaseName;
    _parentTask = rhs._parentTask;
    }
  return *this;
  }
  
  const String HistogramGroup::parentName() const
  {
  return _parentTask->name();
  }
  
  const String HistogramGroup::parentPathName() const
  {
  return _parentTask->fullTaskPath();
  }
  
  const std::vector<TH1*> & HistogramGroup::histograms() const
  {
  return  _objects;
  }
  
  std::vector<TH1*> & HistogramGroup::histograms()
  {
  return  _objects;
  }
  
  void HistogramGroup::configure(const String & taskName,
                                 const String & objectType,
                                 const Configuration & configuration,
                                 unsigned int index)
  {
  if (reportDebug(__FUNCTION__)) { printCR(); }
  _histogramBaseName  = configuration.valueString(createKey(taskName,objectType,index,"BASE_NAME"));
  }
  
  //!
  //!Reset all objects to zero
  //!
  void HistogramGroup::reset()
  {
  if (reportDebug(__FUNCTION__)) { printCR(); }
  for (auto & object : _objects) object ->Reset();
  }
  
  //!
  //!Clear all objects
  //!
  void HistogramGroup::clear()
  {
  if (reportDebug(__FUNCTION__)) { printCR(); }
  _objects.clear();
  }
  
  void HistogramGroup::print() const
  {
  ManagedObject::print();
  printValue("histogramBaseName",_histogramBaseName);
  printValue("bn",_histogramBaseName);
  printValue("N Histograms",size());
  for (auto & object : _objects) printHistoInfo(object);
  }
  
  void HistogramGroup::saveTo(TFile & file)
  {
  if (reportDebug(__FUNCTION__)) { printCR(); }
  file.cd();
  // Defensive: a histogram in _objects may be (a) nullptr (a flag-gated
  // create() never allocated it) or (b) currently attached to a file
  // that's already been closed (ROOT's TFile destructor moves owned
  // histograms with it) or (c) point to freed memory.  Skip nulls and
  // re-anchor each histogram on the current file before writing so
  // Write() never dereferences a dangling pointer or writes into the
  // wrong directory.  Per-object try/catch avoids one bad histogram
  // taking down the whole stage.
  size_t idx = 0;
  for (auto & object : _objects)
    {
    if (!object) { ++idx; continue; }
    try
      {
      object->SetDirectory(&file);
      object->Write();
      }
    catch (std::exception & ex)
      {
      static int warned = 0;
      if (warned++ < 8)
        {
        printCR();
        printValue("HistogramGroup::saveTo: skip exception at idx", (long)idx);
        printValue("HistogramGroup::saveTo: histogram",
                   String(object ? object->GetName() : "<null>"));
        printValue("HistogramGroup::saveTo: what",  String(ex.what()));
        }
      }
    catch (...)
      {
      static int warned = 0;
      if (warned++ < 8)
        {
        printCR();
        printValue("HistogramGroup::saveTo: skip unknown exception at idx",
                   (long)idx);
        printValue("HistogramGroup::saveTo: histogram",
                   String(object ? object->GetName() : "<null>"));
        }
      }
    ++idx;
    }
  }
  
  void HistogramGroup::scaleHistograms(double a)
  {
  if (reportInfo(__FUNCTION__)) { printCR(); }
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
  for (unsigned int k=0; k< _objects.size(); k++) _objects[k]->Add(rhsHistos[k],1.0);
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
  
  
  void HistogramGroup::sumHistogramGroups(double a1, const HistogramGroup & list1,
                                          HistogramGroup & sum)
  {
  if (!sum.sameSizeAs(list1))
    {
    printIncompatibleLists("sumHistogramGroups(1)",sum,list1);
    throw IncompatibleHistogramGroupException("Incompatible lists","sumHistogramGroups(1)");
    }
  const std::vector<TH1*> & histograms1 = list1.histograms();
  std::vector<TH1*> & histogramSum = sum.histograms();
  for (unsigned int k=0; k<histograms1.size(); k++)
    sumHistograms(a1,histograms1[k],
                  histogramSum[k]);
  }
  
  void HistogramGroup::sumHistogramGroups(double a1, const HistogramGroup & list1,
                                          double a2, const HistogramGroup & list2,
                                          HistogramGroup & sum)
  {
  if (!sum.sameSizeAs(list1,list2))
    {
    printIncompatibleLists("sumHistogramGroups(2)",sum,list1,list2);
    throw IncompatibleHistogramGroupException("Incompatible lists","sumHistogramGroups(2)");
    }
  const std::vector<TH1*> & histograms1 = list1.histograms();
  const std::vector<TH1*> & histograms2 = list2.histograms();
  std::vector<TH1*> & histogramSum = sum.histograms();
  for (unsigned int k=0; k<histograms1.size(); k++)
    sumHistograms(a1,histograms1[k],
                  a2,histograms2[k],
                  histogramSum[k]);
  }
  
  void HistogramGroup::sumHistogramGroups(double a1, const HistogramGroup & list1,
                                          double a2, const HistogramGroup & list2,
                                          double a3, const HistogramGroup & list3,
                                          HistogramGroup & sum)
  {
  if (!sum.sameSizeAs(list1,list2,list3))
    {
    printIncompatibleLists("sumHistogramGroups(3)",sum,list1,list2,list3);
    throw IncompatibleHistogramGroupException("Incompatible lists","sumHistogramGroups(3)");
    }
  const std::vector<TH1*> & histograms1 = list1.histograms();
  const std::vector<TH1*> & histograms2 = list2.histograms();
  const std::vector<TH1*> & histograms3 = list3.histograms();
  std::vector<TH1*> & histogramSum = sum.histograms();
  for (unsigned int k=0; k<histograms1.size(); k++)
    sumHistograms(a1,histograms1[k],
                  a2,histograms2[k],
                  a3,histograms3[k],
                  histogramSum[k]);
  }
  
  void HistogramGroup::sumHistogramGroups(double a1, const HistogramGroup & list1,
                                          double a2, const HistogramGroup & list2,
                                          double a3, const HistogramGroup & list3,
                                          double a4, const HistogramGroup & list4,
                                          HistogramGroup & sum)
  {
  if (!sum.sameSizeAs(list1,list2,list3,list4))
    {
    printIncompatibleLists("sumHistogramGroups(4)",sum,list1,list2,list3,list4);
    throw IncompatibleHistogramGroupException("Incompatible lists","sumHistogramGroups(4)");
    }
  const std::vector<TH1*> & histograms1 = list1.histograms();
  const std::vector<TH1*> & histograms2 = list2.histograms();
  const std::vector<TH1*> & histograms3 = list3.histograms();
  const std::vector<TH1*> & histograms4 = list4.histograms();
  std::vector<TH1*> & histogramSums = sum.histograms();
  for (unsigned int k=0; k<histograms1.size(); k++)
    sumHistograms(a1,histograms1[k],
                  a2,histograms2[k],
                  a3,histograms3[k],
                  a4,histograms4[k],
                  histogramSums[k]);
  }
  
  
  //!
  //! Divide the objects of this list by those of the given list
  //! 
  void HistogramGroup::divideHistogramsLists(double a1, const HistogramGroup & numerator,
                                             double a2, const HistogramGroup & denominator,
                                             HistogramGroup & ratio)
  {
  if (!ratio.sameSizeAs(numerator,denominator))
    {
    printIncompatibleLists("sumHistogramGroups(5)",ratio,numerator,denominator);
    throw IncompatibleHistogramGroupException("Incompatible lists","sumHistogramGroups(5)");
    }
  const std::vector<TH1*> & histograms1 = numerator.histograms();
  const std::vector<TH1*> & histograms2 = denominator.histograms();
  std::vector<TH1*> & histogramsRatio = ratio.histograms();
  for (unsigned int k=0; k<histograms1.size(); k++)
    divideHistograms(a1,histograms1[k],
                     a2,histograms2[k],
                     histogramsRatio[k]);
  }
  
  TH1 * HistogramGroup::append(TH1* h)
  {
  _objects.push_back(h);
  return h;
  }
  
  TH2 * HistogramGroup::append(TH2* h)
  {
  _objects.push_back(h);
  return h;
  }
  
  TH3 * HistogramGroup::append(TH3* h)
  {
  _objects.push_back(h);
  return h;
  }
  
  TProfile * HistogramGroup::append(TProfile* h)
  {
  _objects.push_back(h);
  return h;
  }
  
  TProfile2D * HistogramGroup::append(TProfile2D * h)
  {
  _objects.push_back(h);
  return h;
  }
  
  
  
  //!
  //! Create 1D object 
  //!
  TH1 * HistogramGroup::createHistogram(const String & name,
                                        int n_x, double min_x, double max_x,
                                        const String & title_x,
                                        const String & title_y,
                                        int storageOption)
  {
  return append(createNewHistogram(name,n_x,min_x,max_x,title_x,title_y,storageOption));
  }
  
  
  
  //!
  //! Create 1D object 
  //!
  TH1 * HistogramGroup::createHistogram(const String & name,
                                        int n_x, double * bins,
                                        const String & title_x,
                                        const String & title_y,
                                        int storageOption)
  {
  return append(createNewHistogram(name,n_x,bins,title_x,title_y,storageOption));
  }
  
  //!
  //! Create 2D object 
  //!
  TH2 * HistogramGroup::createHistogram(const String & name,
                                        int n_x, double min_x, double max_x,
                                        int n_y, double rapidity_minimum, double rapidity_maximum,
                                        const String & title_x,
                                        const String & title_y,
                                        const String & title_z,
                                        int storageOption)
  {
  return append(createNewHistogram(name,n_x,min_x,max_x,n_y,rapidity_minimum,rapidity_maximum,title_x,title_y,title_z,storageOption));
  }
  
  //!
  // Create 2D object 
  //!
  TH2 * HistogramGroup::createHistogram(const String & name,
                                        int n_x, double* xbins, 
                                        int n_y, double rapidity_minimum, double rapidity_maximum,
                                        const String & title_x,
                                        const String & title_y,
                                        const String & title_z,
                                        int storageOption)
  
  {
  return append(createNewHistogram(name,n_x,xbins,n_y,rapidity_minimum,rapidity_maximum,title_x,title_y,title_z,storageOption));
  }
  
  
  //!
  // Create 3D object 
  //!
  TH3 * HistogramGroup::createHistogram(const String & name,
                                        int n_x, double min_x, double max_x,
                                        int n_y, double rapidity_minimum, double rapidity_maximum,
                                        int n_z, double z_min, double z_max,
                                        const String & title_x,
                                        const String & title_y,
                                        const String & title_z,
                                        int storageOption)
  
  {
  return append(createNewHistogram(name,n_x,min_x,max_x,n_y,rapidity_minimum,rapidity_maximum,n_z,z_min,z_max,title_x,title_y,title_z,storageOption));
  }
  
  //!
  //! Create 1D profile object 
  //!
  TProfile * HistogramGroup::createProfile(const String & name,
                                           int n_x,double min_x,double max_x,
                                           const String & title_x,
                                           const String & title_y)
  {
  return append(createNewProfile(name,n_x,min_x,max_x,title_x,title_y));
  }
  
  //!
  // Create 1D profile object 
  //!
  TProfile * HistogramGroup::createProfile(const String & name,
                                           int n_x,  double* bins,
                                           const String & title_x,
                                           const String & title_y)
  {
  return append(createNewProfile(name,n_x,bins,title_x,title_y));
  }
  
  //!
  // Create 2D profile object 
  //!
  TProfile2D * HistogramGroup::createProfile(const String & name,
                                             int n_x, double min_x, double max_x,
                                             int n_y, double rapidity_minimum, double rapidity_maximum,
                                             const String & title_x,
                                             const String & title_y,
                                             const String & title_z)
  {
  return append(createNewProfile(name,n_x,min_x,max_x,n_y,rapidity_minimum,rapidity_maximum,title_x,title_y,title_z));
  }
  
  //!
  //! Add HistogramGroup to an external list
  //!
  void HistogramGroup::addHistogramsToExtList(TList *list)
  {
  for (auto & object  : histograms()) list->Add(object );
  }
  
  //!
  //! Load objects in the given file and add them to this list.
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
  //! Add the objects of the given list to those of this list
  //!
  HistogramGroup * HistogramGroup::clone() const
  {
  HistogramGroup * newGroup = new HistogramGroup();
  for (auto & object : _objects) newGroup->append((TH1*) object ->Clone());
  return newGroup;
  }
  
  bool HistogramGroup::sameSizeAs(const HistogramGroup & g) const
  {
  return size() == g.size();
  }
  
  bool HistogramGroup::sameSizeAs(const HistogramGroup & g1, const HistogramGroup & g2) const
  {
  Size_t n = size();
  return n==g1.size()  &&  n==g2.size();
  }
  
  bool HistogramGroup::sameSizeAs(const HistogramGroup & g1,
                                  const HistogramGroup & g2,
                                  const HistogramGroup & g3) const
  {
  Size_t n = size();
  return n==g1.size()  &&  n==g2.size()  &&  n==g3.size();
  }
  
  bool HistogramGroup::sameSizeAs(const HistogramGroup & g1,
                                  const HistogramGroup & g2,
                                  const HistogramGroup & g3,
                                  const HistogramGroup & g4) const
  {
  Size_t n = size();
  return n==g1.size()  &&  n==g2.size()  &&  n==g3.size()  &&  n==g4.size();
  }
  
  
  void HistogramGroup::printIncompatibleLists(const String & caller,
                                              const HistogramGroup & list1,
                                              const HistogramGroup & list2)
  {
  printCR();
  printValue("Incompatible lists in call from",caller);
  printValue("HistogramGroup 1: Name",list1.name());
  printValue("HistogramGroup 1: Size",list1.size());
  printValue("HistogramGroup 2: Name",list2.name());
  printValue("HistogramGroup 2: Size",list2.size());
  }
  
  void HistogramGroup::printIncompatibleLists(const String & caller,
                                              const HistogramGroup & list1,
                                              const HistogramGroup & list2,
                                              const HistogramGroup & list3)
  {
  printCR();
  printValue("Incompatible lists in call from",caller);
  printValue("HistogramGroup 1: Name",list1.name());
  printValue("HistogramGroup 1: Size",list1.size());
  printValue("HistogramGroup 2: Name",list2.name());
  printValue("HistogramGroup 2: Size",list2.size());
  printValue("HistogramGroup 3: Name",list3.name());
  printValue("HistogramGroup 3: Size",list3.size());
  }
  
  
  void HistogramGroup::printIncompatibleLists(const String & caller,
                                              const HistogramGroup & list1,
                                              const HistogramGroup & list2,
                                              const HistogramGroup & list3,
                                              const HistogramGroup & list4)
  {
  printCR();
  printValue("Incompatible lists in call from",caller);
  printValue("HistogramGroup 1: Name",list1.name());
  printValue("HistogramGroup 1: Size",list1.size());
  printValue("HistogramGroup 2: Name",list2.name());
  printValue("HistogramGroup 2: Size",list2.size());
  printValue("HistogramGroup 3: Name",list3.name());
  printValue("HistogramGroup 4: Name",list4.name());
  printValue("HistogramGroup 4: Size",list4.size());
  }
  
  void HistogramGroup::printIncompatibleLists(const String & caller,
                                              const HistogramGroup & list1,
                                              const HistogramGroup & list2,
                                              const HistogramGroup & list3,
                                              const HistogramGroup & list4,
                                              const HistogramGroup & list5)
  {
  printCR();
  printValue("Incompatible lists in call from",caller);
  printValue("HistogramGroup 1: Name",list1.name());
  printValue("HistogramGroup 1: Size",list1.size());
  printValue("HistogramGroup 2: Name",list2.name());
  printValue("HistogramGroup 2: Size",list2.size());
  printValue("HistogramGroup 3: Name",list3.name());
  printValue("HistogramGroup 4: Name",list4.name());
  printValue("HistogramGroup 4: Size",list4.size());
  printValue("HistogramGroup 5: Name",list5.name());
  printValue("HistogramGroup 5: Size",list5.size());
  }
  
  
  //void HistogramGroup::ratioHistogramGroups(const HistogramGroup & list1,
  //                                        const HistogramGroup & list2, bool correlatedUncertainties)
  //{
  //  if (!g1.sameSizeAs(g2))
  //    {
  //      printIncompatibleGroups(__FUNCTION__,g1,g2);
  //      throw IncompatibleHistogramGroupException("Incompatible lists","sumHistogramGroups(5)");
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
  //! Calculate square difference (bin by bin) of all  objects of the other  list relative to the
  //! reference list and store the result in the objects of  this list.
  //! TProfile objects have to be handled differently
  //! This function is used for sub-sample analyses..
  //! double sumWeights: sum of the weights of the n-1 list accumulated in this list
  //!     double weight: weight of the current n-th list.
  //! The means are store in the BinContent
  //! The rms are store in the BinError parts of the objects.
  //!
  void HistogramGroup::squareDifferenceLists(const HistogramGroup & list1,
                                             double sumWeights, double weight, int n)
  {
  std:: vector<TH1*> & histograms0 = _objects;
  const std:: vector<TH1*> & histograms1 = list1._objects;
  
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
  
  void HistogramGroup::loadInList(TFile & inputFile, HistogramGroup * list)
  {
  std::vector<TH1*> & _objects = list->histograms();
  for (auto & h : _objects) append(loadH1(inputFile,h->GetName()));
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
  return *sumList;
  }
  
  
  
} // namespace CAP
