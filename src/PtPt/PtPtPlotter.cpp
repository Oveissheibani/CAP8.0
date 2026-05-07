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
#include "PtPtPlotter.hpp"
#include "PrintHelpers.hpp"
#include "RootHistogramHelpers.hpp"
#include "NameHelpers.hpp"

using CAP::PtPtPlotter;

ClassImp(PtPtPlotter);

PtPtPlotter::PtPtPlotter()
:
Plotter("PtPtPlotter",Info)
{  }

PtPtPlotter::~PtPtPlotter()
{
}

void PtPtPlotter::makePlots(const String & canvasNameBase,
                             PtPtHistos * PtPtHistos,
                             PtPtDerivedHistos * PtPtDerivedHistos,
                             CanvasConfiguration & canvasConfiguration,
                             GraphConfiguration  ** graphConfigurations)
{
  TH1 ** histograms = new TH1* [100];
  String ** histTitles = new String * [100];

  String histName;
  String histTitle;

  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  //canvasConfiguration->setAxes(CanvasConfiguration::LogY);

  int nPart = 4;
  int nGraphs = 0;

  histograms[0] = PtPtHistos->h_eventStreams_evt;     histTitles[0] = new String("N_{eventStreams}");
  plot(1, canvasNameBase+"eventStreams_evt",canvasConfiguration,graphConfigurations,
       "n", 0.0, 0.0,
       "Counts", 0.0, -1.2,
       histograms,histTitles,0.7, 0.5, 0.9, 0.9, 0.055);

  nGraphs = 0;
  for (int i1=0; i1<nPart; i1++)
  {
  histTitle = "f_{1}";
  histTitle += "^{";
  histTitle += i1;
  histTitle += "}";
  cout << "Inserting: " << histTitle << endl;
  histograms[i1] = PtPtHistos->h_f1_evt[i1]; histTitles[i1] = new String(histTitle);
  nGraphs++;
  }
  plot(nGraphs, canvasNameBase+"f1_evt",canvasConfiguration,graphConfigurations,
       "Mult", 20.0, 400.0,
       "f_{1}", 0.0, -1.2,
       histograms,histTitles,0.75, 0.7, 0.85, 0.8, 0.055);

  nGraphs = 0;
  for (int i1=0; i1<nPart; i1++)
    {
    for (int i2=i1; i2<nPart; i2++)
      {
      int index12 = PtPtHistos->index2(i1,i2);
      histTitle = "f_{2}";
      histTitle += "^{";
      histTitle += i1;
      histTitle += i2;
      histTitle += "}";
      cout << "Inserting: " << histTitle << endl;
      histograms[index12] = PtPtHistos->h_f2_evt[index12]; histTitles[index12] = new String(histTitle);
      nGraphs++;
      }
    }
//  histograms[0] = PtPtHistos->h_f2_evt[2]; histTitles[0] = new String("f_{2}^{00}");
//    plot(1, canvasNameBase+"f2_evt",canvasConfiguration,graphConfigurations,
       plot(nGraphs, canvasNameBase+"f2_evt",canvasConfiguration,graphConfigurations,
       "Mult", 20.0, 400.0,
       "f_{2}", 0.0, -1.2,
       histograms,histTitles,0.75, 0.7, 0.85, 0.8, 0.055);

  nGraphs = 0;
  for (int i1=0; i1<nPart; i1++)
    {
    for (int i2=i1; i2<nPart; i2++)
      {
      int index12 = PtPtHistos->index2(i1,i2);
      histTitle = "F_{2}";
      histTitle += "^{";
      histTitle += i1;
      histTitle += i2;
      histTitle += "}";
      cout << "Inserting: " << histTitle << endl;
      histograms[index12] = PtPtDerivedHistos->h_F2_evt[index12]; histTitles[index12] = new String(histTitle);
      if (!histograms[index12])
        {
        cout << "<F> HistogramGroup not loaded!!!! ABORT!!!" << endl;
        return;
        }
      nGraphs++;
      }
    }
//  plot(1, canvasNameBase+"F2_evt",canvasConfiguration,graphConfigurations,
       plot(nGraphs, canvasNameBase+"F2_evt",canvasConfiguration,graphConfigurations,
       "Mult", 20.0, 400.0,
       "F_{2}", 1.2, -1.2,
       histograms,histTitles,0.75, 0.7, 0.85, 0.8, 0.055);

  nGraphs = 0;
   for (int i1=0; i1<nPart; i1++)
     {
     for (int i2=i1; i2<nPart; i2++)
       {
       int index12 = PtPtHistos->index2(i1,i2);
       histTitle = "R_{2}";
       histTitle += "^{";
       histTitle += i1;
       histTitle += i2;
       histTitle += "}";
       cout << "Inserting: " << histTitle << endl;
       histograms[index12] = PtPtDerivedHistos->h_R2_evt[index12]; histTitles[index12] = new String(histTitle);
       if (!histograms[index12])
         {
         cout << "<F> HistogramGroup not loaded!!!! ABORT!!!" << endl;
         return;
         }
       nGraphs++;
       }
     }
   plot(nGraphs, canvasNameBase+"R2_evt",canvasConfiguration,graphConfigurations,
   //     plot(1, canvasNameBase+"R2_evt",canvasConfiguration,graphConfigurations,
        "Mult", 20.0, 400.0,
             "R_{2}", -0.15, 0.15,
        histograms,histTitles,0.75, 0.7, 0.85, 0.8, 0.055);



//  nGraphs = 0;
//  for (int i1=0; i1<nPart; i1++)
//    {
//    for (int i2=i1; i2<nPart; i2++)
//      {
//      for (int i3=i2; i3<nPart; i3++)
//        {
//        int index123 = PtPtHistos->index3(i1,i2,i3);
//        histTitle = "f_{3}";
//        histTitle += "^{";
//        histTitle += i1;
//        histTitle += i2;
//        histTitle += i3;
//        histTitle += "}";
//        cout << "Inserting: " << histTitle << endl;
//        histograms[index123] = PtPtHistos->h_f3_evt[index123]; histTitles[index123] = new String(histTitle);
//        if (!histograms[index123])
//          {
//          cout << "<F> HistogramGroup not loaded!!!! ABORT!!!" << endl;
//          return;
//          }
//        nGraphs++;
//        }
//      }
//    }
  histograms[0] = PtPtHistos->h_f3_evt[1]; histTitles[0] = new String("f_{3}^{000}");

  plot(1, canvasNameBase+"f3_evt",canvasConfiguration,graphConfigurations,
       "Mult", 20.0, 400.0,
       "f_{3}", 0.0, -1.2,
       histograms,histTitles,0.75, 0.7, 0.85, 0.8, 0.055);

  nGraphs = 0;
   for (int i1=0; i1<nPart; i1++)
     {
     for (int i2=i1; i2<nPart; i2++)
       {
       for (int i3=i2; i3<nPart; i3++)
         {
         int index123 = PtPtHistos->index3(i1,i2,i3);
         histTitle = "F_{3}";
         histTitle += "^{";
         histTitle += i1;
         histTitle += i2;
         histTitle += i3;
         histTitle += "}";
         //cout << "Inserting: " << histTitle << endl;
         histograms[index123] = PtPtDerivedHistos->h_F3_evt[index123]; histTitles[index123] = new String(histTitle);
         if (!histograms[index123])
           {
           cout << "<F> HistogramGroup not loaded!!!! ABORT!!!" << endl;
           return;
           }
         nGraphs++;
         }
       }
     }
   //plot(nGraphs, canvasNameBase+"F3_evt",canvasConfiguration,graphConfigurations,
        plot(1, canvasNameBase+"F3_evt",canvasConfiguration,graphConfigurations,
        "Mult", 20.0, 400.0,
        "F_{3}", 1.2, -1.2,
        histograms,histTitles,0.75, 0.7, 0.85, 0.8, 0.055);

  nGraphs = 0;
    for (int i1=0; i1<nPart; i1++)
      {
      for (int i2=i1; i2<nPart; i2++)
        {
        for (int i3=i2; i3<nPart; i3++)
          {
          int index123 = PtPtHistos->index3(i1,i2,i3);
          histTitle = "R_{3}";
          histTitle += "^{";
          histTitle += i1;
          histTitle += i2;
          histTitle += i3;
          histTitle += "}";
          cout << "Inserting: " << histTitle << endl;
          histograms[index123] = PtPtDerivedHistos->h_R3_evt[index123]; histTitles[index123] = new String(histTitle);
          if (!histograms[index123])
            {
            cout << "<F> HistogramGroup not loaded!!!! ABORT!!!" << endl;
            return;
            }
          nGraphs++;
          }
        }
      }
    plot(nGraphs, canvasNameBase+"R3_evt",canvasConfiguration,graphConfigurations,
    //     plot(1, canvasNameBase+"R3_evt",canvasConfiguration,graphConfigurations,
         "Mult", 20.0, 400.0,
              "R_{3}", -0.03, 0.03,
         histograms,histTitles,0.75, 0.7, 0.85, 0.8, 0.055);


  nGraphs = 0;
  for (int i1=0; i1<nPart; i1++)
    {
    for (int i2=i1; i2<nPart; i2++)
      {
      for (int i3=i2; i3<nPart; i3++)
        {
        for (int i4=i3; i4<nPart; i4++)
          {
          int index1234 = PtPtHistos->index4(i1,i2,i3,i4);
          histTitle = "f_{4}";
          histTitle += "^{";
          histTitle += i1;
          histTitle += i2;
          histTitle += i3;
          histTitle += i4;
          histTitle += "}";
          cout << "Inserting: " << histTitle << endl;
          histograms[index1234] = PtPtHistos->h_f4_evt[index1234]; histTitles[index1234] = new String(histTitle);
          if (!histograms[index1234])
          {
          cout << "<F> HistogramGroup not loaded!!!! ABORT!!!" << endl;
          return;
          }
          nGraphs++;
          }
        }
      }
    }
   //plot(nGraphs, canvasNameBase+"f4_evt",canvasConfiguration,graphConfigurations,
        plot(1, canvasNameBase+"f4_evt",canvasConfiguration,graphConfigurations,
        "Mult", 20.0, 400.0,
        "f_{4}", 0.0, -1.2,
        histograms,histTitles,0.75, 0.7, 0.85, 0.8, 0.055);


  nGraphs = 0;
  for (int i1=0; i1<nPart; i1++)
    {
    for (int i2=i1; i2<nPart; i2++)
      {
      for (int i3=i2; i3<nPart; i3++)
        {
        for (int i4=i3; i4<nPart; i4++)
          {
          int index1234 = PtPtHistos->index4(i1,i2,i3,i4);
          histTitle = "F_{4}";
          histTitle += "^{";
          histTitle += i1;
          histTitle += i2;
          histTitle += i3;
          histTitle += i4;
          histTitle += "}";
          cout << "Inserting: " << histTitle << endl;
          histograms[index1234] = PtPtDerivedHistos->h_F4_evt[index1234]; histTitles[index1234] = new String(histTitle);
          if (!histograms[index1234])
          {
          cout << "<F> HistogramGroup not loaded!!!! ABORT!!!" << endl;
          return;
          }
          nGraphs++;
          }
        }
      }
    }
   //plot(nGraphs, canvasNameBase+"F4_evt",canvasConfiguration,graphConfigurations,
        plot(1, canvasNameBase+"F4_evt",canvasConfiguration,graphConfigurations,
        "Mult", 20.0, 400.0,
        "F_{4}", 1.2, -1.2,
        histograms,histTitles,0.75, 0.7, 0.85, 0.8, 0.055);


  nGraphs = 0;
  for (int i1=0; i1<nPart; i1++)
    {
    for (int i2=i1; i2<nPart; i2++)
      {
      for (int i3=i2; i3<nPart; i3++)
        {
        for (int i4=i3; i4<nPart; i4++)
          {
          int index1234 = PtPtHistos->index4(i1,i2,i3,i4);
          histTitle = "R_{4}";
          histTitle += "^{";
          histTitle += i1;
          histTitle += i2;
          histTitle += i3;
          histTitle += i4;
          histTitle += "}";
          cout << "Inserting: " << histTitle << endl;
          histograms[index1234] = PtPtDerivedHistos->h_R4_evt[index1234]; histTitles[index1234] = new String(histTitle);
          if (!histograms[index1234])
          {
          cout << "<F> HistogramGroup not loaded!!!! ABORT!!!" << endl;
          return;
          }
          nGraphs++;
          }
        }
      }
    }
   plot(nGraphs, canvasNameBase+"R4_evt",canvasConfiguration,graphConfigurations,
//          plot(1, canvasNameBase+"R4_evt",canvasConfiguration,graphConfigurations,
          "Mult", 20.0, 400.0,
               "R_{4}", -0.02, 0.02,
        histograms,histTitles,0.75, 0.7, 0.85, 0.8, 0.055);



}

