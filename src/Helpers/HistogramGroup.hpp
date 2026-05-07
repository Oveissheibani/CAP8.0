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
#include "Exceptions.hpp"
#include "ManagedObject.hpp"
#include "NameHelpers.hpp"
#include "PrintHelpers.hpp"
#include "RootHistogramHelpers.hpp"

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

  ~HistogramGroup() {}
  const std::vector<TH1*> & histograms() const
  {
  return _objects;
  }

  std::vector<TH1*> & histograms()
  {
  return _objects;
  }

  //!
  //!Reset all objects to zero
  //!
  virtual void reset()
  {
  if (reportDebug(__FUNCTION__)) { printCR(); }
  for (auto & object  : _objects) object->Reset();
  }

  //!
  //!Clear all objects
  //!
  virtual void clear() { _objects.clear(); }

  virtual void configure(const String & taskName,
                               const String & objectType,
                               const Configuration & configuration,
                               unsigned int index);
  //!
  //! Overload this class to create objects.
  //!
  virtual void create()
  {
  throw Exception("Improper call to base class method",__FUNCTION__);
  }

  //!
  //! Overload this class to load objects.
  //!
  virtual void loadFrom(TFile & inputFile __attribute__((unused)))
  {
  throw Exception("Improper call to base class method", __FUNCTION__);
  }

  //!
  //!Save histograms oswed by this group to the given file.
  //!
  virtual void saveTo(TFile & file);
  virtual void scaleHistograms(double a);
