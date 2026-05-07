#ifndef CAP_RootHistogramHelpers
#define CAP_RootHistogramHelpers
#include "Aliases.hpp"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TLine.h"
#include "TArrow.h"
#include "TLatex.h"

namespace CAP
{
  
    // histogram helpers
  TH1 * createNewHistogram(const String & name,
                           int n, double min_x, double max_x,
                           const String & title_x="x",
                           const String & title_y="N",
                           int storageOption=1,
                           bool verbose=false);
  TH1 * createNewHistogram(const String & name,
                           int n, double * bins,
                           const String & title_x="x",
                           const String & title_y="N",
                           int storageOption=1,
                           bool verbose=false);
  TH2 * createNewHistogram(const String & name,
                           int n_x, double min_x, double max_x,
                           int n_y, double rapidity_minimum, double rapidity_maximum,
                           const String & title_x="x",
                           const String & title_y="y",
                           const String & title_z="N",
                           int storageOption=1,
                           bool verbose=false);
  TH2 * createNewHistogram(const String & name,
                           int n_x, double* xbins, int n_y, double rapidity_minimum, double rapidity_maximum,
                           const String & title_x="x",
                           const String & title_y="y",
                           const String & title_z="N",
                           int storageOption=1,
                           bool verbose=false);
  TH3 * createNewHistogram(const String & name,
                           int n_x, double min_x, double max_x,
                           int n_y, double rapidity_minimum, double rapidity_maximum,
                           int n_z, double z_min, double z_max,
                           const String & title_x="x",
                           const String & title_y="y",
                           const String & title_z="z",
                           const String & title_w="N",
                           int storageOption=1,
                           bool verbose=false);
  TProfile * createNewProfile(const String & _name,
                              int n_x,double min_x,double max_x,
                              const String & title_x,
                              const String & title_y,
                              bool verbose=false);
  
  TProfile * createNewProfile(const String & name,
                              int n_x,  double* bins,
                              const String & title_x,
                              const String & title_y,
                              bool verbose=false);
  
  TProfile2D * createNewProfile(const String & title,
                                int n_x, double min_x, double max_x,
                                int n_y, double rapidity_minimum, double rapidity_maximum,
                                const String & title_x,
                                const String & title_y,
                                const String & title_z,
                                bool verbose=false);
  
  void sumHistograms(double a1, const TH1 * h1,
                     TH1 * sum);
  void sumHistograms(double a1, const TH1 * h1,
                     double a2, const TH1 * h2,
                     TH1 * sum);
  void sumHistograms(double a1, const TH1 * h1,
                     double a2, const TH1 * h2,
                     double a3, const TH1 * h3,
                     TH1 * sum);
  void sumHistograms(double a1, const TH1 * h1,
                     double a2, const TH1 * h2,
                     double a3, const TH1 * h3,
                     double a4, const TH1 * h4,
                     TH1 * sum);
  
  void sumHistograms(std::vector<double> a, std::vector<double> histograms,
                     TH1 * sum);
  
  void divideHistograms(double a1, const TH1 * h1,
                        double a2, const TH1 * h2,
                        TH1 * ratio);
  void differenceHistograms(const TH1 * h1,
                            const TH1 * h2,
                            TH1 * difference);
  
  double calculateN1N1(const TH1 * h_1, const TH1 * h_2, TH1 * h_12, double a1, double a2);
  double calculateN1N1_H1H1H2(const TH1 * h_1, const TH1 * h_2, TH2 * h_12, double a1, double a2);
  double calculateN1N1_H2H2H2(const TH2 *h_1, const TH2 * h_2, TH2 * h_12, double a1, double a2);
  double calculateN1N1N1(const TH1 * h_1, const TH1 * h_2, const TH1 * h_3, TH1 * h_123);
  double calculateN1N1N1(const TH1 * h_1, const TH1 * h_2, const TH1 * h_3, TH3 * h_123);
  void calculateN2N1(const TH2 * s2, const TH1* s1, TH2 * target, int single);
  void calculateN2N1x(const TH2 * s2, const TH1* s1, TH3 * target, int single);
  
