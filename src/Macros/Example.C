
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

//!
//! Create a director of the given name
//! The action is not taken is the directory already exist
//!
void createDirectory(const TString & dirName)
{
  gSystem->mkdir(dirName,1);
}

//!
//!Set default plotting style we like
//!
void setDefaultOptions(bool color)
{
  if (color)
    gStyle->SetPalette(1,0);
  else
    gStyle->SetPalette(7,0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetOptDate(0);
  gStyle->SetOptTitle(0);
  gStyle->SetPadBorderMode(0);
}

//!
//!Create and position a Legend for display in graphs
//!
TLegend * createLegend(double x1, double y1, double x2, double y2, double fontSize)
{
  TLegend *legend = new TLegend(x1,y1,x2,y2);
  legend->SetTextSize(fontSize);
  legend->SetFillColor(0);
  legend->SetBorderSize(0);
  return legend;
}

TLine * createLine(float x1, float y1, float x2, float y2, int style, int color, int width, bool doDraw)
{
  TLine *line = new TLine(x1,y1,x2,y2);
  line->SetLineStyle(style);
  line->SetLineColor(color);
  line->SetLineWidth(width);
  if (doDraw) line->Draw();
  return line;
}

//!
//!Create Arrow Line
//!
TArrow * createArrow(float x1, float y1, float x2, float y2, float arrowSize, Option_t* option, int style, int color, int width, bool doDraw)
{
  TArrow *line = new TArrow(x1,y1,x2,y2,arrowSize,option);
  line->SetLineStyle(style);
  line->SetLineColor(color);
  line->SetLineWidth(width);
  if (doDraw) line->Draw();
  return line;
}

class GraphConfiguration
{
public:
  GraphConfiguration();
  GraphConfiguration(const GraphConfiguration & source);
  GraphConfiguration operator=(const GraphConfiguration & rhs);
  ~GraphConfiguration() {}

  int   lineColor;
  int   lineStyle;
  int   lineWidth;
  int   markerColor;
  int   markerStyle;
  int   nXDivisions;
  int   nYDivisions;
  int   nZDivisions;
  float markerSize;
  float xTitleSize;
  float xTitleOffset;
  float xLabelSize;
  float xLabelOffset;
  float yTitleSize;
  float yTitleOffset;
  float yLabelSize;
  float yLabelOffset;
  float zTitleSize;
  float zTitleOffset;
  float zLabelSize;
  float zLabelOffset;

  void setMyFavorites(int color);

};

//!
//!Default Constructor
//!
GraphConfiguration::GraphConfiguration()
{
  setMyFavorites(2);
}

//!
//!Copy Constructor
//!
GraphConfiguration::GraphConfiguration(const GraphConfiguration & source)
:
lineColor(source.lineColor),
lineStyle(source.lineStyle),
lineWidth(source.lineWidth),
markerColor(source.markerColor),
markerStyle(source.markerStyle),
nXDivisions(source.nXDivisions),
nYDivisions(source.nYDivisions),
nZDivisions(source.nZDivisions),
markerSize(source.markerSize),
xTitleSize(source.xTitleSize),
xTitleOffset(source.xTitleOffset),
xLabelSize(source.xLabelSize),
xLabelOffset(source.xLabelOffset),
yTitleSize(source.yTitleSize),
yTitleOffset(source.yTitleOffset),
yLabelSize(source.yLabelSize),
yLabelOffset(source.yLabelOffset),
zTitleSize(source.zTitleSize),
zTitleOffset(source.zTitleOffset),
zLabelSize(source.zLabelSize),
zLabelOffset(source.zLabelOffset)
{  }

//!
//!Assignment operator for GraphConfiguration objects
//!
GraphConfiguration GraphConfiguration::operator=(const GraphConfiguration & rhs)
{
  lineColor = rhs.lineColor;
  lineStyle = rhs.lineStyle;
  lineWidth = rhs.lineWidth;
  markerColor = rhs.markerColor;
  markerStyle = rhs.markerStyle;
  nXDivisions = rhs.nXDivisions;
  nYDivisions = rhs.nYDivisions;
  nZDivisions = rhs.nZDivisions;
  markerSize = rhs.markerSize;
  xTitleSize = rhs.xTitleSize;
  xTitleOffset = rhs.xTitleOffset;
  xLabelSize = rhs.xLabelSize;
  xLabelOffset = rhs.xLabelOffset;
  yTitleSize = rhs.yTitleSize;
  yTitleOffset = rhs.yTitleOffset;
  yLabelSize = rhs.yLabelSize;
  yLabelOffset = rhs.yLabelOffset;
  zTitleSize = rhs.zTitleSize;
  zTitleOffset = rhs.zTitleOffset;
  zLabelSize = rhs.zLabelSize;
  zLabelOffset = rhs.zLabelOffset;
  return *this;
}

void GraphConfiguration::setMyFavorites(int color)
{
  lineColor = color;
  lineStyle = 1;
  lineWidth = 2;
  markerColor = color;
  markerStyle = 20;
  nXDivisions = 5;
  nYDivisions = 5;
  nZDivisions = 5;
  markerSize = 1.1;
  xTitleSize = 0.05;
  xTitleOffset = 1.2;
  xLabelSize = 0.05;
  xLabelOffset = 0.01;
  yTitleSize = 0.05;
  yTitleOffset = 1.45;
  yLabelSize = 0.05;
  yLabelOffset = 0.01;
  zTitleSize = 0.05;
  zTitleOffset = 0.5;
  zLabelSize = 0.05;
  zLabelOffset = 0.01;
}

class CanvasConfiguration
{
public:
  int   windowXPosition;
  int   windowYPosition;
  int   windowWidth;
  int   windowHeight;
  float leftMargin;
  float topMargin;
  float rightMargin;
  float bottomMargin;
  bool  logX;
  bool  logY;
  bool  logZ;

  CanvasConfiguration()
  :
  windowXPosition(20),
  windowYPosition(20),
  windowWidth(600),
  windowHeight(600),
  leftMargin(0.15),
  topMargin(0.05),
  rightMargin(0.05),
  bottomMargin(0.15),
  logX(0),
  logY(0),
  logZ(0)
  {   }

  CanvasConfiguration(const CanvasConfiguration & source)
  :
  windowXPosition(source.windowXPosition),
  windowYPosition(source.windowYPosition),
  windowWidth(source.windowWidth),
  windowHeight(source.windowHeight),
  leftMargin(source.leftMargin),
  topMargin(source.topMargin),
  rightMargin(source.rightMargin),
  bottomMargin(source.bottomMargin),
  logX(source.logX),
  logY(source.logY),
  logZ(source.logZ)
  { }

  CanvasConfiguration &  operator=(const CanvasConfiguration & rhs)
  {
  windowXPosition = rhs.windowXPosition;
  windowYPosition = rhs.windowYPosition;
  windowWidth = rhs.windowWidth;
  windowHeight = rhs.windowHeight;
  leftMargin = rhs.leftMargin;
  topMargin = rhs.topMargin;
  rightMargin = rhs.rightMargin;
  bottomMargin = rhs.bottomMargin;
  logX = rhs.logX;
  logY = rhs.logY;
  logZ = rhs.logZ;
  return *this;
  }

  virtual ~CanvasConfiguration() {}

  //!
  //!Set margin sizes
  //!
  void setMargins(float _leftMargin, float _topMargin, float _rightMargin, float _bottomMargin)
  {
  leftMargin = _leftMargin;
  topMargin = _topMargin;
  rightMargin = _rightMargin;
  bottomMargin = _bottomMargin;
  }

  //!
  //!Set the window size
  //!
  void setSize(int _width, int _height)
  {
  windowWidth = _width;
  windowHeight = _height;
  }

  //!
  //!Set the nominal window position
  //!
  void setPosition(int x, int y)
  {
  windowXPosition = x;
  windowYPosition = y;
  }
};

TCanvas * createCanvas(const TString & canvasName, const CanvasConfiguration & configuration, int inc=0)
{
  TCanvas * canvas = new TCanvas(canvasName,
                                 canvasName,
                                 inc+configuration.windowXPosition,
                                 configuration.windowYPosition,
                                 configuration.windowWidth,
                                 configuration.windowHeight);
  canvas->SetLogx(        configuration.logX);
  canvas->SetLogy(        configuration.logY);
  canvas->SetLogz(        configuration.logZ);
  canvas->SetRightMargin( configuration.rightMargin);
  canvas->SetLeftMargin(  configuration.leftMargin);
  canvas->SetBottomMargin(configuration.bottomMargin);
  canvas->SetTopMargin(   configuration.topMargin);
  return canvas;
}


void setHistoProperties(TH1 * h, const GraphConfiguration & graphConfiguration)
{
  h->SetLineColor(graphConfiguration.lineColor);
  h->SetLineStyle(graphConfiguration.lineStyle);
  h->SetLineWidth(graphConfiguration.lineWidth);
  h->SetMarkerColor(graphConfiguration.markerColor);
  h->SetMarkerStyle(graphConfiguration.markerStyle);
  h->SetMarkerSize (graphConfiguration.markerSize);
  TAxis * xAxis = (TAxis *) h->GetXaxis();
  xAxis->SetNdivisions(graphConfiguration.nXDivisions);
  xAxis->SetTitleSize(graphConfiguration.xTitleSize);
  xAxis->SetTitleOffset(graphConfiguration.xTitleOffset);
  xAxis->SetLabelSize(graphConfiguration.xLabelSize);
  xAxis->SetLabelOffset(graphConfiguration.xLabelOffset);
  TAxis * yAxis = (TAxis *) h->GetYaxis();
  yAxis->SetNdivisions(graphConfiguration.nYDivisions);
  yAxis->SetTitleSize(graphConfiguration.yTitleSize);
  yAxis->SetTitleOffset(graphConfiguration.yTitleOffset);
  yAxis->SetLabelSize(graphConfiguration.yLabelSize);
  yAxis->SetLabelOffset(graphConfiguration.yLabelOffset);
  if (h->IsA() == TH2::Class()  || h->IsA() == TH2F::Class() || h->IsA() == TH2F::Class() )
    {
    TAxis * zAxis = (TAxis *) h->GetZaxis();
    zAxis->SetNdivisions(graphConfiguration.nZDivisions);
    zAxis->SetTitleSize(graphConfiguration.zTitleSize);
    zAxis->SetTitleOffset(graphConfiguration.zTitleOffset);
    zAxis->SetLabelSize(graphConfiguration.zLabelSize);
    zAxis->SetLabelOffset(graphConfiguration.zLabelOffset);
    }
}


int Example(unsigned int nCounts=10000)
{
  setDefaultOptions(1);

  CanvasConfiguration canvasConfig;
  GraphConfiguration graphConfig1;
  GraphConfiguration graphConfig2;
  graphConfig1.setMyFavorites(2);
  graphConfig2.setMyFavorites(4);

  TH1D * h1 = new TH1D("h1","h1",100,0.0,100.0);
  TH1D * h2 = new TH1D("h2","h2",100,0.0,100.0);
  h1->SetXTitle("R");
  h1->SetYTitle("f(R)");

  for (unsigned int k=0; k<nCounts; k++)
    {
    double r1 = gRandom->Gaus(30.0,5.0);
    double r2 = gRandom->Gaus(60.0,15.0);
    h1->Fill(r1);
    h2->Fill(r2);
    }

  // Create a directory
  TCanvas * canvas1 = createCanvas("MyFunction",canvasConfig);

  // change properties of the two histograms
  setHistoProperties(h1,graphConfig1);
  setHistoProperties(h2,graphConfig2);
  // draw the two histograms
  h1->Draw("E");
  h2->Draw("SAME");

  //print the canvas to file
  TString directory("~/Documents/MyPlots/");
  TString canvasFileName = directory;
  createDirectory(directory);
  canvasFileName += canvas1->GetName();
  canvas1->Print(canvasFileName);

  return 0;
}
