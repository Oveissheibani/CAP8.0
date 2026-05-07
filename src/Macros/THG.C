
unsigned int   gOutputStyle  = 0;
unsigned int   gOutputLength = 50;


char selectFiller(unsigned int style)
{
  char filler;
  switch (style)
    {
      default:
      case 0: filler = '.'; break;
      case 1: filler = ' '; break;
      case 2: filler = '_'; break;
      case 3: filler = '-'; break;
      case 4: filler = '='; break;
    }
  return filler;
}

//!
//! Print n carriage returns.
//!
void printCR(unsigned int n=1)
{
  for (unsigned int k=0; k<n; k++) std::cout << std::endl;
}

//!
//! Print a line on (*gOutputStream) according to the properties size and gOutputStyle.
//!
void printLine(unsigned int n)
{
  char filler = selectFiller(4);
  for (unsigned int k=0; k<n; k++)
    std::cout <<  std::left << std::setw(2*gOutputLength) << std::setfill(filler)<< filler << std::endl;
}

//!
//! Print a line on (*gOutputStream) according to the properties size and gOutputStyle.
//!
void printString(const char * keyword)
{
  if (gOutputStyle>0)
    std::cout  << keyword <<  std::endl;
  else
    {
    char filler = selectFiller(gOutputStyle);
    std::cout  <<  std::left << std::setw(gOutputLength) << std::setfill(filler)<< keyword << " : " <<  std::endl;
    }
}

void printValue(const char * keyword, bool   value)
{
  char filler = selectFiller(gOutputStyle);
  std::cout  <<  std::left << std::setw(gOutputLength) << std::setfill(filler)<< keyword << " : " << value << std::endl;
}

void printValue(const char * keyword, size_t value)
{
  char filler = selectFiller(gOutputStyle);
  std::cout  <<  std::left << std::setw(gOutputLength) << std::setfill(filler)<< keyword << " : " << value <<  std::endl;
}

void printValue(const char * keyword, unsigned int value)
{
  char filler = selectFiller(gOutputStyle);
  std::cout  <<  std::left << std::setw(gOutputLength) << std::setfill(filler)<< keyword << " : " << value <<  std::endl;
}

void printValue(const char * keyword, int    value)
{
  char filler = selectFiller(gOutputStyle);
  std::cout  <<  std::left << std::setw(gOutputLength) << std::setfill(filler)<< keyword << " : " << value <<  std::endl;
}

void printValue(const char * keyword, long   value)
{
  char filler = selectFiller(gOutputStyle);
  std::cout  <<  std::left << std::setw(gOutputLength) << std::setfill(filler)<< keyword << " : " << value << std::endl;
}

void printValue(const char * keyword, long long value)
{
  char filler = selectFiller(gOutputStyle);
  std::cout  <<  std::left << std::setw(gOutputLength) << std::setfill(filler)<< keyword << " : " << value << std::endl;
}


void printValue(const char * keyword, double value)
{
  char filler = selectFiller(gOutputStyle);
  std::cout  <<  std::left << std::setw(gOutputLength) << std::setfill(filler)<< keyword << " : " << value << std::endl;
}

void printValue(const char * keyword, const char * value)
{
  char filler = selectFiller(gOutputStyle);
  std::cout  <<  std::left << std::setw(gOutputLength) << std::setfill(filler)<< keyword << " : " << value << std::endl;
}

void printValue(const char * keyword, const TString value)
{
  char filler = selectFiller(gOutputStyle);
  std::cout  <<  std::left << std::setw(gOutputLength) << std::setfill(filler)<< keyword << " : " << value << std::endl;
}

void printRange(const char * keyword, double minimum, double maximum)
{
  char filler = selectFiller(gOutputStyle);
  std::cout  << std::left << std::setw(gOutputLength) << std::setfill(filler)<< keyword
  << " Minimum=" << minimum << "  Maximum=" << maximum << std::endl;
}

void printRange(const char * keyword, int minimum, int maximum)
{
  char filler = selectFiller(gOutputStyle);
  std::cout  << std::left << std::setw(gOutputLength) << std::setfill(filler)<< keyword
  << " Minimum=" << minimum << "  Maximum=" << maximum << std::endl;
}

void printDualRange(const char * keyword,
                    double minimum,  double maximum,
                    double minimum2, double maximum2)
{
  char filler = selectFiller(gOutputStyle);
  std::cout  << std::left << std::setw(gOutputLength) << std::setfill(filler)<< keyword
  << " Minimum=" << minimum << "  Maximum=" << maximum
  << " Mimumum2=" << minimum2 << "  Maximum2=" << maximum2
  << std::endl;
}

void printDualRange(const char * keyword,
                    int minimum,  int maximum,
                    int minimum2, int maximum2)
{
  char filler = selectFiller(gOutputStyle);
  std::cout  << std::left << std::setw(gOutputLength) << std::setfill(filler)<< keyword
  << " Minimum=" << minimum << "  Maximum=" << maximum
  << " Mimumum2=" << minimum2 << "  Maximum2=" << maximum2
  << std::endl;
}



TH1 * loadH1(TFile * inputFile, TString histoName)
{
  TH1 * h = (TH1*) inputFile->Get(histoName);
  return h;
}

TH1 * loadH2(TFile & inputFile, TString histoName)
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
  legend->SetTextColor(1);
  legend->SetTextFont(42);
  legend->SetTextSize(fontSize);
  legend->SetFillColor(0);
  legend->SetBorderSize(0);
  return legend;
}

