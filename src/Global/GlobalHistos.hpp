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
#ifndef CAP__GlobalHistos
#define CAP__GlobalHistos
#include "HistogramGroup.hpp"
#include "ParticleFilter.hpp"

namespace CAP
{
class GlobalDerivedHistos;
class GlobalCalculator;
//!
//!
//! HistogramGroup of "global" observables.
//!
//! Observables are computed in the acceptance defined by
//! particle filters for each of the selected event filters. The particle filters can be set to select narrow
//! or broad ranges of kinematic variables as well as species
//! of particles.
//!
//! n:  number of selected particles in an  event
//! e:  total energy of the selected particles in an event
//! q:  total (net) electric charge of the selected particles in an event
//! b:  total (net) baryon charge of the selected particles in an event
//!
//! The fill method of this class must be called at most once per event
//! otherwise weird multiple counting will happen..
//!
class GlobalHistos : public HistogramGroup
{
public:

  friend GlobalDerivedHistos;
  friend GlobalCalculator;

  GlobalHistos();

  GlobalHistos(const GlobalHistos & group);

  GlobalHistos & operator=(const GlobalHistos & group);

  virtual ~GlobalHistos() {}

  virtual void configure(const String & taskName,
                               const String & objectType,
                               const Configuration & configuration,
                               unsigned int index);
  //!
  //!Creating histograms corresponding to the selected event and particle filters from the given file.
  //!
  virtual void create();

  //!
  //!Load histograms corresponding to the selected event and particle filters from the given file.
 virtual void loadFrom(TFile & inputFile);

  //!
  //!Fill eventswise observables for the selected filters
  //!
  //! @param n  number of particles accepted
  //! @param pTsum total transverse momentum (pT) of  accepted particles
  //! @param e  total energyof  accepted particles
  //! @param q  net charge of  accepted particles
  //! @param s  net strangeness of  accepted particles
  //! @param b net baryon number of  accepted particles
  //!
  //! By default, only histograms of single particle quantities are filled. These include the number of particles (multiplicity), the pt sum,
  //! the total energy, the net charge, the net strangeness, and the net baryon number. Scatter plot histograms of observable correlations
  //! are also filled including for instance ptSum vs multiplicity, total E vs multiplicity, net charge vs multiplicity, and so on. An eventwise  pt average
  //! histogram is also filled (event by event), for all event and particle filters, when the selected mulitplicity in non zero.
  //!
  //! If the bool variable fillCorrelationHistos is set to true, cross-species histograms are also filled including for instance the multiplicity selected by
  //! particle filter k1 and the multiplicity selected by filter k2.
  //!
  void fill(double n,
            double ptSum,
            double e,
            double q,
            double s,
            double b,
            double weight);

protected:
  void cloneB(const GlobalHistos & group);
  TH1 * h_n;
  TH1 * h_e;
  TH1 * h_q;
  TH1 * h_s;
  TH1 * h_b;
  TH1 * h_ptSum;
  TH1 * h_ptAvg;

  TProfile * p_eVsN;
  TProfile * p_qVsN;
  TProfile * p_sVsN;
  TProfile * p_bVsN;
  TProfile * p_ptSumVsN;
  TProfile * p_ptAvgVsN;

  int n_nbins;
  int e_nbins;
  int q_nbins;
  int b_nbins;
  int ptSum_nbins;
  int s_nbins;
  int ptAvg_nbins;
  double n_min;
  double n_max;
  double e_min;
  double e_max;
  double q_min;
  double q_max;
  double b_min;
  double b_max;
  double s_min;
  double s_max;
  double ptSum_min;
  double ptSum_max;
  double ptAvg_min;
  double ptAvg_max;


  ClassDef(GlobalHistos,0)
};

} // namespace CAP

#endif /* CAP__GlobalHistos  */