  void calculateDptDpt(const TH2 * spp, const TH2 * spn, const TH2 * snp, const TH2 * snn,
                       const TH2 * avgp1, const TH2 * avgp2,  TH2 * s2dptdpt,  TH2 * dptdpt,
                       bool ijNormalization, int nEta, int nPhi);
  void calculateDptDpt(const TH2 * spp, const TH2 * spn, const TH2 * snp, const TH2 * snn,
                       const TH1 * avgp1, const TH1 * avgp2,
                       TH2 * dptdpt,
                       bool ijNormalization, int nBins);
  void calculateSc(const TH1 * spp, const TH1 * n1n1, const TH1 * pt1pt1, TH1 * g2, bool ijNormalization);
  
  void calculateG2_H1H2H2H2(const TH1 * spp, const TH2 * n1n1, const TH2 * pt1pt1, TH2 * g2, bool ijNormalization, double a1, double a2);
  void calculateG2_H2H2H2H2(const TH2 * spp, const TH2 * n1n1, const TH2 * pt1pt1, TH2 * g2, bool ijNormalization, double a1, double a2);
  void calculateBf(const TH2 *n2, const TH2 *n1_1, const TH2 *n1_2, TH2 *bf_12, TH2 *bf_21);
  void HistogramG2_H1H2H2H2(const TH1 * spp, const TH2 * n1n1, const TH2 * pt1pt1, TH2 * g2, bool ijNormalization, double a1, double a2);
  int  calculateQ3DwPtPhiEta(double pt1, double phi1, double eta1,
                             double pt2, double phi2, double eta2,
                             double & Qlong, double & Qout, double & Qside);
  int  calculateQ3DwPtPhiY(double pt1, double phi1, double y1,
                           double pt2, double phi2, double y2,
                           double & Qlong, double & Qout, double & Qside);
  void calculateN1N1H2H2_Q3D_MCY(TH2 * n1_1, TH2 * n1_2, TH3 * n1n1_Q3D, double a1, double a2);
  void calculateN1N1H2H2_Q3D_MCEta(TH2 * n1_1, TH2 * n1_2, TH3 * n1n1_Q3D, double a1, double a2);
  void calculateN1N1H2H2_Q3D(const TH2 * n1_1, const TH2 * n1_2, TH3 * n1n1_Q3D, double a1, double a2);
  void calculateN1N1H3H3_Q3D(const TH3 * n1_1, const TH3 * n1_2, TH3 * n1n1_Q3D, double a1, double a2);
  void calculateR2_Q3D(const TH3 * n2_Q3D, const TH3 * n1n1_Q3D, TH3 * R2_Q3D, double a1, double a2);
  double avgValue(TH1 * h);
  
  void setHistogram(TH1 * h, double v, double ev);
  void setHistogram(TH2 * h, double v, double ev);
  void setHistogram(TH3 * h, double v, double ev);
  
  
  TH1 * loadH1(TFile & inputFile,const String & histoName);
  TH2 * loadH2(TFile & inputFile,const String & histoName);
  TH3 * loadH3(TFile & inputFile,const String & histoName);
  TProfile * loadProfile(TFile & inputFile,const String & histoName) ;
  TProfile2D * loadProfile2D(TFile & inputFile,const String & histoName) ;
  TH1 * clone(const TH1 * h1, const String & histoName)  ;
  
  TH1 * safeCloneH1(const TH1 * h);
  TH2 * safeCloneH2(const TH2 * h);
  TH3 * safeCloneH3(const TH3 * h);
  TProfile * safeCloneProfile(const TProfile * h);
  TProfile2D * safeCloneProfile2D(const TProfile2D * h);
  
  void cloneHistoVector(const std::vector<TH1*> & source, std::vector<TH1*> & target);
  void cloneHistoVector(const std::vector<TH2*> & source, std::vector<TH2*> & target);
  void cloneHistoVector(const std::vector<TH3*> & source, std::vector<TH3*> & target);
  void cloneHistoVector(const std::vector<TProfile*> & source, std::vector<TProfile*> & target);
  void cloneHistoVector(const std::vector<TProfile2D*> & source, std::vector<TProfile2D*> & target);
  
  
  double findHistoMinimum(TH1 * h);
  double findHistoMaximum(TH1 * h);
  double findHistoMinimum2D(TH2 * h);
  double findHistoMaximum2D(TH2 * h);
  double findGraphMinimum(TGraph * h);
  double findGraphMaximum(TGraph * h);
  double findMinimum(std::vector<double> & values);
  double findMaximum(std::vector<double> & values);
  
  
  void findMinimum(TH1 * h, int & xMinValueBin, double & minValue, int xFirstBin=1, int xLastBin=-1);
  void findMaximum(TH1 * h, int & xMaxValueBin, double & maxValue, int xFirstBin=1, int xLastBin=-1);
  void findMinMax (TH1 * h,
                   int & xMinValueBin, double & minValue,
                   int & xMaxValueBin, double & maxValue,
                   int xFirstBin=1, int xLastBin=-1);
  void findMinMax (std::vector<TH1*> histograms,
                   int & xMinValueBin, double & minValue,
                   int & xMaxValueBin, double & maxValue,
                   int xFirstBin=1, int xLastBin=-1);
  
  
  