TLegend * createLegend(std::vector<TH1*> & histograms,
                       std::vector<TString> & titles,
                       double x1, double x2, double y1, double y2,
                       double fontSize,
                       bool doDraw)
{
  TLegend *legend = new TLegend(x1,y1,x2,y2,"","NB.NDC");
  legend->SetTextSize(fontSize);
  legend->SetFillColor(0);
  legend->SetBorderSize(0);
  for (unsigned int k=0; k<histograms.size();k++)
    {
    legend->AddEntry(histograms[k],titles[k],"lep");
    }
  if (doDraw) legend->Draw();
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
  line->SetFillColor(color);

  if (doDraw) line->Draw();
  return line;
}

class GraphConfiguration
{
public:
  GraphConfiguration();
  GraphConfiguration operator=(const GraphConfiguration & rhs);
  ~GraphConfiguration() {}

  int   lineColor;
  int   lineStyle;
  int   lineWidth;
  int   markerColor;
  int   markerStyle;
  float markerSize;

  int   nXDivisions;
  int   xTitleColor;
  int   xTitleFont;
  float xTitleSize;
  float xTitleOffset;
  int   xLabelColor;
  int   xLabelFont;
  float xLabelSize;
  float xLabelOffset;

  int   nYDivisions;
  int   yTitleColor;
  int   yTitleFont;
  float yTitleSize;
  float yTitleOffset;
  int   yLabelColor;
  int   yLabelFont;
  float yLabelSize;
  float yLabelOffset;

  void setMyFavorites(int color=1, int markerStyle=20, float markerSize=0.05);

};

//!
//!Default Constructor
//!
GraphConfiguration::GraphConfiguration()
{
  setMyFavorites(1,20,0.05);
}



//!
//!Assignment operator for GraphConfiguration objects
//!
GraphConfiguration GraphConfiguration::operator=(const GraphConfiguration & rhs)
{
  if (this!=&rhs)
    {
    lineColor = rhs.lineColor;
    lineStyle = rhs.lineStyle;
    lineWidth = rhs.lineWidth;
    markerColor = rhs.markerColor;
    markerStyle = rhs.markerStyle;
    markerSize  = rhs.markerSize;

    nXDivisions  = rhs.nXDivisions;
    xTitleColor  = rhs.xTitleColor;
    xTitleFont   = rhs.xTitleFont;
    xTitleSize   = rhs.xTitleSize;
    xTitleOffset = rhs.xTitleOffset;
    xLabelColor  = rhs.xLabelColor;
    xLabelFont   = rhs.xLabelFont;
    xLabelSize   = rhs.xLabelSize;
    xLabelOffset = rhs.xLabelOffset;

    nYDivisions  = rhs.nYDivisions;
    yTitleColor  = rhs.yTitleColor;
    yTitleFont   = rhs.yTitleFont;
    yTitleSize   = rhs.yTitleSize;
    yTitleOffset = rhs.yTitleOffset;
    yLabelColor  = rhs.yLabelColor;
    yLabelFont   = rhs.yLabelFont;
    yLabelSize   = rhs.yLabelSize;
    yLabelOffset = rhs.yLabelOffset;

    }
  return *this;
}