//  virtual void deepCopy(const HistogramGroup & source);

  //!
  //!Return the parent task owning this group
  //!No check is done to see if the parent exist.
  //!
  Task * parentTask() const
  {
  return _parentTask;
  }

  //!
  //!Return the base name of the histograms of this group
  //!
  const String & baseName() const
  { return _histogramBaseName;}


  const String parentName() const;
  const String parentPathName() const;

  void  setParentTask(Task * parentTask)
  {
  _parentTask = parentTask;
  }

  //!
  //!Append the given histogram to this group
  //!The histogram pointer must point to an existing histogram, otherwise
  //!an exception is thrown. Return the pointer h.
  //!
  TH1 * append(TH1* h)
  {
  if (!h) throw Exception("Given argument h is a null pointer",__FUNCTION__);
    _objects.push_back(h);
    return h;
  }

  //!
  //!Append the given histogram to this group
  //!The histogram pointer must point to an existing histogram, otherwise
  //!an exception is thrown. Return the pointer h.
  //!
  TH2 * append(TH2* h)
  {
  if (!h) throw Exception("Given argument h is a null pointer",__FUNCTION__);
  _objects.push_back(h);
  return h;
  }

  //!
  //!Append the given histogram to this group
  //!The histogram pointer must point to an existing histogram, otherwise
  //!an exception is thrown. Return the pointer h.
  //!
  TH3 * append(TH3* h)
  {
  if (!h) throw Exception("Given argument h is a null pointer",__FUNCTION__);
  _objects.push_back(h);
  return h;
  }

  //!
  //!Append the given histogram to this group
  //!The histogram pointer must point to an existing histogram, otherwise
  //!an exception is thrown. Return the pointer h.
  //!
  TProfile * append(TProfile* h)
  {
  if (!h) throw Exception("Given argument h is a null pointer",__FUNCTION__);
  _objects.push_back(h);
  return h;
  }

  //!
  //!Append the given histogram to this group
  //!The histogram pointer must point to an existing histogram, otherwise
  //!an exception is thrown. Return the pointer h.
  //!
  TProfile2D * append(TProfile2D* h)
  {
  if (!h) throw Exception("Given argument h is a null pointer",__FUNCTION__);
  _objects.push_back(h);
  return h;
  }

  //!
  //! Create 1D object
  //!
  TH1 * createHistogram(const String & name,
                        int n_x, double min_x, double max_x,
                        const String & title_x="",
                        const String & title_y="",
                        int storageOption=1)
  {
  return append(createNewHistogram(name,n_x,min_x,max_x,title_x,title_y,storageOption));
  }

  //!
  //! Create 1D object
  //!
  TH1 * createHistogram(const String & name,
                        int n_x, double * bins,
                        const String & title_x="",
                        const String & title_y="",
                        int storageOption=1)
  {
  return append(createNewHistogram(name,n_x,bins,title_x,title_y,storageOption));
  }

  //!
  //! Create 2D object
  //!
  TH2 * createHistogram(const String & name,
                        int n_x, double min_x, double max_x,
                        int n_y, double rapidity_minimum, double rapidity_maximum,
                        const String & title_x="",
                        const String & title_y="",
                        const String & title_z="",
                        int storageOption=1)
  {
  return append(createNewHistogram(name,n_x,min_x,max_x,n_y,rapidity_minimum,rapidity_maximum,title_x,title_y,title_z,storageOption));
  }

  //!
  //! Create 2D object
  //!
  TH2 * createHistogram(const String & name,
                        int n_x, double* xbins, int n_y, double rapidity_minimum, double rapidity_maximum,
                        const String & title_x="",
                        const String & title_y="",
                        const String & title_z="",
                        int storageOption=1)
  {
  return append(createNewHistogram(name,n_x,xbins,n_y,rapidity_minimum,rapidity_maximum,title_x,title_y,title_z,storageOption));
  }


  //!
  //! Create 3D object
  //!
  TH3 * createHistogram(const String & name,
                        int n_x, double min_x, double max_x,
                        int n_y, double rapidity_minimum, double rapidity_maximum,
                        int n_z, double z_min, double z_max,
                        const String & title_x="",
                        const String & title_y="",
                        const String & title_z="",
                        int storageOption=1)
  {
  return append(createNewHistogram(name,n_x,min_x,max_x,n_y,rapidity_minimum,rapidity_maximum,n_z,z_min,z_max,title_x,title_y,title_z,storageOption));
  }

  //!
  //! Create 1D profile object
  //!
  TProfile * createProfile(const String & name,
                           int n_x,double min_x,double max_x,
                           const String & title_x,
                           const String & title_y)
  {
  return append(createNewProfile(name,n_x,min_x,max_x,title_x,title_y));
  }

  //!
  //! Create 1D profile object
  //!
  TProfile * createProfile(const String & name,
                           int n_x,  double* bins,
                           const String & title_x,
                           const String & title_y)
  {
  return append(createNewProfile(name,n_x,bins,title_x,title_y));
  }

  //!
  //! Create 2D profile object
  //!Store the pointer
  //!
  TProfile2D * createProfile(const String & name,
                             int n_x, double min_x, double max_x,
                             int n_y, double min_y, double max_y,
                             const String & title_x,
                             const String & title_y,
                             const String & title_z)
  {
  return append(createNewProfile(name,n_x,min_x,max_x,n_y,min_y,max_y,title_x,title_y,title_z));
  }


  void addHistogramsToExtList(TList *g);

  inline bool sameSizeAs(const HistogramGroup & g) const
  {
  return size() == g.size();
  }

  inline bool sameSizeAs(const HistogramGroup & g1, const HistogramGroup & g2) const
  {
  Size_t n = size();
  return n==g1.size() && n==g2.size();
  }

  inline bool sameSizeAs(const HistogramGroup & g1, const HistogramGroup & g2, const HistogramGroup & g3) const
  {
  Size_t n = size();
  return n==g1.size() && n==g2.size() && n==g3.size();
  }

  inline bool sameSizeAs(const HistogramGroup & g1, const HistogramGroup & g2, const HistogramGroup & g3, const HistogramGroup & g4) const
  {
  Size_t n = size();
  return n==g1.size() && n==g2.size() && n==g3.size() && n==g4.size();
  }
  //
  void loadList(TFile & inputFile);

  virtual void sumHistogramGroups(double a1, const HistogramGroup & group1,
                                 HistogramGroup & sum);
  virtual void sumHistogramGroups(double a1, const HistogramGroup & group1,
                                 double a2, const HistogramGroup & group2,
                                 HistogramGroup & sum);
  virtual void sumHistogramGroups(double a1, const HistogramGroup & group1,
                                 double a2, const HistogramGroup & group2,
                                 double a3, const HistogramGroup & group3,
                                 HistogramGroup & sum);
  virtual void sumHistogramGroups(double a1, const HistogramGroup & group1,
                                 double a2, const HistogramGroup & group2,
                                 double a3, const HistogramGroup & group3,
                                 double a4, const HistogramGroup & group4,
                                 HistogramGroup & sum);

  virtual void divideHistogramsLists(double a1, const HistogramGroup & numerator,
                                     double a2, const HistogramGroup & denominator,
                                     HistogramGroup & ratio);



  void squareDifferenceLists(const HistogramGroup & group,
                             double sumWeights, double weight, int n);

  HistogramGroup * clone() const;


  void printIncompatibleLists(const String & caller, const HistogramGroup & group1, const HistogramGroup & group2);
  void printIncompatibleLists(const String & caller, const HistogramGroup & group1, const HistogramGroup & group2, const HistogramGroup & group3);
  void printIncompatibleLists(const String & caller, const HistogramGroup & group1, const HistogramGroup & group2, const HistogramGroup & group3, const HistogramGroup & group4);
  void printIncompatibleLists(const String & caller, const HistogramGroup & group1, const HistogramGroup & group2, const HistogramGroup & group3, const HistogramGroup & group4, const HistogramGroup & group5);

  void sumw2All();
  void scaleAllHistoByBinWidth(double scale);
  TH1 * importH1(TFile & inputFile,const String & histoName);
  TH2 * importH2(TFile & inputFile,const String & histoName);
  TH3 * importH3(TFile & inputFile, const String & histoName);
  TProfile * importProfile(TFile & inputFile, const String & histoName);
  TProfile2D * importProfile2D(TFile & inputFile, const String & histoName);
  void loadInList(TFile & inputFile, HistogramGroup * group);

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
