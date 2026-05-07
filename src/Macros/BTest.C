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

TH1 * cloneLoadH1(TFile & inputFile, TString histoName)
{
  TH1 * h = (TH1*) inputFile.Get(histoName);
  TH1 * clone = (TH1*) h->Clone();
  return clone;
}

TH1 * cloneLoadH2(TFile & inputFile, TString histoName)
{
  TH2 * h = (TH2*) inputFile.Get(histoName);
  TH2 * clone = (TH2*) h->Clone();
  return clone;
}

void printCR()
{
  cout << endl;
}

void printLine()
{
  cout << "============================================" << endl;
}

void printString(const TString &s)
{
  cout << s << endl;
}

void printValue(const TString &s, double v)
{
  cout << s << "       " << v << endl;
}


void partitionHistogram(const std::vector<double> fractions,
                        std::vector<double> boundaries,
                        TH1 * histogram,
                        double minimum=+1.0,
                        double maximum=-1.0)
{
  //  printCR();
  //  printLine();
  //  printString("partitionHistograms(...)");
  unsigned int nFractions = fractions.size();
  unsigned int nBoundaries = boundaries.size();
  printValue("partitionHistograms(...) nFractions",nFractions);
  printValue("partitionHistograms(...) nBoundaries",nBoundaries);
  //  if (nFractions!=nBoundaries)
  //    {
  //    printString("<E> partitionHistograms(...) nFractions!=nBoundaries");
  //    //throw Exception("<E> partitionHistograms(...) nFractions!=nBoundaries",__FUNCTION__);
  //    }
  TH1 * normalized = (TH1*) histogram->Clone();
  unsigned int first = 1;
  unsigned int last  = normalized->GetNbinsX();
  double sum, esum, esum2;
  double vk, evk;
  double vk1, evk1;
  if (minimum>=maximum) // use the whole histogram
    {
    sum = 0.0;
    for (unsigned int k=first; k<=last; k++)
      sum += normalized->GetBinContent(k);
    if (sum<=0.0)
      {
      printString("<E> partitionHistograms(...) sum<=0.0");
      //throw Exception("<E> partitionHistograms(...) sum<=0.0",__FUNCTION__);
      }
    printValue("partitionHistograms(...) sum",sum);

    normalized->Scale(1.0/sum);

    vk = normalized->GetBinContent(first);
    evk = normalized->GetBinError(first);
    sum = vk;
    esum2 = evk*evk;
    for (unsigned int k=first+1; k<=last; k++)
      {
      vk1 = normalized->GetBinContent(k);
      evk1 = normalized->GetBinError(k);
      sum += vk1;
      esum += evk1*evk1;
      normalized->SetBinContent(k,sum);
      normalized->SetBinError(k,sqrt(esum2));
      }
    normalized->Draw();
    for (auto & fraction : fractions)
      {
      printValue("Fraction:  ", fraction);
      bool ok = false;
      for (unsigned int k=first; k<=last; k++)
        {
        vk = normalized->GetBinContent(k);
        if (vk>=fraction && !ok)
          {
          double edge = normalized->GetXaxis()->GetBinLowEdge(k);
          cout << "k: " << k << " LOW:" << edge << " Frac:" << vk << endl;
          boundaries.push_back(edge);
          ok = true;
          }
        }
      }
    delete normalized;
    }
}


int BTest(const TString & fileName="/Volumes/ClaudeDisc4/OutputFiles/pythiaTest/GlobaleGen.root")
{
  TFile * file = new TFile(fileName,"OLD");
  TH1 * histo = cloneLoadH1(*file,"Global_ALL_ALL_n");
  //ROOT::RBrowser * b = new ROOT::RBrowser();

  histo->Draw();

  std::vector<double> fractions;
  std::vector<double> boundaries;
  fractions.push_back(0.0);
  fractions.push_back(0.2);
  fractions.push_back(0.4);
  fractions.push_back(0.6);
  fractions.push_back(0.8);
  fractions.push_back(1.0);

  partitionHistogram(fractions,boundaries,histo);


  return 0;
}