void GraphConfiguration::setMyFavorites(int color, int style, float size)
{
  float fontSize = 32;
  int fontCode = 42;

  lineColor = color;
  lineStyle = 1;
  lineWidth = 1;

  markerColor = color;
  markerStyle = style;
  markerSize  = size;

  nXDivisions = 5;
  nYDivisions = 5;

  nXDivisions  = 5;
  xTitleColor  = 1;
  xTitleFont   = fontCode;
  xTitleSize   = fontSize;
  xTitleOffset = 1.1;
  xLabelColor  = 1;
  xLabelFont   = fontCode;
  xLabelSize   = fontSize;
  xLabelOffset = 0.002;

  nYDivisions  = 5;
  yTitleColor  = 1;
  yTitleFont   = fontCode;
  yTitleSize   = fontSize;
  yTitleOffset = 1.50;
  yLabelColor  = 1;
  yLabelFont   = fontCode;
  yLabelSize   = fontSize;
  yLabelOffset = 0.01;

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
  bottomMargin(0.17),
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


void setHistoProperties(TH1 * h, const GraphConfiguration & config)
{
  h->SetLineStyle(config.lineStyle);
  h->SetLineColor(config.lineColor);
  h->SetLineWidth(config.lineWidth);
  h->SetMarkerColor(config.markerColor);
  h->SetMarkerStyle(config.markerStyle);
  h->SetMarkerSize(config.markerSize);

  TAxis * xAxis = (TAxis *) h->GetXaxis();
  xAxis->SetNdivisions(config.nXDivisions);
  xAxis->SetTitleColor(config.xTitleColor);
  xAxis->SetTitleFont(config.xTitleFont);
  xAxis->SetTitleSize(config.xTitleSize);
  xAxis->SetTitleOffset(config.xTitleOffset);
  xAxis->SetLabelColor(config.xLabelColor);
  xAxis->SetLabelFont(config.xLabelFont);
  xAxis->SetLabelSize(config.xLabelSize);
  xAxis->SetLabelOffset(config.xLabelOffset);

  TAxis * yAxis = (TAxis *) h->GetYaxis();
  yAxis->SetNdivisions(config.nYDivisions);
  yAxis->SetTitleColor(config.yTitleColor);
  yAxis->SetTitleFont(config.yTitleFont);
  yAxis->SetTitleSize(config.yTitleSize);
  yAxis->SetTitleOffset(config.yTitleOffset);
  yAxis->SetLabelColor(config.yLabelColor);
  yAxis->SetLabelFont(config.yLabelFont);
  yAxis->SetLabelSize(config.yLabelSize);
  yAxis->SetLabelOffset(config.yLabelOffset);
}

//!
//!Open Root file at the given path and with the given file name
//!ioOption : OLD, NEW, RECREATE
//
TFile * openRootFile(const TString & path,
                     const TString & name,
                     const TString & ioOption,
                     bool verbose)
{
  TString fileName;
  if (name.BeginsWith("/"))
    fileName = name;
  else
    {
    TString filePath = path;
    int slash = filePath.First('/');
    int len   = filePath.Length();
    if (len>0 && (len-1)!=slash) filePath += "/";
    fileName = filePath;
    fileName += name;
    }
  if (!fileName.EndsWith(".root")) fileName += ".root";
  if (verbose)
    {
    printCR();
    printValue("openRootFile(...) Opening file",fileName);
    printValue("ioOption",ioOption);
    }
  TFile * file = new TFile(fileName,ioOption);
  if (!file)
    {
    std::cout << "no file found named: " << fileName << std::endl;
    exit(1);
    }
  if (!file->IsOpen())
    {
    std::cout << "no file found named: " << fileName << std::endl;
    exit(1);
    }
  if (verbose) std::cout << "openRootFile(...) File opened successfully." << std::endl;
  return file;
}

TFile * openRootFile(const TString & name,
                     const TString & ioOption,
                     bool verbose)
{
  TString fileName = name;
  if (!fileName.EndsWith(".root")) fileName += ".root";
  if (verbose)
    {
    printCR();
    printValue("openRootFile(...) Opening file",fileName);
    printValue("ioOption",ioOption);
    }
  TFile * file = new TFile(fileName,ioOption);
  if (!file)
    {
    std::cout << "no file found named: " << fileName << std::endl;
    exit(1);
    }

  if (!file->IsOpen())
    {
    std::cout << "no file found named: " << fileName << std::endl;
    exit(1);
    }
  if (verbose)
    {
    printCR();
    printValue("openRootFile(...) File opened successfully",fileName);
    }
  return file;
}

TFile * openOldRootFile(const TString & name,
                        bool verbose)
{
  return openRootFile(name,"OLD",verbose);
}

TFile * openOldRootFile(const TString & path,
                        const TString & name,
                        bool verbose)
{
  return openRootFile(path,name,"OLD",verbose);
}

TFile * openNewRootFile(const TString & name,
                        bool verbose)
{
  return openRootFile(name,"NEW",verbose);
}

TFile * openNewRootFile(const TString & path,
                        const TString & name,
                        bool verbose)
{
  return openRootFile(path,name,"NEW",verbose);
}

TFile * openRecreateRootFile(const TString & name,
                             bool verbose)
{
  return openRootFile(name,"RECREATE",verbose);
}

TFile * openRecreateRootFile(const TString & path,
                             const TString & name,
                             bool verbose)
{
  return openRootFile(path,name,"RECREATE",verbose);
}

std::vector<TFile*> openRootFiles(std::vector<TString> & names,
                                  const TString   & ioOption,
                                  bool verbose)
{
  int nFiles = names.size();
  if (nFiles<1)
    {
    std::cout << "no files available" << std::endl;
    exit(1);
    }
  if (verbose)
    {
    printCR();
    printValue("openRootFiles(...) Opening N files",names.size());
    }
  std::vector<TFile*> files;
  for (auto & name : names)
    {
    files.push_back( openRootFile(name,ioOption,verbose));
    }
  if (verbose)
    {
    printCR();
    printString("openRootFiles(...) Completed successfully");
    }
  return files;
}

std::vector<TFile*> openRootFiles(const TString & path,
                                  std::vector<TString> & names,
                                  const TString   & ioOption,
                                  bool verbose)
{
  int nFiles = names.size();
  if (nFiles<1)
    {
    std::cout << "Empty file list -- File not found/opened" << std::endl;
    }
  if (verbose)
    {
    printCR();
    printValue("openRootFiles(...) Opening N files",names.size());
    printValue("openRootFiles(...) Path",path);
    }
  std::vector<TFile*> files;
  for (auto & name : names)
    {
    files.push_back( openRootFile(path,name,ioOption,verbose));
    }
  if (verbose)  std::cout << "openRootInputFiles(...) Completed successfully" << std::endl;
  return files;
}

TCanvas * plot(const TString & canvasName,
               const CanvasConfiguration & canvasConfig,
               vector<TH1*> & histos,
               const vector<TString> & labels,
               float xMinimum, float xMaximum,
               float yMinimum, float yMaximum,
               bool drawLegend=1,
               float x1=0.1, float y1=0.1, float x2=0.2, float y2=0.2, float fontSize=24)
{
  if (histos.size()<1)
    {
    cout << "<ERROR> plot() histos.size()<1" << endl;
    exit(1);
    }
  TCanvas * c = createCanvas(canvasName,canvasConfig);

  int n = histos[0]->GetNbinsX();

  histos[0]->GetXaxis()->SetRangeUser(xMinimum,xMaximum);
  histos[0]->SetMinimum(yMinimum);
  histos[0]->SetMaximum(yMaximum);
  histos[0]->Draw("E");
  printValue("name",histos[0]->GetTitle());
  printValue("name",labels[0]);
  for (int k=1;k<=n;k++)
    std::cout << k << "   " << histos[0]->GetBinContent(k) << endl;

  for (unsigned int iHisto = 1; iHisto<histos.size(); iHisto++)
    {
    printValue("name",histos[iHisto]->GetTitle());
    printValue("name",labels[iHisto]);
    for (int k=1;k<=n;k++)
      std::cout << k << "   " << histos[iHisto]->GetBinContent(k) << endl;
    histos[iHisto]->Draw("E.SAME");

    }
  if (drawLegend)
    {
    printValue("xMinimum",x1);
    printValue("xMaximum",x2);
    printValue("yMinimum",y1);
    printValue("yMaximum",y2);
    TLegend * legend = createLegend(x1, y1, x2, y2, fontSize);
    for (unsigned int iHisto = 0; iHisto<histos.size(); iHisto++)
      legend->AddEntry(histos[iHisto],labels[iHisto],"lp"); // lep
    legend->SetNColumns(3);
    legend->Draw();
    }
  return c;
}

TCanvas * plot2(const TString & canvasName,
                vector<TH1*> & histos1,
                vector<TH1*> & histos2,
                const vector<TString> & labels,
                float yMinimum, float yMaximum,
                bool drawLegend=1,
                float x1=0.1, float y1=0.1, float x2=0.2, float y2=0.2, float fontSize=24)
{
  float  div1 = 0.55;
  TCanvas * c;
  TPad *pad1;
  TPad *pad2;
  c = new TCanvas(canvasName,canvasName, 60,60, 800, 800);

  pad1 = new TPad("pad1","pad1",0.0,div1,1.0,0.99);
  pad1->SetLogy(true);
  pad1->SetRightMargin( 0.02);
  pad1->SetLeftMargin(  0.15);
  pad1->SetBottomMargin(0.001);
  pad1->SetTopMargin(   0.001);
  pad1->Draw();
  pad2 = new TPad("pad2","pad2",0.0,0.0,1.0,div1);
  pad2->SetLogy(true);
  pad2->SetRightMargin( 0.02);
  pad2->SetLeftMargin(  0.15);
  pad2->SetBottomMargin(0.15);
  pad2->SetTopMargin(   0.001);
  pad2->Draw();

  pad1->cd();
  float pairMin = 1.01E-4;
  float pairMax = 1.999;
  histos1[0]->SetMinimum(pairMin);
  histos1[0]->SetMaximum(pairMax);
  histos1[0]->SetNdivisions(5,"Y");
  histos1[0]->Draw("E");
  for (unsigned int iHisto = 1; iHisto<histos1.size(); iHisto++)
    histos1[iHisto]->Draw("E.SAME");

  pad2->cd();
  pairMin = 1.01E-4;
  pairMax = 1.999;
  histos2[0]->SetMinimum(pairMin);
  histos2[0]->SetMaximum(pairMax);
  histos2[0]->SetNdivisions(5,"Y");
  histos2[0]->Draw("E");
  for (unsigned int iHisto = 1; iHisto<histos1.size(); iHisto++)
    histos2[iHisto]->Draw("E.SAME");

  if (drawLegend)
    {
    TLegend * legend = createLegend(x1,y1,x2,y2,fontSize);
    for (unsigned int iHisto = 0; iHisto<histos1.size(); iHisto++)
      legend->AddEntry(histos1[iHisto],labels[iHisto],"lep");
    legend->Draw();
    }

  return c;
}


TCanvas * plot3(const TString & canvasName,
                vector<TH1*> & histos1,
                vector<TH1*> & histos2,
                vector<TH1*> & histos3,
                const vector<TString> & labels,
                float yMinimum, float yMaximum,
                bool drawLegend=1,
                float x1=0.1, float y1=0.1, float x2=0.2, float y2=0.2, float fontSize=24,bool ratioAtBottom = false)
{
  float pairMin = yMinimum;
  float pairMax = yMaximum;
  float pairMinB = yMinimum;
  float pairMaxB = yMaximum;
  if (ratioAtBottom)
    {
    pairMinB = 0.0;
    pairMaxB = 14.99;
    }

  float  div1 = 0.69;
  float  div2 = 0.37;
  TCanvas * c;
  TPad *pad1;
  TPad *pad2;
  TPad *pad3;
  c = new TCanvas(canvasName,canvasName, 60,60, 800, 800);

  pad1 = new TPad("pad1","pad1",0.0,div1,1.0,0.99);
  pad1->SetLogy(true);
  pad1->SetRightMargin( 0.02);
  pad1->SetLeftMargin(  0.15);
  pad1->SetBottomMargin(0.001);
  pad1->SetTopMargin(   0.001);
  pad1->Draw();
  pad2 = new TPad("pad2","pad2",0.0,div2,1.0,div1);
  pad2->SetLogy(true);
  pad2->SetRightMargin( 0.02);
  pad2->SetLeftMargin(  0.15);
  pad2->SetBottomMargin(0.001);
  pad2->SetTopMargin(   0.001);
  pad2->Draw();

  pad3 = new TPad("pad3","pad3",0.0,0.00,1.0,div2);
  pad3->SetLogy(!ratioAtBottom);
  pad3->SetRightMargin( 0.02);
  pad3->SetLeftMargin(  0.15);
  pad3->SetBottomMargin(0.20);
  pad3->SetTopMargin(   0.001);
  pad3->Draw();

  pad1->cd();
  unsigned int n = histos1.size();
  unsigned int nBins = histos1[0]->GetNbinsX();
  std::cout << "First Panel" << std::endl;
  printValue("iHisto",0);
  printValue("name",histos1[0]->GetTitle());
  printValue("name",labels[0]);
  for (int k=1;k<=nBins;k++)    std::cout << k << "   " << histos1[0]->GetBinContent(k) << endl;

  histos1[0]->SetMinimum(pairMin);
  histos1[0]->SetMaximum(pairMax);
  histos1[0]->SetNdivisions(5,"Y");
  histos1[0]->Draw("E");
  for (unsigned int iHisto = 1; iHisto<n; iHisto++)
    {
    printValue("iHisto",iHisto);
    printValue("name",histos1[iHisto]->GetTitle());
    printValue("name",labels[iHisto]);
    for (int k=1;k<=nBins;k++)    std::cout << k << "   " << histos1[iHisto]->GetBinContent(k) << endl;
    histos1[iHisto]->Draw("E.SAME");
    }

  pad2->cd();
  std::cout << "Second Panel" << std::endl;
  printValue("iHisto",0);
  printValue("name",histos2[0]->GetTitle());
  printValue("name",labels[0]);
  for (int k=1;k<=nBins;k++)    std::cout << k << "   " << histos2[0]->GetBinContent(k) << endl;


  histos2[0]->SetMinimum(pairMin);
  histos2[0]->SetMaximum(pairMax);
  histos2[0]->SetNdivisions(5,"Y");
  histos2[0]->Draw("E");
  for (unsigned int iHisto = 1; iHisto<n; iHisto++)
    {
    printValue("iHisto",iHisto);
    printValue("name",histos2[iHisto]->GetTitle());
    printValue("name",labels[iHisto]);
    for (int k=1;k<=nBins;k++)    std::cout << k << "   " << histos2[iHisto]->GetBinContent(k) << endl;
    histos2[iHisto]->Draw("E.SAME");
    }

  pad3->cd();
  std::cout << "Third Panel" << std::endl;
  printValue("iHisto",0);
  printValue("name",histos3[0]->GetTitle());
  printValue("name",labels[0]);
  for (int k=1;k<=nBins;k++)    std::cout << k << "   " << histos3[0]->GetBinContent(k) << endl;


  histos3[0]->SetMinimum(pairMinB);
  histos3[0]->SetMaximum(pairMaxB);
  histos3[0]->SetNdivisions(3,"Y");
  histos3[0]->Draw("E");
  for (unsigned int iHisto = 1; iHisto<n; iHisto++)
    {
    printValue("iHisto",iHisto);
    printValue("name",histos3[iHisto]->GetTitle());
    printValue("name",labels[iHisto]);
    for (int k=1;k<=nBins;k++)    std::cout << k << "   " << histos3[iHisto]->GetBinContent(k) << endl;
    histos3[iHisto]->Draw("E.SAME");
    }

  if (drawLegend)
    {
    TLegend * legend = createLegend(x1,y1,x2,y2,fontSize);
    for (unsigned int iHisto = 0; iHisto<histos1.size(); iHisto++)
      legend->AddEntry(histos1[iHisto],labels[iHisto],"lep");
    legend->Draw();
    }

  return c;
}

TH1 * createPairYieldVsTemp(const TString & name, const TString & title, const std::vector<TH1 *> densities, unsigned int index)
{
  TH1 * h = new TH1D(name, title, 4, 130.0, 210.0);
  double value;
  unsigned int k = 1;
  printValue("name",name);
  printValue("title",title);
  for (auto & d : densities)
    {
    value = d->GetBinContent(index);
    std::cout << k << "   " << value << endl;
    h->SetBinContent(k,value);
    h->SetBinError(k++,0.0);
    }
  return h;
}


int THG()
{
  setDefaultOptions(1);
  CanvasConfiguration canvasConfig;
  canvasConfig.setSize(800,800);
  canvasConfig.logY = true;

  vector<GraphConfiguration *> graphConfigs;
  graphConfigs.push_back(new GraphConfiguration());
  graphConfigs.push_back(new GraphConfiguration());
  graphConfigs.push_back(new GraphConfiguration());
  graphConfigs.push_back(new GraphConfiguration());
  graphConfigs.push_back(new GraphConfiguration());
  graphConfigs.push_back(new GraphConfiguration());
  graphConfigs.push_back(new GraphConfiguration());
  float ss = 2.1;
  graphConfigs[0]->setMyFavorites(kBlack,kFullSquare,ss);
  graphConfigs[1]->setMyFavorites(kBlue,kFullCircle,ss);
  graphConfigs[2]->setMyFavorites(kRed,kFullSquare,ss);
  graphConfigs[3]->setMyFavorites(kMagenta,kFullCircle,ss);
  graphConfigs[4]->setMyFavorites(kBlack,kFullSquare,ss);
  graphConfigs[5]->setMyFavorites(kBlue,kFullCircle,ss);
  graphConfigs[6]->setMyFavorites(kRed,kFullSquare,ss);
  TString inputPath      = "/Users/aa7526/Documents/MyPublications/Theory/BalanceFunctions/FeedDownHadronGas/";
  TString outDirectory   = "/Users/aa7526/Documents/MyPublications/Theory/BalanceFunctions/FeedDownHadronGas/New/";
  //TString outDirectory = "/Volumes/ClaudeDisc4/OutputFiles/ThermalGas/";
  //TString inputPath    = "/Users/aa7526/Documents/GitHub/CAP8.0/build/";
  createDirectory(outDirectory);

  std::vector<TCanvas*> canvasList;
  vector<TFile*> files;
  vector<TString> fileNames;
  vector<TString> labels;
  vector<TString> labelsR;
  vector<TString> rhos;
  rhos.push_back(TString("C_{2}(#alpha|#gamma) (fm^{-6})") );
  rhos.push_back(TString("C_{2}(#alpha|#pi^{0}) (fm^{-6})") );
  rhos.push_back(TString("C_{2}(#alpha|#pi^{+}) (fm^{-6})") );
  rhos.push_back(TString("C_{2}(#alpha|#pi^{-}) (fm^{-6})") );
  rhos.push_back(TString("C_{2}(#alpha|K^{0}) (fm^{-6})") );
  rhos.push_back(TString("C_{2}(#alpha|#bar{K}^{0}) (fm^{-6})"));
  rhos.push_back(TString("C_{2}(#alpha|K^{+}) (fm^{-6})"));
  rhos.push_back(TString("C_{2}(#alpha|K^{-}) (fm^{-6})"));
  rhos.push_back(TString("C_{2}(#alpha|p) (fm^{-6})"));
  rhos.push_back(TString("C_{2}(#alpha|#bar{p}) (fm^{-6})"));
  rhos.push_back(TString("C_{2}(#alpha|n) (fm^{-6})"));
  rhos.push_back(TString("C_{2}(#alpha|#bar{n}) (fm^{-6})"));
  rhos.push_back(TString("C_{2}(#alpha|#Lambda) (fm^{-6})"));
  rhos.push_back(TString("C_{2}(#alpha|#bar{#Lambda}) (fm^{-6})"));

  fileNames.push_back(TString("THG_140.root"));
  fileNames.push_back(TString("THG_160.root"));
  fileNames.push_back(TString("THG_180.root"));
  fileNames.push_back(TString("THG_200.root"));
  labels.push_back(TString("140 MeV"));
  labels.push_back(TString("160 MeV"));
  labels.push_back(TString("180 MeV"));
  labels.push_back(TString("200 MeV"));
  labelsR.push_back(TString("200 MeV"));
  labelsR.push_back(TString("180 MeV"));
  labelsR.push_back(TString("160 MeV"));
  labelsR.push_back(TString("140 MeV"));
  for (unsigned int iFile=0; iFile<fileNames.size(); iFile++)
    files.push_back(openOldRootFile(inputPath,fileNames[iFile],true));

  TH1 * h;
  vector<TH1*> thermalDensityVsIndex;
  vector<TH1*> thermalDensityVsIndexR;
  vector<TH1*> relThermalDensityVsIndex;
  vector<TH1*> thermalDensityVsMass;
  vector<TH1*> thermalDensityVsMassR;
  vector<TH1*> singleDensityVsIndex;
  vector<TH1*> singleDensityWFDVsIndex;
  vector<TH1*> singleDensityRatioVsIndex;
  vector<vector<TH1*>> pairDensityWFD;
  vector<vector<TH1*>> normPairDensityWFD;
  vector<TH1*> pairs;
  float  fontSize  = 30;
  int    fontCode = 42;


  //  float  markerSize = 1.5;
  //  double pairMin   = 1.0E-7;
  //  double pairMax   = 1.9999;
  //  float  yTitleOffset = 1.6;
  //  float  yLabelOffset = 0.03;

  TString name;
  for (unsigned int iFile=0; iFile<fileNames.size(); iFile++)
    {
    h = loadH1(files[iFile],"THG_numberDensity");
    h->SetTitle(labels[iFile]);
    h->SetXTitle("#alpha");
    h->SetYTitle("#rho_{1}^{TH}(#alpha) (fm^{-3})");
    setHistoProperties(h,*(graphConfigs[iFile]));
    thermalDensityVsIndex.push_back(h);

    h = loadH1(files[iFile],"THG_numberDensityVsM");
    h->SetTitle(labels[iFile]);
    h->SetXTitle("Mass (GeV/c^{2})");
    h->SetYTitle("<#rho_{1}^{TH}(#alpha)>  (fm^{-3})");
    setHistoProperties(h,*(graphConfigs[iFile]));
    thermalDensityVsMass.push_back(h);

    h = loadH1(files[iFile],"THG_relNumberDensity");
    h->SetTitle(labels[iFile]);
    h->SetXTitle("#alpha");
    h->SetYTitle("#rho_{1}^{TH}(#alpha)/#rho_{1}^{TH}(#pi)  ");
    setHistoProperties(h,*(graphConfigs[iFile]));
    relThermalDensityVsIndex.push_back(h);

    h = loadH1(files[iFile],"THG_singleDensityVsIndex");
    h->SetTitle(labels[iFile]);
    h->SetXTitle("#alpha");
    h->SetYTitle("#rho_{1}^{TH}(#alpha)  (fm^{-3})");
    setHistoProperties(h,*(graphConfigs[iFile]));
    singleDensityVsIndex.push_back(h);

    h = loadH1(files[iFile],"THG_singleDensityFDVsIndex");
    h->SetTitle(labels[iFile]);
    h->SetXTitle("#alpha");
    h->SetYTitle("#rho_{1}^{TH+FD}(#alpha)  (fm^{-3})");
    setHistoProperties(h,*(graphConfigs[iFile]));
    singleDensityWFDVsIndex.push_back(h);

    h = loadH1(files[iFile],"THG_singleDensityRatioVsIndex");
    h->SetTitle(labels[iFile]);
    h->SetXTitle("#alpha");
    h->SetYTitle("#rho_{1}^{TH+FD}(#alpha)/#rho_{1}^{TH}(#alpha) ");
    setHistoProperties(h,*(graphConfigs[iFile]));
    singleDensityRatioVsIndex.push_back(h);
    }

  thermalDensityVsIndexR.push_back(thermalDensityVsIndex[3]);
  thermalDensityVsIndexR.push_back(thermalDensityVsIndex[2]);
  thermalDensityVsIndexR.push_back(thermalDensityVsIndex[1]);
  thermalDensityVsIndexR.push_back(thermalDensityVsIndex[0]);

  thermalDensityVsMassR.push_back(thermalDensityVsMass[3]);
  thermalDensityVsMassR.push_back(thermalDensityVsMass[2]);
  thermalDensityVsMassR.push_back(thermalDensityVsMass[1]);
  thermalDensityVsMassR.push_back(thermalDensityVsMass[0]);

  for (unsigned int iStable=0; iStable<14; iStable++)
    {
    name = "THG_pair_"; name+=iStable;
    pairs.clear();
    for (unsigned int iFile=0; iFile<fileNames.size(); iFile++)
      {
      h = loadH1(files[iFile],name);
      h->SetTitle(labels[iFile]);
      h->SetXTitle("#alpha");
      //cout << "iStable=" << iStable << "  rho=" << rhos[iFile] << endl;
      h->SetYTitle(rhos[iStable]);
      setHistoProperties(h,*(graphConfigs[iFile]));
      pairs.push_back(h);
      }
    pairDensityWFD.push_back(pairs);

    name = "THG_normPair_"; name+=iStable;
    pairs.clear();
    for (unsigned int iFile=0; iFile<fileNames.size(); iFile++)
      {
      h = loadH1(files[iFile],name);
      h->SetTitle(labels[iFile]);
      h->SetXTitle("#alpha");
      h->SetYTitle(rhos[iStable]);
      setHistoProperties(h,*(graphConfigs[iFile]));
      pairs.push_back(h);
      }
    normPairDensityWFD.push_back(pairs);
    }

  //canvasList.push_back(plot("ThermalDensityVsTempVsIndex",  canvasConfig, thermalDensityVsIndexR,   labelsR, 0.0,  370.0,  1.0E-8, 2.0, 1, 0.70, 0.7,  0.89, 0.9, fontSize));
  //canvasList.push_back(plot("ThermalDensityVsTempVsMass",   canvasConfig, thermalDensityVsMassR,    labelsR, 0.0,   2.49,  1.0E-8, 2.0, 1, 0.70, 0.7,  0.89, 0.9, fontSize));
  //canvasList.push_back(plot("SingleDensityVsTempVsIndex",   canvasConfig, singleDensityVsIndex,    labels, 0.0,   15.0,  1.0E-7, 2.0, 1, 0.85, 0.9,  0.89, 0.9, fontSize));
  //canvasList.push_back(plot("SingleDensityWFVsTempVsIndex", canvasConfig, singleDensityWFDVsIndex, labels, 0.0,   15.0,  1.0E-7, 2.0, 1, 0.85, 0.9,  0.89, 0.9, fontSize));

  if (0)
    {
    for (unsigned int iStable=0; iStable<14; iStable++)
      {
      vector<TH1*> & pairs = pairDensityWFD[iStable];
      cout << "iStable=" << iStable << " pairs.size()=" << pairs.size() << endl;
      TString pairName = "PairDensityWFD_VsTempVsIndex_"; pairName += iStable;
      canvasList.push_back(plot(pairName, canvasConfig,pairs,labels,0.0,15.,1.0E-6,2.0,1,0.72, 0.6, 0.85, 0.9, fontSize));
      }
    }

  if (0)
    {

    singleDensityRatioVsIndex[0]->GetXaxis()->SetLabelOffset(0.025);
    singleDensityRatioVsIndex[0]->GetXaxis()->SetTitleOffset(0.60);
    singleDensityRatioVsIndex[0]->GetYaxis()->SetTitleSize(28);
    singleDensityRatioVsIndex[0]->GetYaxis()->SetTitleOffset(1.8);
    singleDensityVsIndex[0]->GetXaxis()->SetLabelOffset(1.02);
    singleDensityVsIndex[0]->GetYaxis()->SetTitleSize(28);
    singleDensityVsIndex[0]->GetYaxis()->SetTitleOffset(1.8);
    singleDensityWFDVsIndex[0]->GetXaxis()->SetLabelOffset(1.02);
    singleDensityWFDVsIndex[0]->GetYaxis()->SetTitleSize(28);
    singleDensityWFDVsIndex[0]->GetYaxis()->SetTitleOffset(1.8);
    canvasList.push_back( plot3("SingleStableDensityIndex",singleDensityVsIndex,singleDensityWFDVsIndex,singleDensityRatioVsIndex,labels,1.01E-4,4.99,1,0.5,0.55,0.75,0.95,22,1) );
    }

  if (0)
    {
    pairDensityWFD[2][0]->GetXaxis()->SetLabelOffset(0.02);
    pairDensityWFD[6][0]->GetXaxis()->SetLabelOffset(0.02);
    pairDensityWFD[8][0]->GetXaxis()->SetLabelOffset(0.02);
    pairDensityWFD[2][0]->GetXaxis()->SetTitleOffset(1.8);
    pairDensityWFD[6][0]->GetXaxis()->SetTitleOffset(1.8);
    pairDensityWFD[8][0]->GetXaxis()->SetTitleOffset(1.8);
    canvasList.push_back( plot3("PairDensityWFD_G3VsTempVsIndex",pairDensityWFD[2],pairDensityWFD[6],pairDensityWFD[8],labels,1.01E-6,4.99, 1,0.6,0.4,0.75,0.9,28,false) );
    }

  if (0)
    {
    normPairDensityWFD[2][0]->GetXaxis()->SetLabelOffset(0.02);
    normPairDensityWFD[6][0]->GetXaxis()->SetLabelOffset(0.02);
    normPairDensityWFD[8][0]->GetXaxis()->SetLabelOffset(0.02);
    normPairDensityWFD[2][0]->GetXaxis()->SetTitleOffset(0.8);
    normPairDensityWFD[6][0]->GetXaxis()->SetTitleOffset(0.8);
    normPairDensityWFD[8][0]->GetXaxis()->SetTitleOffset(0.8);
    normPairDensityWFD[2][0]->GetYaxis()->SetTitle("C^{norm}_{2}(#alpha|#pi^{+}) ");
    normPairDensityWFD[6][0]->GetYaxis()->SetTitle("C^{norm}_{2}(#alpha|K^{+}) ");
    normPairDensityWFD[8][0]->GetYaxis()->SetTitle("C^{norm}_{2}(#alpha|p) ");
    canvasList.push_back( plot3("NormPairDensityWFD_G3VsTempVsIndex",normPairDensityWFD[2],normPairDensityWFD[6],normPairDensityWFD[8],labels,1.01E-4,1.99,1,0.75,0.4,0.95,0.9,28,false) );
    }

  if (1)
    {
    std::vector<TH1 *> densities;
    std::vector<TH1 *> pairsVsTemp;
    std::vector<TH1 *> normDensities;
    std::vector<TH1 *> normPairsVsTemp;
    TH1 * hh;

    std::vector<TString> pairLabels;
//    pairLabels.push_back(TString("#pi^{0}#pi^{+}"));
//    pairLabels.push_back(TString("#pi^{+}#pi^{+}"));
//    pairLabels.push_back(TString("#pi^{-}#pi^{+}"));
//    pairLabels.push_back(TString("K^{+}#pi^{+}"));
//    pairLabels.push_back(TString("K^{-}#pi^{+}"));
//    pairLabels.push_back(TString("p#pi^{+}"));
//    pairLabels.push_back(TString("#bar{p}#pi^{+}"));
    pairLabels.push_back(TString(" #pi^{0}"));
    pairLabels.push_back(TString(" #pi^{+}"));
    pairLabels.push_back(TString(" #pi^{-}"));
    pairLabels.push_back(TString(" K^{+}"));
    pairLabels.push_back(TString(" K^{-}"));
    pairLabels.push_back(TString(" #kern[0.1]{p}"));
    pairLabels.push_back(TString(" #bar{p}"));



    densities.clear();
    densities.push_back(pairDensityWFD[2][0]);
    densities.push_back(pairDensityWFD[2][1]);
    densities.push_back(pairDensityWFD[2][2]);
    densities.push_back(pairDensityWFD[2][3]);
    hh = createPairYieldVsTemp("pi0piP","#pi^{0}#pi^{+}", densities, 2);
    hh->GetXaxis()->SetTitle("T (MeV)");
    hh->GetYaxis()->SetTitle("C_{2}(#alpha|#pi^{+})  (fm^{-6})");
    setHistoProperties(hh,*(graphConfigs[0]));
    pairsVsTemp.push_back(hh);

    hh = createPairYieldVsTemp("piPpiP","#pi^{+}#pi^{+}", densities, 3);
    setHistoProperties(hh,*(graphConfigs[1]));
    pairsVsTemp.push_back(hh);

    hh = createPairYieldVsTemp("piMpiP","#pi^{-}#pi^{+}", densities, 4);
    setHistoProperties(hh,*(graphConfigs[2]));
    pairsVsTemp.push_back(hh);

    hh = createPairYieldVsTemp("kPpiP","K^{+}#pi^{+}", densities, 7);
    setHistoProperties(hh,*(graphConfigs[3]));
    pairsVsTemp.push_back(hh);

    hh = createPairYieldVsTemp("kMpiP","K^{-}#pi^{+}", densities, 8);
    setHistoProperties(hh,*(graphConfigs[4]));
    pairsVsTemp.push_back(hh);

    hh = createPairYieldVsTemp("pPpiP","p#pi^{+}", densities, 9);
    setHistoProperties(hh,*(graphConfigs[5]));
    pairsVsTemp.push_back(hh);

    hh = createPairYieldVsTemp("pMpiP","p#pi^{+}", densities, 10);
    setHistoProperties(hh,*(graphConfigs[6]));
    pairsVsTemp.push_back(hh);

    canvasList.push_back(plot("PairYiedVsTemp", canvasConfig,pairsVsTemp,pairLabels,130.0,200.0,1.0E-4,2.0,1, 0.62, 0.2, 0.92, 0.4, fontSize));

    normDensities.clear();
    normDensities.push_back(normPairDensityWFD[2][0]);
    normDensities.push_back(normPairDensityWFD[2][1]);
    normDensities.push_back(normPairDensityWFD[2][2]);
    normDensities.push_back(normPairDensityWFD[2][3]);
    hh = createPairYieldVsTemp("pi0piP_norm","#pi^{0}#pi^{+}", normDensities, 2);
    hh->GetXaxis()->SetTitle("T (MeV)");
    hh->GetYaxis()->SetTitle("C_{2}^{norm}(#alpha|#pi^{+})");
    setHistoProperties(hh,*(graphConfigs[0]));
    normPairsVsTemp.push_back(hh);

    hh = createPairYieldVsTemp("piPpiP_norm","#pi^{+}#pi^{+}", normDensities, 3);
    setHistoProperties(hh,*(graphConfigs[1]));
    normPairsVsTemp.push_back(hh);

    hh = createPairYieldVsTemp("piMpiP_norm","#pi^{-}#pi^{+}", normDensities, 4);
    setHistoProperties(hh,*(graphConfigs[2]));
    normPairsVsTemp.push_back(hh);

    hh = createPairYieldVsTemp("kPpiP_norm","K^{+}#pi^{+}", normDensities, 7);
    setHistoProperties(hh,*(graphConfigs[3]));
    normPairsVsTemp.push_back(hh);

    hh = createPairYieldVsTemp("kMpiP_norm","K^{-}#pi^{+}", normDensities, 8);
    setHistoProperties(hh,*(graphConfigs[4]));
    normPairsVsTemp.push_back(hh);

    hh = createPairYieldVsTemp("pPpiP_norm","p#pi^{+}", normDensities, 9);
    setHistoProperties(hh,*(graphConfigs[5]));
    normPairsVsTemp.push_back(hh);

    hh = createPairYieldVsTemp("pMpiP_norm","p#pi^{+}", normDensities, 10);
    setHistoProperties(hh,*(graphConfigs[6]));
    normPairsVsTemp.push_back(hh);

    canvasList.push_back(plot("NormPairYiedVsTemp", canvasConfig,normPairsVsTemp,pairLabels,130.0,200.0,1.0E-4,2.0,1, 0.62, 0.2, 0.92, 0.4, fontSize));

    }

  // print all canvas to PDF
  if (1)
    {
    for (auto & canvas : canvasList)
      {
      TString canvasFileName;
      canvasFileName = outDirectory;
      canvasFileName += canvas->GetName();
      canvasFileName += ".jpg";
      //canvasFileName += ".screenshot.pdf";
      canvas->SaveAs(canvasFileName);
      }
    }

  return 0;
}