  void findMinimum(TH2 * h, int & xMinValueBin, int & yMinValueBin, double & minValue, int xFirstBin=1, int xLastBin=-1, int yFirstBin=1, int yLastBin=-1);
  void findMaximum(TH2 * h, int & xMaxValueBin, int & yMaxValueBin, double & maxValue, int xFirstBin=1, int xLastBin=-1, int yFirstBin=1, int yLastBin=-1);
  void findMinMax (TH2 * h,
                   int & xMinValueBin, int & yMinValueBin, double & minValue,
                   int & xMaxValueBin, int & yMaxValueBin, double & maxValue,
                   int xFirstBin=1, int xLastBin=-1,
                   int yFirstBin=1, int yLastBin=-1);
  void findMinMax (std::vector<TH2*> histograms,
                   int & xMinValueBin, int & yMinValueBin, double & minValue,
                   int & xMaxValueBin, int & yMaxValueBin, double & maxValue,
                   int xFirstBin=1, int xLastBin=-1,
                   int yFirstBin=1, int yLastBin=-1);
  void findMinMax(std::vector<double> & values, double & minimum, double & maximum);
  
  
  void scaleByBinWidth1D(TH1 * h, double scale);
  void scaleByBinWidth2D(TH2 * h, double scale);
  void scaleByBinWidth3D(TH3 * h, double scale);
  void scaleByBinWidth(TH1 * h, double scale);
  void scaleAllHistoByBinWidth(double scale);
  void sumw2All();
  void unpack_vsXY_to_vsXVsY(const TH1 * source, TH2 * target);
  void correctMerging(TH1 * h, int nEta, int nPhi, bool reverse);
  
  void calculateC2_H1H1H1(const TH1 * n2_12,
                          const TH1 * n1n1_12,
                          TH1 * c2_12,
                          double a1=1.0, double a2=1.0);
  void calculateC2_H2H2H2(const TH2 * n2_12,
                          const TH2 * n1n1_12,
                          TH2 * c2_12,
                          double a1=1.0, double a2=1.0);
  void calculateC2_H3H3H3(const TH3 * n2_12,
                          const TH3 * n1n1_12,
                          TH3 * c2_12,
                          double a1=1.0, double a2=1.0);
  
  
  void calculateR2_H1H1H1(const TH1 * n2_12, const TH1 * n1n1_12, TH1 * r2_12, bool ijNormalization=0, double a1=1.0, double a2=1.0);
  void calculateR2_H2H2H2(const TH2 * n2_12, const TH2 * n1n1_12, TH2 * r2_12, bool ijNormalization=0, double a1=1.0, double a2=1.0);
  void calculateR2_H3H3H3(const TH3 * n2_12, const TH3 * n1n1_12, TH3 * r2_12, bool ijNormalization=0, double a1=1.0, double a2=1.0);
  void calculateR2_H1H2H2(const TH1 * n2_12, const TH2 * n1n1_12, TH2 * r2_12, bool ijNormalization=0, double a1=1.0, double a2=1.0);
  void calculateAverageVsDeta(const TH2 * obs_12, TH2 * avgObs_12, int n);
  void calculateR2VsM(const TProfile * h1, const TProfile * h2, const TProfile * h12, TH1 * r2VsM, TH1 * intR2, bool sameFilter);
  void calculateBinCorr(const TProfile * h1, const TProfile * h2, TH1 * intBinCorrVsM1, bool sameFilter);
  void calculateInclusiveAvg(const TH1 * h1, const TH1 * h2, TH1 * h3);
  void calculateInclusiveAvg(const TProfile * h1, const TProfile * h2, TH1 * h3);
  void calculateInclusiveAvgH2(const TH2 * h1, const TH2 * h2, TH2 * h3);
  void calculateInclusiveAvgH1H2(const TH1 * h1, const TH1 * h2, TH2 * h3);
  void symmetrize3D(TH3* h);
  void symmetrizeDeltaEtaDeltaPhi(TH2 * h, bool ijNormalization);
  void symmetrizeXX(TH2 * h, bool ijNormalization);
  void reduce_n2xEtaPhi_n2DetaDphi(const TH2 * source, TH2 * target,int nEtaBins,int nPhiBins);
  void reduce_n2xEtaPhi_n2EtaEta(const TH1 * source, TH2 * target,int nEtaBins,int nPhiBins);
  void reduce_n1EtaPhiN1EtaPhiOntoN1N1DetaDphi(const TH2 * h_1, TH2 * h_2, TH2 * h_12,int nDeta,int nDphi);
  void calculateAverage(TH1* h, double & avgDensity, double & eAvgDensity);
  void calculateIntegral(TH1 * h, double xMin, double xMax, double & result, double & resultError, int option);
  void calculateIntegral(TH2 * h, double xMin, double xMax, double yMin, double yMax, double & result, double & resultError, int option);
  
