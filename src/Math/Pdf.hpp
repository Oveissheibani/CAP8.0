#ifndef CAP_Pdf
#define CAP_Pdf
#include "MathConstants.hpp"
#include "MathBasicFunctions.hpp"

using namespace std;

//!
//! Compendium of basic and essential mathematical  functions
//!  By convention, all function names  begin with a lower case letter.

namespace CAP
{
namespace MATH
{

#define kMACHEP  1.11022302462515654042363166809e-16
/* largest argument for TMATH::Exp() */
#define kMAXLOG  709.782712893383973096206318587
/* smallest argument for TMATH::Exp() without underflow */
#define kMINLOG  -708.396418532264078748994506896
/* the maximal number that pow(x,x-0.5) has no overflow */
/* we use a (very) conservative portable bound          */
#define kMAXSTIR  108.116855767857671821730036754
#define kMAXLGM 2.556348e305

double binomial(int  n,int  k) ;
double binomialI(double p, int  n, int  k);
double binomialPdf(unsigned int k, double p, unsigned int n) ;
double binomialCdf(unsigned int k, double p, unsigned int n) ;
double binomialCdfC(unsigned int k, double p, unsigned int n) ;

double negativeBinomialPdf(unsigned int k, double p, double n)  ;
double negativeBinomialCdf(unsigned int k, double p, double n)  ;
double negativeBinomialCdfC(unsigned int k, double p, double n)  ;


double betaIncomplete(double  x, double  a, double  b) ;
double betaCf(double  x, double  a, double  b)   ;
double betaPdf(double x, double a, double b)     ;
double betaCdf(double x, double a, double b )    ;
double betaCdfC(double x, double a, double b )   ;

double Gamma(double  z) ;
double lnGamma(double  z) ;
double gammaStirling(double  z) ;
double gammaIncomplete(double alpha, double x) ;
double gammaIncompleteC(double alpha, double x) ;
double gammaPdf(double x, double alpha, double beta) ;
double gammaCdf(double x, double alpha, double beta)   ;
double gammaCdfC(double x, double alpha, double beta)   ;

double uniformPdf(double x, double a=0, double b=1, double x0=0) ;
double uniformCdf(double x, double a=0, double b=1, double x0=0) ;
double uniformCdfC(double x, double a=0, double b=1, double x0=0) ;

double exponentialPdf(double x, double lambda, double x0 = 0)   ;
double exponentialCdf(double x, double lambda, double x0 = 0)   ;
double exponentialCdfC(double x, double lambda, double x0 = 0)   ;

double erfInverse(double x) ;
double erfcInverse(double x) ;
double standardNormalPdf(double x);
double standardNormalCdf(double x);
double standardNormalCdfC(double x);
double normalPdf(double x, double mean, double sigma) ;
double normalCdf(double x, double mean, double sigma) ;
double normalCdfC(double x, double mean, double sigma) ;
double normQuantile(double p) ;
using  gaussianPDF = normalPdf;

double logNormalPdf(double x, double sigma, double theta=0, double m=1) ;
double logNormalCdf(double x, double sigma, double theta=0, double m=1) ;
double logNormalCdfC(double x, double sigma, double theta=0, double m=1) ;

double breitWignerPdf(double x, double mean=0, double gamma=1);
double breitWignerCdf(double x, double mean=0, double gamma=1);
double breitWignerCdfC(double x, double mean=0, double gamma=1);
double breitWignerRelativisticPdf(double x, double median=0, double gamma=1);
using  cauchyPdf  = breitWignerPdf;
using  cauchyCdf  = breitWignerCdf;
using  cauchyCdfC = breitWignerCdfC;

double chiSquarePdf(double x, double r, double x0 = 0) ;
double chiSquareCdf(double x, double r, double x0 = 0) ;
double chiSquareCdfC(double x, double r, double x0 = 0) ;
double chiSquareQuantile(double p, double ndf);

double landauPdf(double x, double xi=1.0, double x0=0.0) ;
double landauCdf(double x, double xi, double x0);
double landauXm1(double x, double xi, double x0);
double landauXm2(double x, double xi, double x0);

double poissonPdf(double x, double par) ;
double poissonI(double x, double par)  ;
double poissonCdf(unsigned int n, double mu) ;
double poissonCdfC(unsigned int n, double mu) ;

double fDistributionPdf(double x, double n, double m, double x0)  ;
double fDistributionCdf(double x, double n, double m, double x0)  ;
double fDistributionCdfC(double x, double n, double m, double x0)  ;

double KolmogorovProb(double z);
double KolmogorovTest(int  na, const double *a, int  nb, const double *b, const char *option);

double laplacePdf(double x, double alpha=0, double beta=1)  ;
double laplaceCdf(double x, double alpha=0, double beta=1)  ;
double laplaceCdfC(double x, double alpha=0, double beta=1)  ;
double laplaceQuantile(double F, double alpha=0, double beta=1)  ;

double studentPdf(double T, double ndf)  ;
double studentCdf(double x, double r, double x0)  ;
double studentCdfC(double x, double r, double x0)  ;
double studentQuantile(double p, double ndf, bool lower_tail=true)  ;

double vavilovPdf(double x, double kappa, double beta2);
double vavilovDenEval(double  rlam, double  *AC, double  *HC, int  itype);

double voigtPdf(double x, double sigma, double lg, int  r = 4);


void quantiles(int  n, int  nprob, double  *x, double  *quantiles, double  *prob, bool isSorted, int  *index, int  _type);

double crystalBallFct(double x, double alpha, double n, double sigma, double mean = 0) ;
double crystalBallPdf(double x, double alpha, double n, double sigma, double mean = 0)     ; 








 double fDistributionPdf(double x, double n, double m, double x0 = 0)



  
  double gammaPdf(double x, double alpha, double theta, double x0 = 0);



  double gaussianPdf(double x, double sigma = 1, double x0 = 0);

  using normalPdf = gaussianPdf;


   double biGaussianPdf(double x, double y, double sigmax = 1, double sigmay = 1, double rho = 0, double x0 = 0, double y0 = 0);



double landauPdf(double x, double xi = 1, double x0 = 0);

  double logNormalPdf(double x, double m, double s, double x0 = 0);


  double poissonPdf(unsigned int n, double mu);
  double tDistributionPdf(double x, double r, double x0 = 0);

  double uniformPdf(double x, double a, double b, double x0 = 0);

}

}

#endif // !CAP_Pdf