void PtPtPlotter::makeComparisonPlots(const String & canvasNameBase,
                                       CanvasConfiguration & canvasConfiguration,
                                       int nGraphs,
                                       PtPtHistos ** PtPtHistos,
                                       PtPtDerivedHistos ** PtPtDerivedHistos,
                                       String ** histLabels,
                                       GraphConfiguration  ** graphConfigurations)
{
  TH1 ** histograms = new TH1* [100];
  String ** histTitles = new String * [100];

  String histName;
  String histTitleBase;
  String histTitle;

  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  //canvasConfiguration->setAxes(CanvasConfiguration::LogY);
int index1234 = PtPtHistos[0]->index4(0,1,2,3);
  histTitleBase = "f_{4}";
  for (int i1=0; i1<nGraphs; i1++)
    {
    histTitle = histTitleBase;
    histTitle += *histLabels[i1];
    //cout << "Inserting: " << histTitle  << endl;
    histograms[i1] = PtPtHistos[i1]->h_f4_evt[index1234];
    if (!histograms[i1])
        {
        cout << "<F> HistogramGroup not loaded!!!! ABORT!!!" << endl;
        return;
        }
    histTitles[i1] = new String(histTitle);

    }

   plot(nGraphs, canvasNameBase+"f4_evt",canvasConfiguration,graphConfigurations,
        "Mult", 20.0, 400.0,
        "f_{4}", 0.0, -1.2,
        histograms,histTitles,0.25, 0.6, 0.45, 0.85, 0.055);

  histTitleBase = "F_{4}";
   for (int i1=0; i1<nGraphs; i1++)
     {
     histTitle = histTitleBase;
     histTitle += *histLabels[i1];
     cout << "Inserting: " << histTitle  << endl;
     histograms[i1] = PtPtDerivedHistos[i1]->h_F4_evt[index1234]; histTitles[i1] = new String(histTitle);
     if (!histograms[i1])
       {
       cout << "<F> HistogramGroup not loaded!!!! ABORT!!!" << endl;
       return;
       }
     }
    plot(nGraphs, canvasNameBase+"F4_evt",canvasConfiguration,graphConfigurations,
         "Mult", 20.0, 400.0,
         "F_{4}", -50000, 50000,
         histograms,histTitles,0.25, 0.6, 0.45, 0.85, 0.055);

  histTitleBase = "R_{4}";
  for (int i1=0; i1<nGraphs; i1++)
    {
    histTitle = histTitleBase;
    histTitle += *histLabels[i1];
    cout << "Inserting: " << histTitle  << endl;
    histograms[i1] = PtPtDerivedHistos[i1]->h_R4_evt[index1234]; histTitles[i1] = new String(histTitle);
    if (!histograms[i1])
      {
      cout << "<F> HistogramGroup not loaded!!!! ABORT!!!" << endl;
      return;
      }
    }
   plot(nGraphs, canvasNameBase+"R4_evt",canvasConfiguration,graphConfigurations,
        "Mult", 20.0, 400.0,
        "R_{4}", -0.3, 0.3,
        histograms,histTitles,0.65, 0.6, 0.85, 0.9, 0.055);




}