  void calculateAvgH2H2(const TH2 * h_1, TH2 * h_2, double scaleFactor);
  
  void project_n2XYXY_n2XX(const TH2 * source, TH2 * target,int nXBins,int nYBins);
  void project_n2XYXY_n2YY(const TH2 * source, TH2 * target,int nXBins,int nYBins);
  
  void shiftY(const TH2 & source, TH2 & target, int nbins);
  float * floatArray(unsigned int size, float v);
  double * getDoubleArray(unsigned int size, double v);
  void resetDoubleArray(int n, double * array, double value);
  
  int index2(int i1, int i2);
  int index3(int i1, int i2, int i3);
  int index4(int i1, int i2, int i3, int i4);
  
  void calculateF1(TH1* h_f1_1, TH1* h_F1_1);
  void calculateF2R2(TH1* h_f1_1, TH1* h_f1_2, TH1* h_f2_12, TH1* h_F2_12, TH1* h_R2_12);
  void calculateNudyn(TH1* h_R2_11,
                      TH1* h_R2_12,
                      TH1* h_R2_21,
                      TH1* h_R2_22,
                      TH1* h_nudyn_12);
  void calculateF3R3(TH1* h_f1_1, TH1* h_f1_2, TH1* h_f1_3,
                     TH1* h_f2_12, TH1* h_f2_13, TH1* h_f2_23,
                     TH1* h_f3_123,
                     TH1* h_F3_123, TH1* h_R2_123);
  void calculateF4R4(TH1* h_f1_1, TH1* h_f1_2, TH1* h_f1_3, TH1* h_f1_4,
                     TH1* h_f2_12, TH1* h_f2_13, TH1* h_f2_14, TH1* h_f2_23, TH1* h_f2_24, TH1* h_f2_34,
                     TH1* h_f3_123, TH1* h_f3_124, TH1* h_f3_134, TH1* h_f3_234,
                     TH1* h_f4_1234,
                     TH1* h_F4_1234, TH1* h_R4_1234);
  void calculateF2R2(double f1_1,double ef1_1,double f1_2,double ef1_2, double f2_12,double ef2_12,double & F2_12,double & eF2_12, double & R2_12,double & eR2_12);
  void calculateF3R3(double f1_1,double ef1_1,double f1_2,double ef1_2, double f1_3,double ef1_3,
                     double f2_12,double ef2_12,double f2_13,double ef2_13,double f2_23,double ef2_23,
                     double f3_123, double ef3_123,
                     double & F3_123,double & eF3_123, double & R3_123,double & eR3_123);
  void calculateF4R4(double f1_1,double ef1_1,double f1_2,double ef1_2, double f1_3,double ef1_3, double f1_4,double ef1_4,
                     double f2_12,double ef2_12,double f2_13,double ef2_13,double f2_14,double ef2_14,double f2_23,double ef2_23,
                     double f2_24,double ef2_24,double f2_34,double ef2_34,
                     double f3_123, double ef3_123,double f3_124, double ef3_124, double f3_134, double ef3_134,double f3_234, double ef3_234,
                     double f4_1234, double ef4_1234,
                     double & F4_1234,double & eF4_1234, double & R4_1234,double & eR4_1234);
  void calculateNudyn(double r2_11,double er2_11,
                      double r2_12,double er2_12,
                      double r2_21,double er2_21,
                      double r2_22,double er2_22,
                      double & nudyn,double & enudyn);
  

