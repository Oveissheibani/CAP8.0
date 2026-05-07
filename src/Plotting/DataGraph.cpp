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
#include "DataGraph.hpp"
#include "PrintHelpers.hpp"
#include "Exceptions.hpp"

ClassImp(CAP::DataGraph);

namespace CAP
{

DataGraph::DataGraph(const String _graphName,
                     const String _xTitle,
                     const String _yTitle,
                     const String _legendText,
                     double _xMin, double _xMax,
                     double _yMin, double _yMax,
                     int _n,
                     double * _x, double *_exLow,  double *_exHigh,
                     double * _y, double *_eyLow,  double *_eyHigh, double *_eeyLow, double *_eeyHigh, int _opt)
: TH1D(_graphName,_graphName, 100, _xMin, _xMax),
legendText(_legendText),
nPoints(_n),
x(_x),
exLow(_exLow),
exHigh(_exHigh),
y(_y),
eyLow(_eyLow),
eyHigh(_eyHigh),
eeyLow(_eeyLow),
eeyHigh(_eeyHigh),
option(_opt)
{
  Fill(-100.0);
  SetName(_graphName);
  SetXTitle(_xTitle);
  SetYTitle(_yTitle);
  SetMinimum(_yMin);
  SetMaximum(_yMax);
  TGraph * g;
  g = new TGraphAsymmErrors (nPoints, x, y, exLow, exHigh, eyLow, eyHigh);
  graphs.push_back(g);
  g = new TGraphAsymmErrors (nPoints, x, y, exLow, exHigh, eeyLow, eeyHigh);
  graphs.push_back(g);
  visible = true;
}

DataGraph::DataGraph(const String _graphName,
                     const String _xTitle,
                     const String _yTitle,
                     const String _legendText,
                     double _xMin, double _xMax,
                     double _yMin, double _yMax,
                     int _n,
                     double * _x, double *_exLow,  double *_exHigh,
                     double * _y, double *_eyLow,  double *_eyHigh,
                     int _opt)
: TH1D(_graphName,_graphName, 100, _xMin, _xMax),
legendText(_legendText),
nPoints(_n),
x(_x),
exLow(_exLow),
exHigh(_exHigh),
y(_y),
eyLow(_eyLow),
eyHigh(_eyHigh),
eeyLow(nullptr),
eeyHigh(nullptr),
option(_opt)
{
  Fill(-100.0);
  SetName(_graphName);
  SetXTitle(_xTitle);
  SetYTitle(_yTitle);
  SetMinimum(_yMin);
  SetMaximum(_yMax);
  TGraph * g;
  g = new TGraphAsymmErrors (nPoints, x, y, exLow, exHigh, eyLow, eyHigh);
  graphs.push_back(g);
  visible = true;
}

DataGraph::DataGraph(const String _graphName,
                     const String _xTitle,
                     const String _yTitle,
                     const String _legendText,
                     double _xMin, double _xMax,
                     double _yMin, double _yMax,
                     TH1 * histo, int _opt)
: TH1D(_graphName,_graphName, 100, _xMin, _xMax),
legendText(_legendText),
nPoints(0),
x(nullptr),
exLow(nullptr),
exHigh(nullptr),
y(nullptr),
eyLow(nullptr),
eyHigh(nullptr),
eeyLow(nullptr),
eeyHigh(nullptr),
option(_opt)
{
  Fill(-100.0);
  SetName(_graphName);
  SetXTitle(_xTitle);
  SetYTitle(_yTitle);
  SetMinimum(_yMin);
  SetMaximum(_yMax);

  nPoints = histo->GetNbinsX();
  x       = new double[nPoints];
  exLow   = new double[nPoints];
  exHigh  = new double[nPoints];
  y       = new double[nPoints];
  eyLow   = new double[nPoints];
  eyHigh  = new double[nPoints];
  double w, ey;
  for (int k=0; k<nPoints; k++)
    {
    x[k] = histo->GetXaxis()->GetBinCenter(k+1);
    w = 0.5 * histo->GetXaxis()->GetBinWidth(k+1);
    exLow[k]  = w;
    exHigh[k] = w;
    y[k] = histo->GetBinContent(k+1);
    ey   = histo->GetBinError(k+1);
    eyLow[k]  = ey;
    eyHigh[k] = ey;
    }

  TGraph * g;
  g = new TGraphAsymmErrors (nPoints, x, y, exLow, exHigh, eyLow, eyHigh);
  graphs.push_back(g);
  visible = true;
}

DataGraph::DataGraph(const String _graphName,
                     const String _xTitle,
                     const String _yTitle,
                     const String _legendText __attribute__((unused)),
                     double _xMin, double _xMax,
                     double _yMin, double _yMax,
                     TH1 * histo, double *_eeyLow, double *_eeyHigh,  int _opt)
: TH1D(_graphName,_graphName, 100, _xMin, _xMax),
nPoints(0),
x(nullptr),
exLow(nullptr),
exHigh(nullptr),
y(nullptr),
eyLow(nullptr),
eyHigh(nullptr),
eeyLow(_eeyLow),
eeyHigh(_eeyHigh),
option(_opt)
{
  Fill(-100.0);
  SetName(_graphName);
  SetXTitle(_xTitle);
  SetYTitle(_yTitle);
  SetMinimum(_yMin);
  SetMaximum(_yMax);

  nPoints = histo->GetNbinsX();
  x       = new double[nPoints];
  exLow   = new double[nPoints];
  exHigh  = new double[nPoints];
  y       = new double[nPoints];
  eyLow   = new double[nPoints];
  eyHigh  = new double[nPoints];
  double w, ey;
  for (int k=0; k<nPoints; k++)
    {
    x[k] = histo->GetXaxis()->GetBinCenter(k+1);
    w = 0.5 * histo->GetXaxis()->GetBinWidth(k+1);
    exLow[k]  = w;
    exHigh[k] = w;
    y[k] = histo->GetBinContent(k+1);
    ey   = histo->GetBinError(k+1);
    eyLow[k]  = ey;
    eyHigh[k] = ey;
    }

  TGraph * g;
  g = new TGraphAsymmErrors (nPoints, x, y, exLow, exHigh, eyLow, eyHigh);
  graphs.push_back(g);
  visible = true;
}



void DataGraph::draw(bool same)
{
  if (!same) Draw();

  String graphOption;
  switch (option)
    {
      default:
      case 0:
      if (graphs.size()>1)
        {
        graphOption = "E2"; //graphOption = ".SAME";
        graphs[1]->Draw(graphOption);
        graphOption = "P"; //if (same) graphOption = ".SAME";
        graphs[0]->Draw(graphOption);
        }
      else
        {
        graphOption = "P"; //if (same) graphOption = ".SAME";
        graphs[0]->Draw(graphOption);
        }
     break;
    }
}

void DataGraph::setMinMax(double minY, double maxY)
{
  SetMinimum(minY);
  SetMaximum(maxY);
}

void DataGraph::setProperties(GraphConfiguration & graphConfig)
{
  graphs[0]->SetLineColor(  graphConfig.valueInt(name(),"lineColor"));
  graphs[0]->SetLineStyle(  graphConfig.valueInt(name(),"lineStyle"));
  graphs[0]->SetLineWidth(  graphConfig.valueInt(name(),"lineWidth"));
  graphs[0]->SetMarkerColor(graphConfig.valueInt(name(),"markerColor"));
  graphs[0]->SetMarkerStyle(graphConfig.valueInt(name(),"markerStyle"));
  graphs[0]->SetMarkerSize( graphConfig.valueDouble("markerSize"));
  graphs[0]->SetFillColor(0);
  if (graphs.size()>1)
    {
    graphs[1]->SetLineColor(  graphConfig.valueInt(name(),"systsColor"));
    graphs[1]->SetLineStyle(  graphConfig.valueInt(name(),"systsStyle"));
    graphs[1]->SetLineWidth(  graphConfig.valueInt(name(),"systsWidth"));
    graphs[1]->SetMarkerColor(graphConfig.valueInt(name(),"markerColor"));
    graphs[1]->SetMarkerStyle(1);
    graphs[1]->SetMarkerSize(0.1);
    graphs[1]->SetFillStyle(graphConfig.valueInt(name(),"systsStyle"));
    graphs[1]->SetFillColor(graphConfig.valueInt(name(),"systsColor"));
    }

  TAxis * xAxis = (TAxis *) GetXaxis();
  xAxis->SetNdivisions( graphConfig.valueDouble("nXDivisions"));
  xAxis->SetTitleSize(  graphConfig.valueDouble("xTitleSize"));
  xAxis->SetTitleOffset(graphConfig.valueDouble("xTitleOffset"));
  xAxis->SetLabelSize(  graphConfig.valueDouble("xLabelSize"));
  xAxis->SetLabelOffset(graphConfig.valueDouble("xLabelOffset"));
  TAxis * yAxis = (TAxis *) GetYaxis();
  yAxis->SetNdivisions( graphConfig.valueInt(name(),"nYDivisions"));
  yAxis->SetTitleSize(  graphConfig.valueDouble("yTitleSize"));
  yAxis->SetTitleOffset(graphConfig.valueDouble("yTitleOffset"));
  yAxis->SetLabelSize(  graphConfig.valueDouble("yLabelSize"));
  yAxis->SetLabelOffset(graphConfig.valueDouble("yLabelOffset"));
}

void DataGraph::printDataValue(int k,std::ostream & output)
{
  String s;
  s = "  x:";
  s += x[k];
  if (exLow  &&  exHigh)
    {
    s += "+-";
    s += exHigh[k];
    s += "/";
    s += exLow[k];
    }
  else if (exLow)
    {
    s += "+-";
    s += exLow[k];
    }
  if (eyLow  &&  eyHigh)
    {
    s += " (stat) +-";
    s += eyHigh[k];
    s += "/";
    s += eyLow[k];
    }
  else if (eyLow)
    {
    s += " (stat) +-";
    s += eyLow[k];
    }
  if (eeyLow  &&  eeyHigh)
    {
    s += " (stat) +-";
    s += eeyHigh[k];
    s += "/";
    s += eeyLow[k];
    }
  else if (eeyLow)
    {
    s += " (syst) +-";
    s += eyLow[k];
    }
  printString(s,output);
}

void DataGraph::printToFile(std::ostream & output)
{
  printCR(output);
  printLine(output);
  printValue("Graph Name",name(),output);
  printValue("Graph Title",name(),output);
  printValue("Graph nPoints",nPoints,output);
  for (int k=0; k<nPoints; k++) printDataValue(k,output);
  printLine(output);
}


DataGraph *  DataGraph::loadGraph(const String  graphName,
                                  const String  xTitle,
                                  const String  yTitle,
                                  const String  legendText,
                                  double xMin, double xMax,
                                  double yMin, double yMax,
                                  TFile * inputFile,
                                  String folderName,
                                  String h1Name,
                                  String h1e1Name,
                                  String h1e2Name,
                                  String g1Name )
{
  if (!inputFile) return nullptr;

  String name;
  name = folderName; name += "/"; name += h1Name;
  TH1 * h1 = (TH1*)inputFile->Get(name);
  if (!h1) throw HistogramException(name,"Could not load histogram",__FUNCTION__);
  name = folderName; name += "/"; name += h1e1Name;
  TH1 * h1e1 = (TH1*)inputFile->Get(name);
  if (!h1e1) throw HistogramException(name,"Could not load histogram",__FUNCTION__);
  name = folderName; name += "/"; name += h1e2Name;
  TH1 * h1e2 = (TH1*)inputFile->Get(name);
  if (!h1e2) throw HistogramException(name,"Could not load histogram",__FUNCTION__);
  name = folderName; name += "/"; name += g1Name;
  TGraphAsymmErrors* g = (TGraphAsymmErrors*)inputFile->Get(name);
  if (!g) throw HistogramException(name,"Could not load graph",__FUNCTION__);
  int n = h1->GetNbinsX();
  double * x       = new double[n];
  double * exLow   = new double[n];
  double * exHigh  = new double[n];
  double * y       = new double[n];
  double * eyLow   = new double[n];
  double * eyHigh  = new double[n];
  double * eeyLow  = new double[n];
  double * eeyHigh = new double[n];
  for (int k=0; k<n; k++)
    {
//    x[k]       = g->pointX(k);
//    exLow[k]   = g->GetErrorXlow(k);
//    exHigh[k]  = g->GetErrorXhigh(k);
//    y[k]       = g->pointY(k);
//    eyLow[k]   = g->GetErrorYlow(k);
//    eyHigh[k]  = g->GetErrorYhigh(k);
//    eeyLow[k]  = h1e1->GetBinContent(k);
//    eeyHigh[k] = h1e2->GetBinContent(k);
    }
  DataGraph * dg = new DataGraph(graphName, xTitle, yTitle, legendText, xMin, xMax, yMin, yMax, n,
                               x, exLow, exHigh,y,eyLow,eyHigh,eeyLow,eeyHigh,0);
  return dg;
}

DataGraph * DataGraph::loadGraph(const String graphName,
                             const String xTitle,
                             const String yTitle,
                             const String legendText,
                             double xMin, double xMax,
                             double yMin, double yMax,
                             TFile * inputFile, String hName, double scale, int rebin)
{
  if (!inputFile) return nullptr;
  String name;
  name = hName;
  TH1 * histo = (TH1*)inputFile->Get(name);
  if (!histo) throw HistogramException(name,"Could not load histogram",__FUNCTION__);
  if (rebin>1)
    {
    histo->Rebin(rebin);
    histo->Scale(1.0/double(rebin));
    }
  int n = histo->GetNbinsX();
  double * x       = new double[n];
  double * exLow   = new double[n];
  double * exHigh  = new double[n];
  double * y       = new double[n];
  double * eyLow   = new double[n];
  double * eyHigh  = new double[n];
  double w, ey;
  for (int k=0; k<n; k++)
    {
    x[k] = histo->GetXaxis()->GetBinCenter(k+1);
    w = 0.5 * histo->GetXaxis()->GetBinWidth(k+1);
    exLow[k]  = w;
    exHigh[k] = w;
    y[k] = scale * histo->GetBinContent(k+1);
    ey   = scale * histo->GetBinError(k+1);
    eyLow[k]  = ey;
    eyHigh[k] = ey;
    }
  DataGraph * dg = new DataGraph(graphName, xTitle, yTitle, legendText, xMin, xMax, yMin, yMax, n,
                               x, exLow, exHigh,y,eyLow,eyHigh);
  return dg;
}

} // namespace CAP
