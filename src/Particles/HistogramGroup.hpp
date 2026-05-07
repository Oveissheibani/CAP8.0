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
#ifndef CAP__HistogramGroup
#define CAP__HistogramGroup
#include <ostream>
#include "TFile.h"
#include "TH1.h"
#include "TProfile2D.h"
#include "Aliases.hpp"
#include "Configuration.hpp"
#include "ManagedObject.hpp"
#include "Exceptions.hpp"

namespace CAP
{

  class Task;

  class HistogramGroup
  :
  public ManagedObject
  {
  protected:

  String _histogramBaseName;
  Task * _parentTask;
  std::vector<TH1*> _objects;

  public:

  HistogramGroup();
  HistogramGroup(const HistogramGroup & source);
  HistogramGroup & operator=(const HistogramGroup & rhs);
  HistogramGroup & operator+=(const HistogramGroup & rhs);
  HistogramGroup & operator-=(const HistogramGroup & rhs);
  virtual ~HistogramGroup() {}
  const std::vector<TH1*> & histograms() const;
  std::vector<TH1*> & histograms();
  virtual void configure(const String & taskName,
                         const String & objectType,
                         const Configuration & configuration,
                         unsigned int index);
  //  virtual void loadFrom(TFile & inputFile);
  //  virtual void loadFromAscii(std::ifstream & file __attribute__((unused))) {}
  //  virtual void loadFromAscii(std::ifstream & file1 __attribute__((unused)),
  //                                  std::ifstream & file2 __attribute__((unused))) {}
  virtual void saveTo(TFile & file);
  virtual void reset();
  virtual void clear();
  virtual void print() const;
  virtual void scaleHistograms(double a);
  Task * parentTask() const  { return _parentTask; }
  const String parentName() const;
  const String parentPathName() const;
  void  setParentTask(Task * parentTask)
  {
  _parentTask = parentTask;
  }

  const String & baseName()
  {
  return _histogramBaseName;
  }

  TH1 * append(TH1* h);
  TH2 * append(TH2* h);
  TH3 * append(TH3* h);
  TProfile * append(TProfile* h);
  TProfile2D * append(TProfile2D* h);
  TH1 * createHistogram(const String & name,
                        int n, double min_x, double max_x,
                        const String & title_x="",
                        const String & title_y="",
                        int storageOption=1);
  TH1 * createHistogram(const String & name,
                        int n, double * bins,
                        const String & title_x="",
                        const String & title_y="",
                        int storageOption=1);
  TH2 * createHistogram(const String & name,
                        int n_x, double min_x, double max_x,
                        int n_y, double rapidity_minimum, double rapidity_maximum,
                        const String & title_x="",
                        const String & title_y="",
                        const String & title_z="",
                        int storageOption=1);
  TH2 * createHistogram(const String & name,
                        int n_x, double* xbins, int n_y, double rapidity_minimum, double rapidity_maximum,
                        const String & title_x="",
                        const String & title_y="",
                        const String & title_z="",
                        int storageOption=1);
  TH3 * createHistogram(const String & name,
                        int n_x, double min_x, double max_x,
                        int n_y, double rapidity_minimum, double rapidity_maximum,
                        int n_z, double z_min, double z_max,
                        const String & title_x="",
                        const String & title_y="",
                        const String & title_z="",
                        int storageOption=1);
  TProfile * createProfile(const String & _name,
                           int n_x,double min_x,double max_x,
                           const String & title_x,
                           const String & title_y);

  TProfile * createProfile(const String & name,
                           int n_x,  double* bins,
                           const String & title_x,
                           const String & title_y);

  TProfile2D * createProfile(const String & title,
                             int n_x, double min_x, double max_x,
                             int n_y, double rapidity_minimum, double rapidity_maximum,
                             const String & title_x,
                             const String & title_y,
                             const String & title_z);

  void addHistogramsToExtList(TList *list);

  bool sameSizeAs(const HistogramGroup & list) const;
  bool sameSizeAs(const HistogramGroup & list1, const HistogramGroup & list2) const;
  bool sameSizeAs(const HistogramGroup & list1, const HistogramGroup & list2, const HistogramGroup & list3) const;
  bool sameSizeAs(const HistogramGroup & list1, const HistogramGroup & list2, const HistogramGroup & list3, const HistogramGroup & list4) const;

  //
  void loadList(TFile & inputFile);

  virtual void sumHistogramGroups(double a1, const HistogramGroup & list1,
                                  HistogramGroup & sum);
  virtual void sumHistogramGroups(double a1, const HistogramGroup & list1,
                                  double a2, const HistogramGroup & list2,
                                  HistogramGroup & sum);
  virtual void sumHistogramGroups(double a1, const HistogramGroup & list1,
                                  double a2, const HistogramGroup & list2,
                                  double a3, const HistogramGroup & list3,
                                  HistogramGroup & sum);
  virtual void sumHistogramGroups(double a1, const HistogramGroup & list1,
                                  double a2, const HistogramGroup & list2,
                                  double a3, const HistogramGroup & list3,
                                  double a4, const HistogramGroup & list4,
                                  HistogramGroup & sum);

  virtual void divideHistogramsLists(double a1, const HistogramGroup & numerator,
                                     double a2, const HistogramGroup & denominator,
                                     HistogramGroup & ratio);



  void squareDifferenceLists(const HistogramGroup & list,
                             double sumWeights, double weight, int n);

  HistogramGroup * clone() const;


  void printIncompatibleLists(const String & caller, const HistogramGroup & list1, const HistogramGroup & list2);
  void printIncompatibleLists(const String & caller, const HistogramGroup & list1, const HistogramGroup & list2, const HistogramGroup & list3);
  void printIncompatibleLists(const String & caller, const HistogramGroup & list1, const HistogramGroup & list2, const HistogramGroup & list3, const HistogramGroup & list4);
  void printIncompatibleLists(const String & caller, const HistogramGroup & list1, const HistogramGroup & list2, const HistogramGroup & list3, const HistogramGroup & list4, const HistogramGroup & list5);

  void sumw2All();
  void scaleAllHistoByBinWidth(double scale);
  TH1 * importH1(TFile & inputFile,const String & histoName);
  TH2 * importH2(TFile & inputFile,const String & histoName);
  TH3 * importH3(TFile & inputFile,const String & histoName);
  TProfile * importProfile(TFile & inputFile, const String & histoName);
  TProfile2D * importProfile2D(TFile & inputFile, const String & histoName);
  void loadInList(TFile & inputFile, HistogramGroup * list);

  unsigned size() const
  {
  return _objects.size();
  }

  void setHistogramBaseName(const String & name)
  {
  _histogramBaseName = name;
  }

  const String & histogramBaseName() const
  {
  return _histogramBaseName;
  }

  friend HistogramGroup operator+(const HistogramGroup & lhs,const HistogramGroup & rhs);
  friend HistogramGroup operator-(const HistogramGroup & lhs,const HistogramGroup & rhs);

  ClassDef(HistogramGroup,0)
  };

  HistogramGroup operator+(const HistogramGroup & lhs,const HistogramGroup & rhs);
  HistogramGroup operator-(const HistogramGroup & lhs,const HistogramGroup & rhs);


} // namespace CAP

#endif /* CAP__HistogramGroup */
