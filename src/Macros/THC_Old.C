
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



TH1 * cloneLoadH1(TFile * inputFile, TString histoName)
{
  TH1 * h = (TH1*) inputFile->Get(histoName);
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
  legend->SetTextSize(42);
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


void setHistoProperties(TH1 * h,
                        int markerColor, int markerStyle, float markerSize,
                        int fontStyle, float fontSize,
                        float xLabelOffset, float xTitleOffset,
                        float yLabelOffset, float yTitleOffset)
{

  h->SetLineStyle(1);
  h->SetLineColor(markerColor);
  h->SetLineWidth(1);
  h->SetMarkerColor(markerColor);
  h->SetMarkerStyle(markerStyle);
  h->SetMarkerSize(markerSize);

  TAxis * xAxis = (TAxis *) h->GetXaxis();
  xAxis->SetNdivisions(5);
  xAxis->SetTitleSize(fontSize);
  xAxis->SetTitleOffset(xTitleOffset);
  xAxis->SetLabelSize(fontSize);
  xAxis->SetLabelOffset(xLabelOffset);
  TAxis * yAxis = (TAxis *) h->GetYaxis();
  yAxis->SetNdivisions(5);
  yAxis->SetTitleSize(fontSize);
  yAxis->SetTitleOffset(yTitleOffset);
  yAxis->SetLabelSize(fontSize);
  yAxis->SetLabelOffset(yLabelOffset);

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
  canvasConfig.logY = true;

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
  vector<TFile*> files;
  vector<TString> fileNames;
  vector<TString> labels;
  vector<int> color;
  vector<int> markerStyle;

  TString path = "/Users/aa7526/Documents/GitHub/CAP8.0/build/";
  fileNames.push_back(TString("THG_140.root"));
  fileNames.push_back(TString("THG_160.root"));
  fileNames.push_back(TString("THG_180.root"));
  fileNames.push_back(TString("THG_200.root"));
  labels.push_back(TString("140 MeV"));
  labels.push_back(TString("160 MeV"));
  labels.push_back(TString("180 MeV"));
  labels.push_back(TString("200 MeV"));

  color.push_back(kBlack); markerStyle.push_back(kFullSquare);
  color.push_back(kBlue); markerStyle.push_back(kFullSquare);
  color.push_back(kRed); markerStyle.push_back(kFullCircle);
  color.push_back(kMagenta); markerStyle.push_back(kFullCircle);

  for (unsigned int iFile=0; iFile<fileNames.size(); iFile++)
    files.push_back(openOldRootFile(path,fileNames[iFile],true));


  TH1 * h;
  vector<TH1*> thermalDensityVsIndex;
  vector<TH1*> thermalDensityVsMass;
  vector<TH1*> relThermalDensityVsIndex;
  vector<TH1*> singleDensityVsIndex;
  vector<TH1*> singleDensityWFDVsIndex;
  vector<TH1*> pairDensityWFD_2;
  vector<TH1*> pairDensityWFD_3;
  vector<TH1*> pairDensityWFD_4;
  vector<TH1*> pairDensityWFD_5;
  vector<TH1*> pairDensityWFD_6;
  vector<TH1*> pairDensityWFD_7;
  vector<TH1*> pairDensityWFD_8;
  vector<TH1*> pairDensityWFD_9;
  vector<TH1*> pairDensityWFD_10;
  vector<TH1*> pairDensityWFD_11;
  vector<TH1*> pairDensityWFD_12;

  vector<TH1*> normPairDensityWFD_3;
  vector<TH1*> normPairDensityWFD_7;
  vector<TH1*> normPairDensityWFD_9;


  float markerSize = 1.5;
  double pairMin;
  double pairMax;

  for (unsigned int iFile=0; iFile<fileNames.size(); iFile++)
    {
    h = cloneLoadH1(files[iFile],"THG_numberDensity");
    h->SetTitle(labels[iFile]);
    h->SetYTitle("#rho_{1}^{TH}(#alpha)");
    h->SetLineColor(color[iFile]);
    h->SetMarkerColor(color[iFile]);
    h->SetMarkerStyle(markerStyle[iFile]);
    h->SetMarkerSize(markerSize);
    thermalDensityVsIndex.push_back(h);

    h = cloneLoadH1(files[iFile],"THG_numberDensityVsM");
    h->SetTitle(labels[iFile]);
    h->SetYTitle("#rho_{1}^{TH}(#alpha)");
    h->SetLineColor(color[iFile]);
    h->SetMarkerColor(color[iFile]);
    h->SetMarkerStyle(markerStyle[iFile]);
    h->SetMarkerSize(markerSize);
    thermalDensityVsMass.push_back(h);

    h = cloneLoadH1(files[iFile],"THG_relNumberDensity");
    h->SetTitle(labels[iFile]);
    h->SetYTitle("#rho_{1}^{TH}(#alpha)/#rho_{1}^{TH}(#pi)");
    h->SetLineColor(color[iFile]);
    h->SetMarkerColor(color[iFile]);
    h->SetMarkerStyle(markerStyle[iFile]);
    h->SetMarkerSize(markerSize);
    relThermalDensityVsIndex.push_back(h);

    h = cloneLoadH1(files[iFile],"THG_singleDensityVsIndex");
    h->SetTitle(labels[iFile]);
    h->SetYTitle("#rho_{1}^{TH}(#alpha)");
    h->SetLineColor(color[iFile]);
    h->SetMarkerColor(color[iFile]);
    h->SetMarkerStyle(markerStyle[iFile]);
    h->SetMarkerSize(markerSize);
    singleDensityVsIndex.push_back(h);

    h = cloneLoadH1(files[iFile],"THG_singleDensityFDVsIndex");
    h->SetTitle(labels[iFile]);
    h->SetYTitle("#rho_{1}^{TH+FD}(#alpha)");
    h->SetLineColor(color[iFile]);
    h->SetMarkerColor(color[iFile]);
    h->SetMarkerStyle(markerStyle[iFile]);
    h->SetMarkerSize(markerSize);
    singleDensityWFDVsIndex.push_back(h);

    h = cloneLoadH1(files[iFile],"THG_pair_2");
    h->SetTitle(labels[iFile]);
    h->SetYTitle("#rho_{2}^{TH+FD}(#alpha|#pi^{0})");
    h->SetLineColor(color[iFile]);
    h->SetMarkerColor(color[iFile]);
    h->SetMarkerStyle(markerStyle[iFile]);
    h->SetMarkerSize(markerSize);
    pairDensityWFD_2.push_back(h);

    h = cloneLoadH1(files[iFile],"THG_pair_3");
    h->SetTitle(labels[iFile]);
    h->SetYTitle("#rho_{2}^{TH+FD}(#alpha|#pi^{+})");
    h->SetLineColor(color[iFile]);
    h->SetMarkerColor(color[iFile]);
    h->SetMarkerStyle(markerStyle[iFile]);
    h->SetMarkerSize(markerSize);
    pairDensityWFD_3.push_back(h);

    h = cloneLoadH1(files[iFile],"THG_pair_4");
    h->SetTitle(labels[iFile]);
    h->SetYTitle("#rho_{2}^{TH+FD}(#alpha|#pi^{-})");
    h->SetLineColor(color[iFile]);
    h->SetMarkerColor(color[iFile]);
    h->SetMarkerStyle(markerStyle[iFile]);
    h->SetMarkerSize(markerSize);
    pairDensityWFD_4.push_back(h);

    h = cloneLoadH1(files[iFile],"THG_pair_5");
    h->SetTitle(labels[iFile]);
    h->SetYTitle("#rho_{2}^{TH+FD}(#alpha|K^{0})");
    h->SetLineColor(color[iFile]);
    h->SetMarkerColor(color[iFile]);
    h->SetMarkerStyle(markerStyle[iFile]);
    h->SetMarkerSize(markerSize);
    pairDensityWFD_5.push_back(h);

    h = cloneLoadH1(files[iFile],"THG_pair_6");
    h->SetTitle(labels[iFile]);
    h->SetYTitle("#rho_{2}^{TH+FD}(#alpha|#bar{K}^{0})");
    h->SetLineColor(color[iFile]);
    h->SetMarkerColor(color[iFile]);
    h->SetMarkerStyle(markerStyle[iFile]);
    h->SetMarkerSize(markerSize);
    pairDensityWFD_6.push_back(h);

    h = cloneLoadH1(files[iFile],"THG_pair_7");
    h->SetTitle(labels[iFile]);
    h->SetYTitle("#rho_{2}^{TH+FD}(#alpha|K^{+})");
    h->SetLineColor(color[iFile]);
    h->SetMarkerColor(color[iFile]);
    h->SetMarkerStyle(markerStyle[iFile]);
    h->SetMarkerSize(markerSize);
    pairDensityWFD_7.push_back(h);

    h = cloneLoadH1(files[iFile],"THG_pair_8");
    h->SetTitle(labels[iFile]);
    h->SetYTitle("#rho_{2}^{TH+FD}(#alpha|K^{-})");
    h->SetLineColor(color[iFile]);
    h->SetMarkerColor(color[iFile]);
    h->SetMarkerStyle(markerStyle[iFile]);
    h->SetMarkerSize(markerSize);
    pairDensityWFD_8.push_back(h);

    h = cloneLoadH1(files[iFile],"THG_pair_9");
    h->SetTitle(labels[iFile]);
    h->SetYTitle("#rho_{2}^{TH+FD}(#alpha|p)");
    h->SetLineColor(color[iFile]);
    h->SetMarkerColor(color[iFile]);
    h->SetMarkerStyle(markerStyle[iFile]);
    h->SetMarkerSize(markerSize);
    pairDensityWFD_9.push_back(h);

    h = cloneLoadH1(files[iFile],"THG_pair_10");
    h->SetTitle(labels[iFile]);
    h->SetYTitle("#rho_{2}^{TH+FD}(#alpha|#bar{p})");
    h->SetLineColor(color[iFile]);
    h->SetMarkerColor(color[iFile]);
    h->SetMarkerStyle(markerStyle[iFile]);
    h->SetMarkerSize(markerSize);
    pairDensityWFD_10.push_back(h);

    h = cloneLoadH1(files[iFile],"THG_normPair_3");
    h->SetTitle(labels[iFile]);
    h->SetYTitle("#rho_{2,Norm}^{TH+FD}(#alpha|#pi^{+})");
    h->SetLineColor(color[iFile]);
    h->SetMarkerColor(color[iFile]);
    h->SetMarkerStyle(markerStyle[iFile]);
    h->SetMarkerSize(markerSize);
    normPairDensityWFD_3.push_back(h);

    h = cloneLoadH1(files[iFile],"THG_normPair_7");
    h->SetTitle(labels[iFile]);
    h->SetYTitle("#rho_{2,Norm}^{TH+FD}(#alpha|K^{+})");
    h->SetLineColor(color[iFile]);
    h->SetMarkerColor(color[iFile]);
    h->SetMarkerStyle(markerStyle[iFile]);
    h->SetMarkerSize(markerSize);
    normPairDensityWFD_7.push_back(h);

    h = cloneLoadH1(files[iFile],"THG_normPair_9");
    h->SetTitle(labels[iFile]);
    h->SetYTitle("#rho_{2,Norm}^{TH+FD}(#alpha|p)");
    h->SetLineColor(color[iFile]);
    h->SetMarkerColor(color[iFile]);
    h->SetMarkerStyle(markerStyle[iFile]);
    h->SetMarkerSize(markerSize);
    normPairDensityWFD_9.push_back(h);

    }

  int fontSize = 32;
  int fontCode = 42;
  float yTitleOffset = 1.6;
  float yLabelOffset = 0.03;
  float div1 = 0.69;
  float div2 = 0.37;
  TCanvas * cc;
  TPad *pad1;
  TPad *pad2;
  TPad *pad3;

  canvasList.push_back(createCanvas("ThermalDensityVsTempVsIndex",canvasConfig));

  thermalDensityVsIndex[0]->GetXaxis()->SetTitle("#alpha");
  thermalDensityVsIndex[0]->GetXaxis()->SetLabelFont(fontCode);
  thermalDensityVsIndex[0]->GetXaxis()->SetLabelSize(fontSize+10); // labels will be 20 pixels
  thermalDensityVsIndex[0]->GetXaxis()->SetLabelOffset(0.03);
  thermalDensityVsIndex[0]->GetXaxis()->SetTitleFont(fontCode);
  thermalDensityVsIndex[0]->GetXaxis()->SetTitleSize(fontSize);
  thermalDensityVsIndex[0]->GetXaxis()->SetTitleOffset(0.4);
  thermalDensityVsIndex[0]->GetYaxis()->SetLabelFont(fontCode);
  thermalDensityVsIndex[0]->GetYaxis()->SetLabelSize(fontSize);
  thermalDensityVsIndex[0]->GetYaxis()->SetLabelOffset(yLabelOffset);
  thermalDensityVsIndex[0]->GetYaxis()->SetTitleFont(fontCode);
  thermalDensityVsIndex[0]->GetYaxis()->SetTitleSize(fontSize);
  thermalDensityVsIndex[0]->GetYaxis()->SetTitleOffset(yTitleOffset);
  thermalDensityVsIndex[0]->SetMinimum(1.0E-6);
  thermalDensityVsIndex[0]->SetMaximum(2.0);
  thermalDensityVsIndex[0]->Draw("E");
  for (unsigned int iFile = 1; iFile<fileNames.size(); iFile++)
    thermalDensityVsIndex[iFile]->Draw("E.SAME");
  TLegend * legend00 = createLegend(0.65, 0.6, 0.8, 0.9, 45);
  legend00->AddEntry(thermalDensityVsIndex[0],"140 MeV","lep");
  legend00->AddEntry(thermalDensityVsIndex[1],"160 MeV","lep");
  legend00->AddEntry(thermalDensityVsIndex[2],"180 MeV","lep");
  legend00->AddEntry(thermalDensityVsIndex[3],"200 MeV","lep");
  legend00->Draw();


  canvasList.push_back(createCanvas("ThermalDensityVsTempVsMass",canvasConfig));
  thermalDensityVsMass[0]->GetXaxis()->SetTitle("#alpha");
  thermalDensityVsMass[0]->GetXaxis()->SetLabelFont(fontCode);
  thermalDensityVsMass[0]->GetXaxis()->SetLabelSize(fontSize+10); // labels will be 20 pixels
  thermalDensityVsMass[0]->GetXaxis()->SetLabelOffset(0.03);
  thermalDensityVsMass[0]->GetXaxis()->SetTitleFont(fontCode);
  thermalDensityVsMass[0]->GetXaxis()->SetTitleSize(fontSize);
  thermalDensityVsMass[0]->GetXaxis()->SetTitleOffset(0.4);
  thermalDensityVsMass[0]->GetYaxis()->SetLabelFont(fontCode);
  thermalDensityVsMass[0]->GetYaxis()->SetLabelSize(fontSize);
  thermalDensityVsMass[0]->GetYaxis()->SetLabelOffset(yLabelOffset);
  thermalDensityVsMass[0]->GetYaxis()->SetTitleFont(fontCode);
  thermalDensityVsMass[0]->GetYaxis()->SetTitleSize(fontSize);
  thermalDensityVsMass[0]->GetYaxis()->SetTitleOffset(yTitleOffset);
  thermalDensityVsMass[0]->SetMinimum(1.0E-6);
  thermalDensityVsMass[0]->SetMaximum(2.0);
  thermalDensityVsMass[0]->Draw("E");
  for (unsigned int iFile = 1; iFile<fileNames.size(); iFile++)
    thermalDensityVsMass[iFile]->Draw("E.SAME");
  TLegend * legend01 = createLegend(0.6, 0.6, 0.85, 0.9, 45);
  legend01->AddEntry(thermalDensityVsMass[0],"140 MeV","lep");
  legend01->AddEntry(thermalDensityVsMass[1],"160 MeV","lep");
  legend01->AddEntry(thermalDensityVsMass[2],"180 MeV","lep");
  legend01->AddEntry(thermalDensityVsMass[3],"200 MeV","lep");
  legend01->Draw();
//      canvasList.push_back(createCanvas("ThermalDensityVsTempVsIndexRelToPion",canvasConfig));
//      relThermalDensityVsIndex[0]->SetMinimum(1.0E-7);
//      relThermalDensityVsIndex[0]->SetMaximum(1.0);
//      relThermalDensityVsIndex[0]->Draw("E");
//      for (unsigned int iFile = 1; iFile<fileNames.size(); iFile++)
//        relThermalDensityVsIndex[iFile]->Draw("E.SAME");
//      gPad->BuildLegend();
    
//      canvasList.push_back(createCanvas("StableThermalDensityVsTempVsIndex",canvasConfig));
//      singleDensityVsIndex[0]->SetMinimum(1.0E-4);
//      singleDensityVsIndex[0]->SetMaximum(1.0);
//      singleDensityVsIndex[0]->Draw("E");
//      for (unsigned int iFile = 1; iFile<fileNames.size(); iFile++)
//        singleDensityVsIndex[iFile]->Draw("E.SAME");
//      gPad->BuildLegend();
//    
//    
//      canvasList.push_back(createCanvas("StableThermalDensityWFDVsTempVsIndex",canvasConfig));
//      singleDensityWFDVsIndex[0]->SetMinimum(1.0E-4);
//      singleDensityWFDVsIndex[0]->SetMaximum(1.0);
//      singleDensityWFDVsIndex[0]->Draw("E");
//      for (unsigned int iFile = 1; iFile<fileNames.size(); iFile++)
//        singleDensityWFDVsIndex[iFile]->Draw("E.SAME");
//      gPad->BuildLegend();

  if (false)
    {
      pairMin = 1.0E-6;
      pairMax = 1.0;
      canvasList.push_back(createCanvas("PairDensityWFD_2VsTempVsIndex",canvasConfig));
      pairDensityWFD_2[0]->SetMinimum(pairMin);
      pairDensityWFD_2[0]->SetMaximum(pairMax);
      pairDensityWFD_2[0]->Draw("E");
      for (unsigned int iFile = 1; iFile<fileNames.size(); iFile++)
        pairDensityWFD_2[iFile]->Draw("E.SAME");
      gPad->BuildLegend();
    
      canvasList.push_back(createCanvas("PairDensityWFD_3VsTempVsIndex",canvasConfig));
      pairDensityWFD_3[0]->SetMinimum(pairMin);
      pairDensityWFD_3[0]->SetMaximum(pairMax);
      pairDensityWFD_3[0]->Draw("E");
      for (unsigned int iFile = 1; iFile<fileNames.size(); iFile++)
        pairDensityWFD_3[iFile]->Draw("E.SAME");
      gPad->BuildLegend();
    
      canvasList.push_back(createCanvas("PairDensityWFD_4VsTempVsIndex",canvasConfig));
      pairDensityWFD_4[0]->SetMinimum(pairMin);
      pairDensityWFD_4[0]->SetMaximum(pairMax);
      pairDensityWFD_4[0]->Draw("E");
      for (unsigned int iFile = 1; iFile<fileNames.size(); iFile++)
        pairDensityWFD_4[iFile]->Draw("E.SAME");
      gPad->BuildLegend();
    
      canvasList.push_back(createCanvas("PairDensityWFD_5VsTempVsIndex",canvasConfig));
      pairDensityWFD_5[0]->SetMinimum(pairMin);
      pairDensityWFD_5[0]->SetMaximum(pairMax);
      pairDensityWFD_5[0]->Draw("E");
      for (unsigned int iFile = 1; iFile<fileNames.size(); iFile++)
        pairDensityWFD_5[iFile]->Draw("E.SAME");
      gPad->BuildLegend();
    
      canvasList.push_back(createCanvas("PairDensityWFD_6VsTempVsIndex",canvasConfig));
      pairDensityWFD_6[0]->SetMinimum(pairMin);
      pairDensityWFD_6[0]->SetMaximum(pairMax);
      pairDensityWFD_6[0]->Draw("E");
      for (unsigned int iFile = 1; iFile<fileNames.size(); iFile++)
        pairDensityWFD_6[iFile]->Draw("E.SAME");
      gPad->BuildLegend();
    
      canvasList.push_back(createCanvas("PairDensityWFD_7VsTempVsIndex",canvasConfig));
      pairDensityWFD_7[0]->SetMinimum(pairMin);
      pairDensityWFD_7[0]->SetMaximum(pairMax);
      pairDensityWFD_7[0]->Draw("E");
      for (unsigned int iFile = 1; iFile<fileNames.size(); iFile++)
        pairDensityWFD_7[iFile]->Draw("E.SAME");
      gPad->BuildLegend();
    
      canvasList.push_back(createCanvas("PairDensityWFD_8VsTempVsIndex",canvasConfig));
      pairDensityWFD_8[0]->SetMinimum(pairMin);
      pairDensityWFD_8[0]->SetMaximum(pairMax);
      pairDensityWFD_8[0]->Draw("E");
      for (unsigned int iFile = 1; iFile<fileNames.size(); iFile++)
        pairDensityWFD_8[iFile]->Draw("E.SAME");
      gPad->BuildLegend();
    
      canvasList.push_back(createCanvas("PairDensityWFD_9VsTempVsIndex",canvasConfig));
      pairDensityWFD_9[0]->SetMinimum(pairMin);
      pairDensityWFD_9[0]->SetMaximum(pairMax);
      pairDensityWFD_9[0]->Draw("E");
      for (unsigned int iFile = 1; iFile<fileNames.size(); iFile++)
        pairDensityWFD_9[iFile]->Draw("E.SAME");
      gPad->BuildLegend();
    
      canvasList.push_back(createCanvas("PairDensityWFD_10VsTempVsIndex",canvasConfig));
      pairDensityWFD_10[0]->SetMinimum(pairMin);
      pairDensityWFD_10[0]->SetMaximum(pairMax);
      pairDensityWFD_10[0]->Draw("E");
      for (unsigned int iFile = 1; iFile<fileNames.size(); iFile++)
        pairDensityWFD_10[iFile]->Draw("E.SAME");
      gPad->BuildLegend();
    
      pairMin = 0.0001;
      pairMax = 1.0;
    
      canvasList.push_back(createCanvas("NormPairDensityWFD_3VsTempVsIndex",canvasConfig));
      normPairDensityWFD_3[0]->SetMinimum(pairMin);
      normPairDensityWFD_3[0]->SetMaximum(pairMax);
      normPairDensityWFD_3[0]->Draw("E");
      for (unsigned int iFile = 1; iFile<fileNames.size(); iFile++)
        normPairDensityWFD_3[iFile]->Draw("E.SAME");
      gPad->BuildLegend();
    
      canvasList.push_back(createCanvas("NormPairDensityWFD_7VsTempVsIndex",canvasConfig));
      normPairDensityWFD_7[0]->SetMinimum(pairMin);
      normPairDensityWFD_7[0]->SetMaximum(pairMax);
      normPairDensityWFD_7[0]->Draw("E");
      for (unsigned int iFile = 1; iFile<fileNames.size(); iFile++)
        normPairDensityWFD_7[iFile]->Draw("E.SAME");
      gPad->BuildLegend();
    
      canvasList.push_back(createCanvas("NormPairDensityWFD_9VsTempVsIndex",canvasConfig));
      normPairDensityWFD_9[0]->SetMinimum(pairMin);
      normPairDensityWFD_9[0]->SetMaximum(pairMax);
      normPairDensityWFD_9[0]->Draw("E");
      for (unsigned int iFile = 1; iFile<fileNames.size(); iFile++)
        normPairDensityWFD_9[iFile]->Draw("E.SAME");
      gPad->BuildLegend();
    }




  // Stable
  div1 = 0.55;
  cc = createCanvas("StableDensityVsTempVsIndex",canvasConfig);
  canvasList.push_back(cc);
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
  pad2->SetBottomMargin(0.200);
  pad2->SetTopMargin(   0.001);
  pad2->Draw();

  pairMin = 1.01E-4;
  pairMax = 1.9999;
  pad1->cd();
  singleDensityVsIndex[0]->GetYaxis()->SetLabelFont(fontCode);
  singleDensityVsIndex[0]->GetYaxis()->SetLabelSize(fontSize);
  singleDensityVsIndex[0]->GetYaxis()->SetLabelOffset(yLabelOffset);
  singleDensityVsIndex[0]->GetYaxis()->SetTitleFont(fontCode);
  singleDensityVsIndex[0]->GetYaxis()->SetTitleSize(fontSize);
  singleDensityVsIndex[0]->GetYaxis()->SetTitleOffset(yTitleOffset);
  singleDensityVsIndex[0]->SetMinimum(pairMin);
  singleDensityVsIndex[0]->SetMaximum(pairMax);
  singleDensityVsIndex[0]->Draw("E");
  for (unsigned int iFile = 1; iFile<fileNames.size(); iFile++)
    singleDensityVsIndex[iFile]->Draw("E.SAME");
  pad2->cd();
  singleDensityWFDVsIndex[0]->GetXaxis()->SetTitle("#alpha");
  singleDensityWFDVsIndex[0]->GetXaxis()->SetLabelFont(fontCode);
  singleDensityWFDVsIndex[0]->GetXaxis()->SetLabelSize(fontSize+10); // labels will be 20 pixels
  singleDensityWFDVsIndex[0]->GetXaxis()->SetLabelOffset(0.03);
  singleDensityWFDVsIndex[0]->GetXaxis()->SetTitleFont(fontCode);
  singleDensityWFDVsIndex[0]->GetXaxis()->SetTitleSize(fontSize);
  singleDensityWFDVsIndex[0]->GetXaxis()->SetTitleOffset(0.4);
  singleDensityWFDVsIndex[0]->GetYaxis()->SetLabelFont(fontCode);
  singleDensityWFDVsIndex[0]->GetYaxis()->SetLabelOffset(yLabelOffset);
  singleDensityWFDVsIndex[0]->GetYaxis()->SetLabelSize(fontSize); // labels will be 20 pixels
  singleDensityWFDVsIndex[0]->GetYaxis()->SetTitleSize(fontSize);
  singleDensityWFDVsIndex[0]->GetYaxis()->SetTitleFont(fontCode);
  singleDensityWFDVsIndex[0]->GetYaxis()->SetTitleOffset(yTitleOffset);
  singleDensityWFDVsIndex[0]->SetMinimum(pairMin);
  singleDensityWFDVsIndex[0]->SetMaximum(pairMax);
  singleDensityWFDVsIndex[0]->Draw("E");
  for (unsigned int iFile = 1; iFile<fileNames.size(); iFile++)
    singleDensityWFDVsIndex[iFile]->Draw("E.SAME");
  TLegend * legend0 = createLegend(0.25, 0.2, 0.45, 0.55, 45);
  legend0->AddEntry(singleDensityWFDVsIndex[0],"140 MeV","lep");
  legend0->AddEntry(singleDensityWFDVsIndex[1],"160 MeV","lep");
  legend0->AddEntry(singleDensityWFDVsIndex[2],"180 MeV","lep");
  legend0->AddEntry(singleDensityWFDVsIndex[3],"200 MeV","lep");
  legend0->Draw();

  // Pair Densities
  div1 = 0.69;
  div2 = 0.37;

  cc = createCanvas("PairDensityWFD_G3VsTempVsIndex",canvasConfig);
  canvasList.push_back(cc);

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
  pad3->SetLogy(true);
  pad3->SetRightMargin( 0.02);
  pad3->SetLeftMargin(  0.15);
  pad3->SetBottomMargin(0.20);
  pad3->SetTopMargin(   0.001);
  pad3->Draw();


  pairMin = 1.01E-6;
  pairMax = 0.9999;
  yTitleOffset = 2.0;
  pad1->cd();
  pairDensityWFD_3[0]->GetYaxis()->SetLabelFont(fontCode);
  pairDensityWFD_3[0]->GetYaxis()->SetLabelSize(fontSize);
  pairDensityWFD_3[0]->GetYaxis()->SetLabelOffset(yLabelOffset);
  pairDensityWFD_3[0]->GetYaxis()->SetTitleFont(fontCode);
  pairDensityWFD_3[0]->GetYaxis()->SetTitleSize(fontSize);
  pairDensityWFD_3[0]->GetYaxis()->SetTitleOffset(yTitleOffset);
  pairDensityWFD_3[0]->SetMinimum(pairMin);
  pairDensityWFD_3[0]->SetMaximum(pairMax);
  pairDensityWFD_3[0]->SetNdivisions(5,"Y");
  pairDensityWFD_3[0]->Draw("E");
  for (unsigned int iFile = 1; iFile<fileNames.size(); iFile++)
    pairDensityWFD_3[iFile]->Draw("E.SAME");

  pairMin = 1.01E-7;
  pairMax = 0.09999;

  pad2->cd();
  pairDensityWFD_7[0]->GetYaxis()->SetLabelFont(fontCode);
  pairDensityWFD_7[0]->GetYaxis()->SetLabelSize(fontSize);
  pairDensityWFD_7[0]->GetYaxis()->SetLabelOffset(yLabelOffset);
  pairDensityWFD_7[0]->GetYaxis()->SetTitleFont(fontCode);
  pairDensityWFD_7[0]->GetYaxis()->SetTitleSize(fontSize);
  pairDensityWFD_7[0]->GetYaxis()->SetTitleOffset(yTitleOffset);
  pairDensityWFD_7[0]->SetMinimum(pairMin);
  pairDensityWFD_7[0]->SetMaximum(pairMax);
  pairDensityWFD_7[0]->SetNdivisions(5,"Y");
  pairDensityWFD_7[0]->Draw("E");
  for (unsigned int iFile = 1; iFile<fileNames.size(); iFile++)
    pairDensityWFD_7[iFile]->Draw("E.SAME");

  pairMin = 1.01E-6;
  pairMax = 0.9999;

  pad3->cd();
  pairDensityWFD_9[0]->GetXaxis()->SetLabelFont(fontCode);
  pairDensityWFD_9[0]->GetXaxis()->SetLabelSize(fontSize+10); // labels will be 20 pixels
  pairDensityWFD_9[0]->GetXaxis()->SetLabelOffset(0.03);
  pairDensityWFD_9[0]->GetXaxis()->SetTitleFont(fontCode);
  pairDensityWFD_9[0]->GetXaxis()->SetTitleSize(fontSize);
  pairDensityWFD_9[0]->GetXaxis()->SetTitleOffset(0.4);
  pairDensityWFD_9[0]->GetYaxis()->SetLabelFont(fontCode);
  pairDensityWFD_9[0]->GetYaxis()->SetLabelOffset(yLabelOffset);
  pairDensityWFD_9[0]->GetYaxis()->SetLabelSize(fontSize); // labels will be 20 pixels
  pairDensityWFD_9[0]->GetYaxis()->SetTitleFont(fontCode);
  pairDensityWFD_9[0]->GetYaxis()->SetTitleSize(fontSize);
  pairDensityWFD_9[0]->GetYaxis()->SetTitleOffset(yTitleOffset);
  pairDensityWFD_9[0]->SetMinimum(pairMin);
  pairDensityWFD_9[0]->SetMaximum(pairMax);
  pairDensityWFD_9[0]->SetNdivisions(5,"Y");
  pairDensityWFD_9[0]->Draw("E");
  for (unsigned int iFile = 1; iFile<fileNames.size(); iFile++)
    pairDensityWFD_9[iFile]->Draw("E.SAME");
 // gPad->BuildLegend();

  TLegend * legend1 = createLegend(0.75, 0.35, 0.90, 0.9, 45);
  legend1->AddEntry(pairDensityWFD_9[0],"140 MeV","lep");
  legend1->AddEntry(pairDensityWFD_9[1],"160 MeV","lep");
  legend1->AddEntry(pairDensityWFD_9[2],"180 MeV","lep");
  legend1->AddEntry(pairDensityWFD_9[3],"200 MeV","lep");
  legend1->Draw();

  // Normalized Pair Densities

  cc = createCanvas("NormPairDensityWFD_G3VsTempVsIndex",canvasConfig);
  canvasList.push_back(cc);
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
  pad3->SetLogy(true);
  pad3->SetRightMargin( 0.02);
  pad3->SetLeftMargin(  0.15);
  pad3->SetBottomMargin(0.20);
  pad3->SetTopMargin(   0.001);
  pad3->Draw();

  pairMin = 0.2000E-3;
  pairMax = 2.0000;

  pad1->cd();
  normPairDensityWFD_3[0]->GetYaxis()->SetLabelFont(fontCode);
  normPairDensityWFD_3[0]->GetYaxis()->SetLabelSize(fontSize);
  normPairDensityWFD_3[0]->GetYaxis()->SetLabelOffset(yLabelOffset);
  normPairDensityWFD_3[0]->GetYaxis()->SetTitleFont(fontCode);
  normPairDensityWFD_3[0]->GetYaxis()->SetTitleSize(fontSize);
  normPairDensityWFD_3[0]->GetYaxis()->SetTitleOffset(yTitleOffset);
  normPairDensityWFD_3[0]->SetMinimum(pairMin);
  normPairDensityWFD_3[0]->SetMaximum(pairMax);
  normPairDensityWFD_3[0]->SetNdivisions(5,"Y");
  normPairDensityWFD_3[0]->Draw("E");
  for (unsigned int iFile = 1; iFile<fileNames.size(); iFile++)
    normPairDensityWFD_3[iFile]->Draw("E.SAME");

//  pairMin = 1.01E-7;
//  pairMax = 0.09999;

  pad2->cd();
  normPairDensityWFD_7[0]->GetYaxis()->SetLabelFont(fontCode);
  normPairDensityWFD_7[0]->GetYaxis()->SetLabelSize(fontSize);
  normPairDensityWFD_7[0]->GetYaxis()->SetLabelOffset(yLabelOffset);
  normPairDensityWFD_7[0]->GetYaxis()->SetTitleFont(fontCode);
  normPairDensityWFD_7[0]->GetYaxis()->SetTitleSize(fontSize);
  normPairDensityWFD_7[0]->GetYaxis()->SetTitleOffset(yTitleOffset);
  normPairDensityWFD_7[0]->SetMinimum(pairMin);
  normPairDensityWFD_7[0]->SetMaximum(pairMax);
  normPairDensityWFD_7[0]->SetNdivisions(5,"Y");
  normPairDensityWFD_7[0]->Draw("E");
  for (unsigned int iFile = 1; iFile<fileNames.size(); iFile++)
    normPairDensityWFD_7[iFile]->Draw("E.SAME");

//  pairMin = 1.01E-6;
//  pairMax = 0.9999;

  pad3->cd();
  normPairDensityWFD_9[0]->GetXaxis()->SetLabelFont(fontCode);
  normPairDensityWFD_9[0]->GetXaxis()->SetLabelSize(fontSize+10); // labels will be 20 pixels
  normPairDensityWFD_9[0]->GetXaxis()->SetLabelOffset(0.03);
  normPairDensityWFD_9[0]->GetXaxis()->SetTitleFont(fontCode);
  normPairDensityWFD_9[0]->GetXaxis()->SetTitleSize(fontSize);
  normPairDensityWFD_9[0]->GetXaxis()->SetTitleOffset(0.4);
  normPairDensityWFD_9[0]->GetYaxis()->SetLabelFont(fontCode);
  normPairDensityWFD_9[0]->GetYaxis()->SetLabelOffset(yLabelOffset);
  normPairDensityWFD_9[0]->GetYaxis()->SetLabelSize(fontSize); // labels will be 20 pixels
  normPairDensityWFD_9[0]->GetYaxis()->SetTitleFont(fontCode);
  normPairDensityWFD_9[0]->GetYaxis()->SetTitleSize(fontSize);
  normPairDensityWFD_9[0]->GetYaxis()->SetTitleOffset(yTitleOffset);
  normPairDensityWFD_9[0]->SetMinimum(pairMin);
  normPairDensityWFD_9[0]->SetMaximum(pairMax);
  normPairDensityWFD_9[0]->SetNdivisions(5,"Y");
  normPairDensityWFD_9[0]->Draw("E");
  for (unsigned int iFile = 1; iFile<fileNames.size(); iFile++)
    normPairDensityWFD_9[iFile]->Draw("E.SAME");
  // gPad->BuildLegend();

  TLegend * legend2 = createLegend(0.75, 0.35, 0.90, 0.9, 45);
  legend2->AddEntry(pairDensityWFD_9[0],"140 MeV","lep");
  legend2->AddEntry(pairDensityWFD_9[1],"160 MeV","lep");
  legend2->AddEntry(pairDensityWFD_9[2],"180 MeV","lep");
  legend2->AddEntry(pairDensityWFD_9[3],"200 MeV","lep");
  legend2->Draw();



  // print all canvas to PDF
  if (0)
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