void PtPtPlotter::makePtPtComparisonPlots(const String & canvasNameBase,
                                            CanvasConfiguration & canvasConfiguration,
                                            int nGraphs,
                                            PtPtHistos ** PtPtHistos,
                                            PtPtDerivedHistos ** PtPtDerivedHistos,
                                            String ** histLabels,
                                            GraphConfiguration  ** graphConfigurations)
{
  TH1 ** histograms = new TH1* [100];
  String ** histTitles = new String * [100];

  String histName;
  String histTitleBase;
  String histTitle;

  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  //canvasConfiguration->setAxes(CanvasConfiguration::LogY);
int index12= PtPtHistos[0]->index2(0,1);
  histTitleBase = "#nu_{Dyn} ";
  for (int i1=0; i1<nGraphs; i1++)
    {
    histTitle = histTitleBase;
    histTitle += *histLabels[i1];
    //cout << "Inserting: " << histTitle  << endl; // HMHMHMHM_PtPt_23_evt
    histograms[i1] = PtPtDerivedHistos[i1]->h_PtPt_evt[index12];
    if (!histograms[i1])
        {
        cout << "<F> HistogramGroup not loaded!!!! ABORT!!!" << endl;
        return;
        }
    histTitles[i1] = new String(histTitle);

    }

   plot(nGraphs, canvasNameBase+"PtPt_evt",canvasConfiguration,graphConfigurations,
        "Mult", 20.0, 400.0,
        "#nu_{Dyn}", 0.0, -1.2,
        histograms,histTitles,0.25, 0.6, 0.45, 0.85, 0.055);

}
