//!
//! nn xSection vs sqrt{s}
//! sqrtS [GeV]
//! nnXset [mb]
//!
//! From fits published in
//! Improved Monte Carlo Glauber predictions at present and future nuclear colliders
//! Constantin Loizides (ORNL), Jason Kamin (UiC), David d'Enterria (CERN)
//! Phys.Rev. C97 (2018) 054910; Erratum: Phys.Rev. C99 (2019) 019901
//! Parameters are from Table 1.
//!
//! s_nn = A + B log^n(s)
//!
//! Type    |    A                  |          B                 |     n                  |   chi^2/ndf
//! ------------------------------------------------
//! s_nn = A + B log^n(s)
//! ln(s)     | -3.33 +- 1/58  |  4.195 +- 0.103    |     1  (fixed)      |   1/.52
//! ln^2(s) | 25.0 +- 0.9      |  0.146 +- 0.004    |     2  (fixed)      | 0.97
//! ln^n(s) | 29.8 +- 4.7      |  0.038 +- 0.060     |    2.43 +- 0.50 | 0.98
//!
//!  s_nn  = A+B log^2(s) + C s^{-eta}
//!  A = 24.4\pm 1.4
//!  B = 0.01008 +- 0.1537
//!  C = 0.1.454 +- 1.768
//!  eta = 0.131 +- 0.0180
//!  chi^2/ndf  1.09
//!
//!  s_nn  A+B log^2(s)+D log(s)
//!  A = 39.7 +- 1.4
//!  B = 0.2212 +-0.0708
//!  D = -2.154 +- 2.035
//!  chi^2/ndf = 0.96
//!
TF1 * getNNXSectVsSqrtS(int option=0)
{
  double sqrtSMin = 1.0E2;
  double sqrtSMax = 1.0E5;
  TF1 * f;
  switch (option)
  {
  case 1: // exponent n == 1
  {
  double A = -3.33;
  double B = 4.195;
  f = new TF1("NNXSectVsS1","[0]+[1]*TMath::Log(x*x)",sqrtSMin,sqrtSMax);
  f->SetParameters(A,B);
  f->SetNpx(1000);
  break;
  }
  case 2: // exponent n == 2
  {
  double A = 25.0;
  double B = 0.146;
  f = new TF1("NNXSectVsS2","[0]+[1]*TMath::Log(x*x)*TMath::Log(x*x)",sqrtSMin,sqrtSMax);
  f->SetParameters(A,B);
  f->SetNpx(1000);
  break;
  }
  default:
  case 3: // exponent n == 2.43
  {
  double A = 29.8;
  double B = 0.038;
  double n = 2.43;
  f = new TF1("NNXSectVsS3","[0]+[1]*TMath::Power(TMath::Log(x*x),[2])",sqrtSMin,sqrtSMax);
  f->SetParameters(A,B,n);
  f->SetNpx(1000);
  break;
  }
  case 4: // Compete collaboration
  {
    //!  s_nn  = A+B log^2(s) + C s^{-eta}
  //!  A = 24.4\pm 1.4
  //!  B = 0.01008 +- 0.1537
  //!  C = 0.1.454 +- 1.768
  //!  eta = 0.131 +- 0.0180
  //!  chi^2/ndf  1.09
  double A = 24.4;
  double B = 0.01008;
  double C = 1.454;
  double eta = 0.131;
  f = new TF1("NNXSectVsS4","[0]+[1]*TMath::Power(TMath::Log(x*x),2.0)+[2]*TMath::Power(x*x,-[3])",sqrtSMin,sqrtSMax);
  f->SetParameters(A,B,C,eta);
  f->SetNpx(1000);
  break;
  }
  case 5: // Compete collaboation
  {
  //!  s_nn  = A+B log^2(s)+D log(s)
  //!  A = 39.7 +- 1.4
  //!  B = 0.2212 +-0.0708
  //!  D = -2.154 +- 2.035
  double A = 39.7;
  double B = 0.2212;
  double D = -2.154;
  f = new TF1("NNXSectVsS5","[0]+[1]*TMath::Power(TMath::Log(x*x),2.0)+[2]*TMath::Log(x*x)",sqrtSMin,sqrtSMax);
  f->SetParameters(A,B,D);
  f->SetNpx(1000);
  break;
  }
  }
  return f;
}


int test()
{
  TCanvas * c = new TCanvas();
  c->SetLogx(1);
  TF1 * f1 = getNNXSectVsSqrtS(1);
  f1->SetLineColor(1);
  f1->Draw();

  TF1 * f2 = getNNXSectVsSqrtS(2);
  f2->SetLineColor(2);
  f2->Draw("SAME");

  TF1 * f3 = getNNXSectVsSqrtS(3);
  f3->SetLineColor(3);
  f3->Draw("SAME");

  TF1 * f4 = getNNXSectVsSqrtS(4);
  f4->SetLineColor(4);
  f4->Draw("SAME");

  TF1 * f5 = getNNXSectVsSqrtS(5);
  f5->SetLineColor(5);
  f5->Draw("SAME");

  return 0;
}