  void setTitles(TH1 * histogram,
                 const String & title_x,
                 const String & title_y);
  void setTitles(TH2 * histogram,
                 const String & title_x,
                 const String & title_y,
                 const String & title_z);
  void setTitles(TH3 * histogram,
                 const String & title_x,
                 const String & title_y,
                 const String & title_z);
  void setTitles(TH3 * histogram,
                 const String & title_x,
                 const String & title_y,
                 const String & title_z,
                 const String & title_w);
  
  
  
    //!
    //! Span all bins of hAvg and h and  compute the difference between histo "h" and the reference "havg", and increment the value
    //! havg accordingly. HistogramGroup must have the same exact dimensions. The histograms may be profiles.
    //!
    //!@param hAvg Histogram containing the running average
    //!@param h  Histogram to be compared to hAvg and use to update the average and the uncertainties.
    //!@param sumWeights Cumulated sum of weights
    //!@param weight Weight given to histogram h.
    //!@param n Index of file being processed.
    //!
  void squareDifferenceHistos(TH1 * hAvg, TH1 * h, double sumWeights, double weight, int n);
  
    //!
    //!Span all bins of h and href and calculate the content difference (and the uncertainty) and store the difference in hDiff.
    //!All three given histograms must have the same exact dimensions. The histogram h and hRef may be profiles.
    //!Uncertainties on the difference hDiff are computed as sum of square of uncertainties on h and href if correlatedUncertainties==0 and
    //!as a difference of square of uncertainties if correlatedUncertainties==1.
    //!
    //!@param h Histogram to be tested against the reference Histogram
    //!@param hRef Reference Histogram
    //!@param hDiff Difference histogram produced on output
    //!@param correlatedUncertainties Whether the uncertainties of h and href are correlated: true if they are, false if they are not.
    //!
  void differenceHistos(TH1 *h, TH1 *hRef, TH1 *hDiff, bool correlatedUncertainties);
  
    //!
    //!Span all bins of h and href and calculate the content ratio (and the uncertainty) and store the ratio in hRatio.
    //!All three given histograms must have the same exact dimensions. The histogram h and hRef may be profiles.
    //!Uncertainties on the difference hRatio are computed as sum of square of uncertainties on h and href if correlatedUncertainties==0 and
    //!as a difference of square of uncertainties if correlatedUncertainties==1.
    //!
    //!@param h Histogram to be tested against the reference Histogram
    //!@param hRef Reference Histogram
    //!@param hDiff Ratio histogram produced on output
    //!@param correlatedUncertainties Whether the uncertainties of h and href are correlated: true if they are, false if they are not.
    //!
  void ratioHistos(TH1 *h, TH1 *hRef, TH1 *hDiff, bool correlatedUncertainties);
  
  int  getDimension(const TH1* h);
  
  void printDimensions(const TH1*h);
  void printDimensions(const TH2*h);
  void printDimensions(const TH3*h);
  
  bool sameDimensions(const String & caller, const TH1* h1, const TH1* h2);
  bool sameDimensions(const String & caller, const TH1* h1, const TH1* h2, const TH1* h3);
  bool sameDimensions(const String & caller, const TH1* h1, const TH1* h2, const TH1* h3, const TH1* h4);
  bool sameDimensions(const String & caller, const TH1* h1, const TH1* h2, const TH1* h3, const TH1* h4, const TH1* h5);
  
