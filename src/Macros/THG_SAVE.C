
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



TH1 * cloneLoadH1(TFile & inputFile, TString histoName)
{
  TH1 * h = (TH1*) inputFile.Get(histoName);
  //TH1 * clone = (TH1*) h->Clone();
  return h;
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

  void setMyFavorites(int color=1, int markerStyle=20);

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

void GraphConfiguration::setMyFavorites(int color, int markerStyle)
{
  lineColor = color;
  lineStyle = 1;
  lineWidth = 2;
  markerColor = color;
  markerStyle = markerStyle;
  nXDivisions = 5;
  nYDivisions = 5;
  nZDivisions = 5;
  markerSize = 0.05;
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
//  h->SetLineStyle(1);
//  h->SetLineWidth(1);
//  h->SetMarkerColor(2);
  h->SetMarkerStyle(graphConfiguration.markerStyle);
//  h->SetMarkerSize (0.05);
//  TAxis * xAxis = (TAxis *) h->GetXaxis();
//  xAxis->SetNdivisions(graphConfiguration.nXDivisions);
//  xAxis->SetTitleSize(graphConfiguration.xTitleSize);
//  xAxis->SetTitleOffset(graphConfiguration.xTitleOffset);
//  xAxis->SetLabelSize(graphConfiguration.xLabelSize);
//  xAxis->SetLabelOffset(graphConfiguration.xLabelOffset);
//  TAxis * yAxis = (TAxis *) h->GetYaxis();
//  yAxis->SetNdivisions(graphConfiguration.nYDivisions);
//  yAxis->SetTitleSize(graphConfiguration.yTitleSize);
//  yAxis->SetTitleOffset(graphConfiguration.yTitleOffset);
//  yAxis->SetLabelSize(graphConfiguration.yLabelSize);
//  yAxis->SetLabelOffset(graphConfiguration.yLabelOffset);
//  if (h->IsA() == TH2::Class()  || h->IsA() == TH2F::Class() || h->IsA() == TH2F::Class() )
//    {
//    TAxis * zAxis = (TAxis *) h->GetZaxis();
//    zAxis->SetNdivisions(graphConfiguration.nZDivisions);
//    zAxis->SetTitleSize(graphConfiguration.zTitleSize);
//    zAxis->SetTitleOffset(graphConfiguration.zTitleOffset);
//    zAxis->SetLabelSize(graphConfiguration.zLabelSize);
//    zAxis->SetLabelOffset(graphConfiguration.zLabelOffset);
//    }
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


int THG()
{
  setDefaultOptions(1);

  CanvasConfiguration canvasConfig;
  canvasConfig.setSize(800,800);

//  GraphConfiguration graphConfig1;
//  GraphConfiguration graphConfig2;
//  GraphConfiguration graphConfig3;
//  GraphConfiguration graphConfig4;
//  graphConfig1.setMyFavorites();
//  graphConfig2.setMyFavorites(kBlue,kFullSquare);
//  graphConfig3.setMyFavorites(kRed,kFullSquare);
//  graphConfig4.setMyFavorites(kMagenta,kFullSquare);

  TString directory("/Volumes/ClaudeDisc4/OutputFiles/ThermalGas/");
  createDirectory(directory);
  TString canvasFileName;
  std::vector<TCanvas*> canvasList;
  std::vector<TH1*> histograms;
  std::vector<TString> titles;

  TFile & f140 = * openOldRootFile("/Users/aa7526/Documents/GitHub/CAP8.0/build/THG_140.root",true);
  TFile & f160 = * openOldRootFile("/Users/aa7526/Documents/GitHub/CAP8.0/build/THG_160.root",true);
  TFile & f180 = * openOldRootFile("/Users/aa7526/Documents/GitHub/CAP8.0/build/THG_180.root",true);
  TFile & f200 = * openOldRootFile("/Users/aa7526/Documents/GitHub/CAP8.0/build/THG_200.root",true);


//  TH1 * h_massVsIndex  = cloneLoadH1(f200,"THG_mass");
//
//  TCanvas * c = createCanvas("MassVsIndex",canvasConfig);
//  c->GetFrame()->SetLineColor(2);
//  canvasList.push_back(c);
//  h_massVsIndex->SetYTitle("Mass (GeV/c2)");
//  h_massVsIndex->SetMinimum(0.0);
//  h_massVsIndex->SetMaximum(3.0);
//  h_massVsIndex->Draw();
//
//  double mass;
//  TH1 * diag = new TH1F("Decay","Decay",1,0.0,100.0);
//  diag->SetMinimum(0.0);
//  diag->SetMaximum(3.0);
//  diag->GetXaxis()->SetTickLength(0); // Removes tick marks
//  diag->GetXaxis()->SetLabelOffset(999); //
//  diag->GetYaxis()->SetTitle("Mass (GeV/c^{2})"); //
//  diag->GetYaxis()->SetTitleOffset(2.0); //
//  diag->Draw();
//  for (unsigned int k=0; k<h_massVsIndex->GetNbinsX(); k++)
//    {
//    mass = h_massVsIndex->GetBinContent(k);
//    createLine(0.0, mass, 100.0, mass, 1, 1, 1, true);
//    }
//  mass = 1.0;
//  createArrow(10.0, mass, 10.0, 0.50, 0.02, "|>", 1, 2, 2,true);
//
//  return 1;
//  // -----

  canvasConfig.logY = true;

  TH1 * densityVsIndex140 = cloneLoadH1(f140,"THG_numberDensity");
  TH1 * densityVsIndex160 = cloneLoadH1(f160,"THG_numberDensity");
  TH1 * densityVsIndex180 = cloneLoadH1(f180,"THG_numberDensity");
  TH1 * densityVsIndex200 = cloneLoadH1(f200,"THG_numberDensity");

  densityVsIndex140->SetTitle("140 MeV");
  densityVsIndex160->SetTitle("160 MeV");
  densityVsIndex180->SetTitle("180 MeV");
  densityVsIndex200->SetTitle("200 MeV");

  densityVsIndex140->SetLineColor(kBlack);
  densityVsIndex140->SetMarkerColor(kBlack);
  densityVsIndex140->SetMarkerStyle(kFullSquare);

  densityVsIndex160->SetLineColor(kBlue);
  densityVsIndex160->SetMarkerColor(kBlue);
  densityVsIndex160->SetMarkerStyle(kFullCircle);

  densityVsIndex180->SetLineColor(kRed);
  densityVsIndex180->SetMarkerColor(kRed);
  densityVsIndex180->SetMarkerStyle(kFullSquare);

  densityVsIndex200->SetLineColor(kMagenta);
  densityVsIndex200->SetMarkerColor(kMagenta);
  densityVsIndex200->SetMarkerStyle(kFullCircle);

  canvasList.push_back(createCanvas("ThermalDensityVsTempVsIndex",canvasConfig));
  densityVsIndex140->SetYTitle("#rho_{1}^{TH}(#alpha)");
  densityVsIndex140->SetMinimum(1.0E-7);
  densityVsIndex140->SetMaximum(1.0);
  densityVsIndex140->Draw("E");
  densityVsIndex160->Draw("E.SAME");
  densityVsIndex180->Draw("E.SAME");
  densityVsIndex200->Draw("E.SAME");
  gPad->BuildLegend();


  // vs M
  densityVsIndex140 = cloneLoadH1(f140,"THG_numberDensityVsM");
  densityVsIndex160 = cloneLoadH1(f160,"THG_numberDensityVsM");
  densityVsIndex180 = cloneLoadH1(f180,"THG_numberDensityVsM");
  densityVsIndex200 = cloneLoadH1(f200,"THG_numberDensityVsM");

  densityVsIndex140->SetTitle("140 MeV");
  densityVsIndex160->SetTitle("160 MeV");
  densityVsIndex180->SetTitle("180 MeV");
  densityVsIndex200->SetTitle("200 MeV");

  densityVsIndex140->SetLineColor(kBlack);
  densityVsIndex140->SetMarkerColor(kBlack);
  densityVsIndex140->SetMarkerStyle(kFullSquare);

  densityVsIndex160->SetLineColor(kBlue);
  densityVsIndex160->SetMarkerColor(kBlue);
  densityVsIndex160->SetMarkerStyle(kFullCircle);

  densityVsIndex180->SetLineColor(kRed);
  densityVsIndex180->SetMarkerColor(kRed);
  densityVsIndex180->SetMarkerStyle(kFullSquare);

  densityVsIndex200->SetLineColor(kMagenta);
  densityVsIndex200->SetMarkerColor(kMagenta);
  densityVsIndex200->SetMarkerStyle(kFullCircle);

  canvasList.push_back(createCanvas("ThermalDensityVsTempVsM",canvasConfig));
  densityVsIndex140->SetYTitle("#rho_{1}^{TH}(#alpha)");
  densityVsIndex140->SetMinimum(1.0E-7);
  densityVsIndex140->SetMaximum(1.0);
  densityVsIndex140->Draw("E");
  densityVsIndex160->Draw("E.SAME");
  densityVsIndex180->Draw("E.SAME");
  densityVsIndex200->Draw("E.SAME");
  gPad->BuildLegend();

  // Relative to pion vs M
  densityVsIndex140 = cloneLoadH1(f140,"THG_relNumberDensity");
  densityVsIndex160 = cloneLoadH1(f160,"THG_relNumberDensity");
  densityVsIndex180 = cloneLoadH1(f180,"THG_relNumberDensity");
  densityVsIndex200 = cloneLoadH1(f200,"THG_relNumberDensity");

  densityVsIndex140->SetTitle("140 MeV");
  densityVsIndex160->SetTitle("160 MeV");
  densityVsIndex180->SetTitle("180 MeV");
  densityVsIndex200->SetTitle("200 MeV");

  densityVsIndex140->SetLineColor(kBlack);
  densityVsIndex140->SetMarkerColor(kBlack);
  densityVsIndex140->SetMarkerStyle(kFullSquare);

  densityVsIndex160->SetLineColor(kBlue);
  densityVsIndex160->SetMarkerColor(kBlue);
  densityVsIndex160->SetMarkerStyle(kFullCircle);

  densityVsIndex180->SetLineColor(kRed);
  densityVsIndex180->SetMarkerColor(kRed);
  densityVsIndex180->SetMarkerStyle(kFullSquare);

  densityVsIndex200->SetLineColor(kMagenta);
  densityVsIndex200->SetMarkerColor(kMagenta);
  densityVsIndex200->SetMarkerStyle(kFullCircle);

  canvasList.push_back(createCanvas("ThermalDensityVsTempVsIndexRelToPion",canvasConfig));
  densityVsIndex140->SetYTitle("#rho_{1}^{TH}(#alpha)/#rho_{1}^{TH}(#pi)");
  densityVsIndex140->SetMinimum(1.0E-7);
  densityVsIndex140->SetMaximum(1.0);
  densityVsIndex140->Draw("E");
  densityVsIndex160->Draw("E.SAME");
  densityVsIndex180->Draw("E.SAME");
  densityVsIndex200->Draw("E.SAME");
  gPad->BuildLegend();

  // Stable w/o feed-down v index
  // Relative to pion vs M
  densityVsIndex140 = cloneLoadH1(f140,"THG_singleDensityVsIndex");
  densityVsIndex160 = cloneLoadH1(f160,"THG_singleDensityVsIndex");
  densityVsIndex180 = cloneLoadH1(f180,"THG_singleDensityVsIndex");
  densityVsIndex200 = cloneLoadH1(f200,"THG_singleDensityVsIndex");

  densityVsIndex140->SetTitle("140 MeV");
  densityVsIndex160->SetTitle("160 MeV");
  densityVsIndex180->SetTitle("180 MeV");
  densityVsIndex200->SetTitle("200 MeV");

  densityVsIndex140->SetLineColor(kBlack);
  densityVsIndex140->SetMarkerColor(kBlack);
  densityVsIndex140->SetMarkerStyle(kFullSquare);

  densityVsIndex160->SetLineColor(kBlue);
  densityVsIndex160->SetMarkerColor(kBlue);
  densityVsIndex160->SetMarkerStyle(kFullCircle);

  densityVsIndex180->SetLineColor(kRed);
  densityVsIndex180->SetMarkerColor(kRed);
  densityVsIndex180->SetMarkerStyle(kFullSquare);

  densityVsIndex200->SetLineColor(kMagenta);
  densityVsIndex200->SetMarkerColor(kMagenta);
  densityVsIndex200->SetMarkerStyle(kFullCircle);

  canvasList.push_back(createCanvas("StableThermalDensityVsTempVsIndex",canvasConfig));
  densityVsIndex140->SetXTitle("#alpha");
  densityVsIndex140->SetYTitle("#rho_{1}^{TH}(#alpha)");
  densityVsIndex140->SetMinimum(1.0E-4);
  densityVsIndex140->SetMaximum(1.0);
  densityVsIndex140->Draw("E");
  densityVsIndex160->Draw("E.SAME");
  densityVsIndex180->Draw("E.SAME");
  densityVsIndex200->Draw("E.SAME");
  gPad->BuildLegend();


  TH1 * densityVsIndex140WFD = cloneLoadH1(f140,"THG_singleDensityFDVsIndex");
  TH1 * densityVsIndex160WFD = cloneLoadH1(f160,"THG_singleDensityFDVsIndex");
  TH1 * densityVsIndex180WFD = cloneLoadH1(f180,"THG_singleDensityFDVsIndex");
  TH1 * densityVsIndex200WFD = cloneLoadH1(f200,"THG_singleDensityFDVsIndex");
  densityVsIndex140WFD->SetTitle("140 MeV w/ FD");
  densityVsIndex160WFD->SetTitle("160 MeV w/ FD");
  densityVsIndex180WFD->SetTitle("180 MeV w/ FD");
  densityVsIndex200WFD->SetTitle("200 MeV w/ FD");

  densityVsIndex140WFD->SetLineColor(kBlack);
  densityVsIndex140WFD->SetMarkerColor(kBlack);
  densityVsIndex140WFD->SetMarkerStyle(kFullCircle);

  densityVsIndex160WFD->SetLineColor(kBlue);
  densityVsIndex160WFD->SetMarkerColor(kBlue);
  densityVsIndex160WFD->SetMarkerStyle(kFullCircle);

  densityVsIndex180WFD->SetLineColor(kRed);
  densityVsIndex180WFD->SetMarkerColor(kRed);
  densityVsIndex180WFD->SetMarkerStyle(kFullCircle);

  densityVsIndex200WFD->SetLineColor(kMagenta);
  densityVsIndex200WFD->SetMarkerColor(kMagenta);
  densityVsIndex200WFD->SetMarkerStyle(kFullCircle);

  canvasList.push_back(createCanvas("StableFDVsTempVsIndex",canvasConfig));
  densityVsIndex140WFD->SetXTitle("#alpha");
  densityVsIndex140WFD->SetYTitle("#rho_{1}^{FD}(#alpha)");
  densityVsIndex140WFD->SetMinimum(1.0E-4);
  densityVsIndex140WFD->SetMaximum(1.0);
  densityVsIndex140WFD->Draw("E");
  densityVsIndex160WFD->Draw("E.SAME");
  densityVsIndex180WFD->Draw("E.SAME");
  densityVsIndex200WFD->Draw("E.SAME");
  gPad->BuildLegend();

  /// Pairs w/ FD

  densityVsIndex140WFD = cloneLoadH1(f140,"THG_pair_2");
  densityVsIndex160WFD = cloneLoadH1(f160,"THG_pair_2");
  densityVsIndex180WFD = cloneLoadH1(f180,"THG_pair_2");
  densityVsIndex200WFD = cloneLoadH1(f200,"THG_pair_2");

  densityVsIndex140WFD->SetTitle("140 MeV w/ FD");
  densityVsIndex160WFD->SetTitle("160 MeV w/ FD");
  densityVsIndex180WFD->SetTitle("180 MeV w/ FD");
  densityVsIndex200WFD->SetTitle("200 MeV w/ FD");

  densityVsIndex140WFD->SetLineColor(kBlack);
  densityVsIndex140WFD->SetMarkerColor(kBlack);
  densityVsIndex140WFD->SetMarkerStyle(kFullCircle);

  densityVsIndex160WFD->SetLineColor(kBlue);
  densityVsIndex160WFD->SetMarkerColor(kBlue);
  densityVsIndex160WFD->SetMarkerStyle(kFullCircle);

  densityVsIndex180WFD->SetLineColor(kRed);
  densityVsIndex180WFD->SetMarkerColor(kRed);
  densityVsIndex180WFD->SetMarkerStyle(kFullCircle);

  densityVsIndex200WFD->SetLineColor(kMagenta);
  densityVsIndex200WFD->SetMarkerColor(kMagenta);
  densityVsIndex200WFD->SetMarkerStyle(kFullCircle);

  canvasList.push_back(createCanvas("PairDensityWFD_Pion",canvasConfig));
  densityVsIndex140WFD->SetYTitle("#rho_{2}^{FD}(#alpha|#pi^{+})");
  densityVsIndex140WFD->SetMinimum(1.0E-6);
  densityVsIndex140WFD->SetMaximum(1.0);
  densityVsIndex140WFD->Draw("E");
  densityVsIndex160WFD->Draw("E.SAME");
  densityVsIndex180WFD->Draw("E.SAME");
  densityVsIndex200WFD->Draw("E.SAME");
  gPad->BuildLegend();

  densityVsIndex140WFD = cloneLoadH1(f140,"THG_pair_6");
  densityVsIndex160WFD = cloneLoadH1(f160,"THG_pair_6");
  densityVsIndex180WFD = cloneLoadH1(f180,"THG_pair_6");
  densityVsIndex200WFD = cloneLoadH1(f200,"THG_pair_6");

  densityVsIndex140WFD->SetTitle("140 MeV w/ FD");
  densityVsIndex160WFD->SetTitle("160 MeV w/ FD");
  densityVsIndex180WFD->SetTitle("180 MeV w/ FD");
  densityVsIndex200WFD->SetTitle("200 MeV w/ FD");

  densityVsIndex140WFD->SetLineColor(kBlack);
  densityVsIndex140WFD->SetMarkerColor(kBlack);
  densityVsIndex140WFD->SetMarkerStyle(kFullCircle);

  densityVsIndex160WFD->SetLineColor(kBlue);
  densityVsIndex160WFD->SetMarkerColor(kBlue);
  densityVsIndex160WFD->SetMarkerStyle(kFullCircle);

  densityVsIndex180WFD->SetLineColor(kRed);
  densityVsIndex180WFD->SetMarkerColor(kRed);
  densityVsIndex180WFD->SetMarkerStyle(kFullCircle);

  densityVsIndex200WFD->SetLineColor(kMagenta);
  densityVsIndex200WFD->SetMarkerColor(kMagenta);
  densityVsIndex200WFD->SetMarkerStyle(kFullCircle);

  canvasList.push_back(createCanvas("PairDensityWFD_Kaon",canvasConfig));
  densityVsIndex140WFD->SetYTitle("#rho_{2}^{FD}(#alpha|K^{+})");
  densityVsIndex140WFD->SetMinimum(1.0E-7);
  densityVsIndex140WFD->SetMaximum(1.0E-1);
  densityVsIndex140WFD->Draw("E");
  densityVsIndex160WFD->Draw("E.SAME");
  densityVsIndex180WFD->Draw("E.SAME");
  densityVsIndex200WFD->Draw("E.SAME");
  gPad->BuildLegend();

  densityVsIndex140WFD = cloneLoadH1(f140,"THG_pair_8");
  densityVsIndex160WFD = cloneLoadH1(f160,"THG_pair_8");
  densityVsIndex180WFD = cloneLoadH1(f180,"THG_pair_8");
  densityVsIndex200WFD = cloneLoadH1(f200,"THG_pair_8");

  densityVsIndex140WFD->SetTitle("140 MeV w/ FD");
  densityVsIndex160WFD->SetTitle("160 MeV w/ FD");
  densityVsIndex180WFD->SetTitle("180 MeV w/ FD");
  densityVsIndex200WFD->SetTitle("200 MeV w/ FD");

  densityVsIndex140WFD->SetLineColor(kBlack);
  densityVsIndex140WFD->SetMarkerColor(kBlack);
  densityVsIndex140WFD->SetMarkerStyle(kFullCircle);

  densityVsIndex160WFD->SetLineColor(kBlue);
  densityVsIndex160WFD->SetMarkerColor(kBlue);
  densityVsIndex160WFD->SetMarkerStyle(kFullCircle);

  densityVsIndex180WFD->SetLineColor(kRed);
  densityVsIndex180WFD->SetMarkerColor(kRed);
  densityVsIndex180WFD->SetMarkerStyle(kFullCircle);

  densityVsIndex200WFD->SetLineColor(kMagenta);
  densityVsIndex200WFD->SetMarkerColor(kMagenta);
  densityVsIndex200WFD->SetMarkerStyle(kFullCircle);

  canvasList.push_back(createCanvas("PairDensityWFD_Proton",canvasConfig));
  densityVsIndex140WFD->SetYTitle("#rho_{2}^{FD}(#alpha|p)");
  densityVsIndex140WFD->SetMinimum(1.0E-6);
  densityVsIndex140WFD->SetMaximum(1.0E-1);
  densityVsIndex140WFD->Draw("E");
  densityVsIndex160WFD->Draw("E.SAME");
  densityVsIndex180WFD->Draw("E.SAME");
  densityVsIndex200WFD->Draw("E.SAME");
  gPad->BuildLegend();

  // print all canvas to PDF
  if (false)
    {
    for (auto & canvas : canvasList)
      {
      canvasFileName = directory;
      canvasFileName += canvas->GetName();
      canvasFileName += ".pdf";
      canvas->Print(canvasFileName);
      }
    }

  return 0;
}