  bool ptrFileExist(const String & caller, const TFile * f);
  bool ptrExist(const String & caller, const TH1 * h1);
  bool ptrExist(const String & caller, const TH1 * h1, const TH1 * h2);
  bool ptrExist(const String & caller, const TH1 * h1, const TH1 * h2, const TH1 * h3);
  bool ptrExist(const String & caller, const TH1 * h1, const TH1 * h2, const TH1 * h3, const TH1 * h4);
  bool ptrExist(const String & caller, const TH1 * h1, const TH1 * h2, const TH1 * h3, const TH1 * h4, const TH1 * h5);
  bool ptrExist(const String & caller, const TH1 * h1, const TH1 * h2, const TH1 * h3, const TH1 * h4, const TH1 * h5, const TH1 * h6);
  bool ptrExist(const String & caller, const TH1 * h1, const TH1 * h2, const TH1 * h3, const TH1 * h4, const TH1 * h5, const TH1 * h6, const TH1 * h7);
  bool ptrExist(const String & caller, const TH1 * h1, const TH1 * h2, const TH1 * h3, const TH1 * h4, const TH1 * h5, const TH1 * h6, const TH1 * h7, const TH1 * h8);
  bool ptrExist(const String & caller, const TH1 * h1, const TH1 * h2, const TH1 * h3, const TH1 * h4, const TH1 * h5, const TH1 * h6, const TH1 * h7, const TH1 * h8, const TH1 * h9);
  bool ptrExist(const String & caller, const TH1 * h1, const TH1 * h2, const TH1 * h3, const TH1 * h4, const TH1 * h5, const TH1 * h6, const TH1 * h7, const TH1 * h8, const TH1 * h9, const TH1 * h10);
  bool ptrExist(const String & caller, const TH1 * h1, const TH1 * h2, const TH1 * h3, const TH1 * h4, const TH1 * h5, const TH1 * h6, const TH1 * h7, const TH1 * h8, const TH1 * h9, const TH1 * h10, const TH1 * h11);
  bool ptrExist(const String & caller, const TH1 * h1, const TH1 * h2, const TH1 * h3, const TH1 * h4, const TH1 * h5, const TH1 * h6, const TH1 * h7, const TH1 * h8, const TH1 * h9, const TH1 * h10, const TH1 * h11, const TH1 * h12);
  
  
  TLatex  * createLabel(const String & text, double x, double y, double angle, int color,  double fontSize, bool doDraw=true);
  TLegend * createLegend(double x1, double y1, double x2, double y2, double fontSize);
  TLegend * createLegend(TH1*histogram, const String & legendText, double x1, double y1, double x2, double y2, double fontSize, bool doDraw=true);
  TLegend * createLegend(std::vector<TH1*> & histograms,double x1, double x2, double y1, double y2, double fontSize, bool doDraw=true);
  TLegend * createLegend(std::vector<TH1*> & histograms, std::vector<TString> & legendTexts,double x1, double x2, double y1, double y2, double fontSize, bool doDraw=true);
  TLegend * createLegend(std::vector<TGraph*> graphs, std::vector<TString> & legendTexts,double x1, double x2, double y1, double y2, double fontSize, bool doDraw=true);
  
  TLine   * createLine(float x1, float y1, float x2, float y2, int style, int color, int width, bool doDraw=true);
  TArrow  * createArrow(float x1, float y1, float x2, float y2,
                        float arrowSize, Option_t* option,
                        int style, int color, int width, bool doDraw=true);
  
  TGraph * makeGraph(std::vector<double> vx,
                     std::vector<double> vex,
                     std::vector<double> vy,
                     std::vector<double> vey);
  
  TGraph * sumGraphs(TGraph * g1, TGraph * g2);
  
  TGraph* calculateCumulativeIntegral(TH1 & h,
                                      double xLow,
                                      double xHigh,
                                      double xStep);
  
  void calculateRmsWidth(TH1 * h,
                         double xLowEdge,   double xHighEdge,
                         double & mean,     double & meanError,
                         double & rmsWidth, double & rmsWidthError);
  
  void setLimits(TH1 & h, double xMin, double xMax, double yMin, double yMax);
  void setLimits(TH2 & h, double xMin, double xMax, double yMin, double yMax, double zMin, double zMax);
  
  void setLimits(const std::vector<TH1*> & histograms,double xMin, double xMax, double yMin, double yMax);
  void setLimits(const std::vector<TH2*> & histograms,double xMin, double xMax, double yMin, double yMax, double zMin, double zMax);
  
  double gaussian(double *x, double *par);
  
  //double GeneralizedGaussian(double *x, double *par);
  
  void partitionHistogram(const std::vector<double> fractions,
                          std::vector<double> boundaries,
                          TH1 * histogram,
                          double minimum=+1.0,
                          double maximum=-1.0);
  
  void printHistoInfo(TH1*h);

  void printHistoInfo(const String & source,
                      const String & histoName,
                      int x_nbins=-1, double x_min=-1, double x_max=-1,
                      int y_nbins=-1, double y_min=-1, double y_max=-1,
                      int z_nbins=-1, double z_min=-1, double z_max=-1);

} // namespace CAP

#endif
