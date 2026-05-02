
unsigned int   gOutputStyle  = 0;
unsigned int   gOutputLength = 50;

void loadLatex(const TString & includeBasePath);
void createPresentation(vector<TCanvas*> canvasList, const TString & outDirectory, const TString & fileName);

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

const TString createName(const TString & s0,
                         const TString & s1,
                         int option=0)
{
  TString separator;
  switch (option)
    {
      default:
      case 0: separator = "_"; break;
      case 1: separator = " "; break;
      case 2: separator = ""; break;
    }
  TString name = s0;
  name += separator;
  name += s1;
  return name;
}

const TString createName(const TString & s0,
                         const TString & s1,
                         const TString & s2,
                         int option=0)
{
  TString separator;
  switch (option)
    {
      default:
      case 0: separator = "_"; break;
      case 1: separator = " "; break;
      case 2: separator = ""; break;
    }
  TString name = s0;
  name += separator;
  name += s1;
  name += separator;
  name += s2;
  return name;
}

const TString createName(const TString & s0,
                         const TString & s1,
                         const TString & s2,
                         const TString & s3,
                         int option=0)
{
  TString separator;
  switch (option)
    {
      default:
      case 0: separator = "_"; break;
      case 1: separator = " "; break;
      case 2: separator = ""; break;
    }
  TString name = s0;
  name += separator;
  name += s1;
  name += separator;
  name += s2;
  name += separator;
  name += s3;
  return name;
}





TH1 * loadH1(TFile * inputFile, const TString & histoName)
{
  TH1 * h = (TH1*) inputFile->Get(histoName);
  if (!h)
    {
    cout << "loadH1() ERROR" << endl;
    cout << "Got a null pointer for histo named " << histoName << endl;
    cout << "From file: " << inputFile->GetName() << endl;
    exit(1);
    }
  return h;
}

TH2 * loadH2(TFile * inputFile, const TString & histoName)
{
  TH2 * h = (TH2*) inputFile->Get(histoName);
  if (!h)
    {
    cout << "loadH2() h is a null pointerfor histo named " << histoName <<  endl; exit(1);
    }
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
TLegend * createLegend(float x1, float y1, float x2, float y2, float fontSize)
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
                       float x1, float x2, float y1, float y2,
                       float fontSize,
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



TLine * createLine(float x1, float y1, float x2, float y2, int style, int color, int width, bool doDraw=true)
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

TLatex * createLabel(const TString & text, double x, double y, double angle, int color,  double fontSize, bool doDraw)
{
  TLatex * label = new TLatex(x,y,text);
  label->SetTextColor(color);
  label->SetTextAngle(angle);
  label->SetTextSize(fontSize);
  if (doDraw) label->Draw();
  return label;
}


class GraphConfiguration
{
public:
  GraphConfiguration();
  GraphConfiguration(const GraphConfiguration & src);
  GraphConfiguration & operator=(const GraphConfiguration & rhs);
  ~GraphConfiguration() {}

  float fontSize;
  int   fontCode;

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

  int   nZDivisions;
  int   zTitleColor;
  int   zTitleFont;
  float zTitleSize;
  float zTitleOffset;
  int   zLabelColor;
  int   zLabelFont;
  float zLabelSize;
  float zLabelOffset;

  void setMyFavorites(int color=1, int markerStyle=20, float markerSize=0.05, unsigned int type=1);

};

//!
//!Default Constructor
//!
GraphConfiguration::GraphConfiguration()
:
fontSize (0.05),
fontCode (40),

lineColor (1),
lineStyle (1),
lineWidth (1),

markerColor (1),
markerStyle (1),
markerSize  (0.05),

nXDivisions  (5),
xTitleColor  (1),
xTitleFont   (fontCode),
xTitleSize   (fontSize),
xTitleOffset (1.1),
xLabelColor  (1),
xLabelFont   (fontCode),
xLabelSize   (fontSize),
xLabelOffset (0.002),

nYDivisions  (5),
yTitleColor  (1),
yTitleFont   (fontCode),
yTitleSize   (fontSize),
yTitleOffset (1.40),
yLabelColor  (1),
yLabelFont   (fontCode),
yLabelSize   (fontSize),
yLabelOffset (0.01),

nZDivisions  (5),
zTitleColor  (1),
zTitleFont   (fontCode),
zTitleSize   (fontSize),
zTitleOffset (0.5),
zLabelColor  (1),
zLabelFont   (fontCode),
zLabelSize   (fontSize),
zLabelOffset (0.01)
{ }

GraphConfiguration::GraphConfiguration(const GraphConfiguration & src)
:
fontSize   (src.fontSize),
fontCode   (src.fontCode),

lineColor  (src.lineColor),
lineStyle  (src.lineStyle),
lineWidth  (src.lineWidth),
markerColor(src.markerColor),
markerStyle(src.markerStyle),
markerSize (src.markerSize),

nXDivisions (src.nXDivisions),
xTitleColor (src.xTitleColor),
xTitleFont  (src.xTitleFont),
xTitleSize  (src.xTitleSize),
xTitleOffset(src.xTitleOffset),
xLabelColor (src.xLabelColor),
xLabelFont  (src.xLabelFont),
xLabelSize  (src.xLabelSize),
xLabelOffset(src.xLabelOffset),

nYDivisions (src.nYDivisions),
yTitleColor (src.yTitleColor),
yTitleFont  (src.yTitleFont),
yTitleSize  (src.yTitleSize),
yTitleOffset(src.yTitleOffset),
yLabelColor (src.yLabelColor),
yLabelFont  (src.yLabelFont),
yLabelSize  (src.yLabelSize),
yLabelOffset(src.yLabelOffset),

nZDivisions (src.nZDivisions),
zTitleColor (src.zTitleColor),
zTitleFont  (src.zTitleFont),
zTitleSize  (src.zTitleSize),
zTitleOffset(src.zTitleOffset),
zLabelColor (src.zLabelColor),
zLabelFont  (src.zLabelFont),
zLabelSize  (src.zLabelSize),
zLabelOffset(src.zLabelOffset)
{ }

//!
//!Assignment operator for GraphConfiguration objects
//!
GraphConfiguration & GraphConfiguration::operator=(const GraphConfiguration & rhs)
{
  if (this!=&rhs)
    {
    fontSize = rhs.fontSize;
    fontCode = rhs.fontCode;

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

    nZDivisions  = rhs.nZDivisions;
    zTitleColor  = rhs.zTitleColor;
    zTitleFont   = rhs.zTitleFont;
    zTitleSize   = rhs.zTitleSize;
    zTitleOffset = rhs.zTitleOffset;
    zLabelColor  = rhs.zLabelColor;
    zLabelFont   = rhs.zLabelFont;
    zLabelSize   = rhs.zLabelSize;
    zLabelOffset = rhs.zLabelOffset;

    }
  return *this;
}

void GraphConfiguration::setMyFavorites(int color, int style, float size, unsigned int type=1)
{
  //  printValue("color",color);
  //  printValue("style",style);
  //  printValue("size",size);
  //  printValue("type",type);

  switch (type)
    {
      default:
      case 1:
      fontSize = 0.05;
      fontCode = 42;

      //      printValue("fontSize",fontSize);
      //      printValue("fontCode",fontCode);


      lineColor = color;
      lineStyle = 1;
      lineWidth = 1;

      markerColor = color;
      markerStyle = style;
      markerSize  = size;

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
      yTitleOffset = 1.40;
      yLabelColor  = 1;
      yLabelFont   = fontCode;
      yLabelSize   = fontSize;
      yLabelOffset = 0.01;
      break;

      case 2:
      fontSize = 0.05;
      fontCode = 10;

      //      printValue("fontSize",fontSize);
      //      printValue("fontCode",fontCode);

      lineColor = color;
      lineStyle = 1;
      lineWidth = 1;

      markerColor = color;
      markerStyle = style;
      markerSize  = size;

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
      yTitleOffset = 1.40;
      yLabelColor  = 1;
      yLabelFont   = fontCode;
      yLabelSize   = fontSize;
      yLabelOffset = 0.01;

      nZDivisions  = 5;
      zTitleColor  = 1;
      zTitleFont   = fontCode;
      zTitleSize   = fontSize;
      zTitleOffset = 0.5;
      zLabelColor  = 1;
      zLabelFont   = fontCode;
      zLabelSize   = fontSize;
      zLabelOffset = 0.01;
      break;
    }


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
  float theta;
  float phi;

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
  logZ(0),
  theta(40.0),
  phi(30.0)
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
  logZ(source.logZ),
  theta(source.theta),
  phi(source.phi)
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
  logX   = rhs.logX;
  logY   = rhs.logY;
  logZ   = rhs.logZ;
  theta  = rhs.theta;
  phi    = rhs.phi;
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

  void setAngles(float v1, float v2)
  {
  theta = v1;
  phi   = v2;
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
  canvas->SetTheta(       configuration.theta);
  canvas->SetPhi(         configuration.phi);
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

  if (h->IsA()==TH2::Class() || h->IsA()==TH2F::Class() || h->IsA()==TH2D::Class() || h->IsA()==TH2I::Class() )
    {
    //    cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
    //    cout << "2D" << endl;
    //    cout << "xTitleSize " << config.xTitleSize << endl;
    //    cout << "yTitleSize " << config.yTitleSize << endl;
    //    cout << "zTitleSize " << config.zTitleSize << endl;
    TAxis * zAxis = (TAxis *) h->GetZaxis();
    zAxis->SetNdivisions(config.nZDivisions);
    zAxis->SetTitleColor(config.zTitleColor);
    zAxis->SetTitleFont(config.zTitleFont);
    zAxis->SetTitleSize(config.zTitleSize);
    zAxis->SetTitleOffset(config.zTitleOffset);
    zAxis->SetLabelColor(config.zLabelColor);
    zAxis->SetLabelFont(config.zLabelFont);
    zAxis->SetLabelSize(config.zLabelSize);
    }
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
               const vector<TH1*> & histos,
               const vector<GraphConfiguration*> & graphConfigs,
               const vector<TString> & labels,
               float xMinimum, float xMaximum,
               float yMinimum, float yMaximum,
               float lx1=0.1, float ly1=0.1, float lx2=0.2, float ly2=0.2, float fontSize=24, int nColumns = 1,
               bool drawLegend=1)
{
  int nh = histos.size();
  int nc = graphConfigs.size();
  int nl = labels.size();
  if (nh < 1)
    {
    cout << "<ERROR> plot(...) histos.size()<1" << endl;
    exit(1);
    }
  if (nh>nc || nh!=nl)
    {
    cout << "<ERROR> plot(...) histos, graphConfigs, and labels have different number of entries " << endl;
    exit(1);
    }
  TCanvas * c = createCanvas(canvasName,canvasConfig,10);
  TLegend * legend = nullptr;
  if (drawLegend) legend = createLegend(lx1,ly1,lx2,ly2,fontSize);
  for (unsigned int k=0;k<histos.size(); k++)
    {
    setHistoProperties(histos[k],*graphConfigs[k]);
    if (k==0)
      {
      histos[0]->GetXaxis()->SetRangeUser(xMinimum,xMaximum);
      histos[0]->SetMinimum(yMinimum);
      histos[0]->SetMaximum(yMaximum);
      if (histos[k]->IsA()==TH2::Class() ||
          histos[k]->IsA()==TH2F::Class() ||
          histos[k]->IsA()==TH2D::Class() ||
          histos[k]->IsA()==TH2I::Class() )
        histos[k]->Draw("COLZ");
      else
        histos[k]->Draw("E");
      }
    else
      histos[k]->Draw("SAME");
    if (drawLegend) legend->AddEntry(histos[k],labels[k],"lep");
    }
  if (drawLegend)
    {
    legend->SetNColumns(nColumns);
    legend->Draw();
    }
  return c;
}

TH1* FindTH1Recursive(TObject* obj, const TString& name)
{
  if (!obj) return nullptr;

  // If it's already a TH1, check name
  if (obj->InheritsFrom(TH1::Class())) {
    TH1* h = (TH1*)obj;
    if (name.IsNull() || name == h->GetName()) return h;
    return nullptr;
  }

  // If it's a pad/canvas, search its primitives
  if (obj->InheritsFrom(TPad::Class())) {
    TPad* p = (TPad*)obj;
    TList* prims = p->GetListOfPrimitives();
    if (!prims) return nullptr;

    TIter it(prims);
    TObject* child = nullptr;
    while ((child = it())) {
      if (TH1* h = FindTH1Recursive(child, name)) return h;
    }
  }

  return nullptr;
}

TH2* FindTH2Recursive(TObject* obj, const TString& name)
{
  if (!obj) return nullptr;

  // If it's already a TH1, check name
  if (obj->InheritsFrom(TH2::Class())) {
    TH2* h = (TH2*)obj;
    if (name.IsNull() || name == h->GetName()) return h;
    return nullptr;
  }

  // If it's a pad/canvas, search its primitives
  if (obj->InheritsFrom(TPad::Class())) {
    TPad* p = (TPad*)obj;
    TList* prims = p->GetListOfPrimitives();
    if (!prims) return nullptr;

    TIter it(prims);
    TObject* child = nullptr;
    while ((child = it())) {
      if (TH2* h = FindTH2Recursive(child, name)) return h;
    }
  }
  return nullptr;
}


TProfile* FindProfileRecursive(TObject* obj, const TString& name)
{
  if (!obj) return nullptr;

  // If it's already a TH1, check name
  if (obj->InheritsFrom(TProfile::Class())) {
    TProfile* h = (TProfile*)obj;
    if (name.IsNull() || name == h->GetName()) return h;
    return nullptr;
  }

  // If it's a pad/canvas, search its primitives
  if (obj->InheritsFrom(TPad::Class())) {
    TPad* p = (TPad*)obj;
    TList* prims = p->GetListOfPrimitives();
    if (!prims) return nullptr;

    TIter it(prims);
    TObject* child = nullptr;
    while ((child = it())) {
      if (TProfile* h = FindProfileRecursive(child, name)) return h;
    }
  }

  return nullptr;
}


TH1* Slice2DHisto (const TH2& toBeSliced ,double tobeSliceAt )
{
  // Find the bin in z2 (X axis) corresponding to z2 = 0.5
  int binx = toBeSliced.GetXaxis()->FindBin(tobeSliceAt);
  int biny = toBeSliced.GetYaxis()->FindBin(tobeSliceAt);
  // Take a 1D slice in Y at that single X bin
  TH1D* slice = toBeSliced.ProjectionY(
                                       Form("%s_z2_%0.2f", toBeSliced.GetName(), tobeSliceAt),
                                       binx, biny
                                       );
  return slice;
}

TH1* loadH1FromCanvas(TFile * file, const TString & canvasName, const TString &  histName)
{
  TObject* obj = file->Get(canvasName);
  if (!obj)
    {
    cout << "CANVAS named " << canvasName << " not found" << endl; exit(1);
    return 0;
    }
  //obj->ls();
  TH1* h = FindTH1Recursive(obj, histName);
  if (!h)
    {
    cout << "Histogram named " << histName << " not found" << endl; exit(1);
    return 0;
    }
  return h;
}

TH2* loadH2FromCanvas(TFile * file, const TString & canvasName, const TString &  histName)
{
  TObject* obj = file->Get(canvasName);
  if (!obj)
    {
    cout << "CANVAS named " << canvasName << " not found" << endl; exit(1);
    return 0;
    }
  //obj->ls();
  TH2* h = FindTH2Recursive(obj, histName);
  if (!h)
    {
    cout << "Histogram named " << histName << " not found" << endl; exit(1);
    return 0;
    }
  return h;
}

TProfile* loadProfileFromCanvas(TFile * file, const TString & canvasName, const TString &  histName)
{
  TObject* obj = file->Get(canvasName);
  if (!obj)
    {
    cout << "CANVAS named " << canvasName << " not found" << endl; exit(1);
    return 0;
    }
  //obj->ls();
  TProfile* h = FindProfileRecursive(obj, histName);
  if (!h)
    {
    cout << "Profile named " << histName << " not found" << endl; exit(1);
    return 0;
    }
  return h;
}

TH1 * convertProfileToH1(TProfile * p)
{
  TString name = p->GetName(); name += "H";
  int n = p->GetNbinsX();
  float minX = p->GetXaxis()->GetXmin();
  float maxX = p->GetXaxis()->GetXmax();
  TH1 * h = new TH1D(name,name, n, minX, maxX);
  for (int k = 1; k<n; k++)
    {
    double c = p->GetBinContent(k);
    double e = p->GetBinError(k);
    h->SetBinContent(k,c);
    h->SetBinError(k,e);
    }
  return h;
}



void printCanvases(vector<TCanvas*> canvasList, const TString & outDirectory)
{
  for (auto & canvas : canvasList)
    {
    TString canvasFileName;
    canvasFileName = outDirectory;
    canvasFileName += canvas->GetName();
    canvasFileName += ".pdf";
    canvas->SaveAs(canvasFileName);
    }
}

//!
//!option 0 : multiplicative scaling
//!option 1 : divide

TH1 * scaleByMult(TH1 * h, unsigned int option=0)
{
  printValue("Cloning histogram named ",h->GetName());
  TH1 * clone = (TH1*) h->Clone();
  unsigned int n = clone->GetNbinsX();
  printValue("N bins (x) ",n);
  for (int k=1; k<=n; k++)
    {
    double c = clone->GetBinCenter(k);
    double v = clone->GetBinContent(k);
    double ev = clone->GetBinError(k);
    //    printValue("v",v);
    //    printValue("c",c);
    //    printValue("v*c",v*c);
    switch (option)
      {
        case 0:
        clone->SetBinContent(k,v*c);
        clone->SetBinError(k,ev*c);
        break;
        case 1:
        clone->SetBinContent(k,v/c);
        clone->SetBinError(k,ev/c);
        break;
      }

    }
  return clone;
}

std::ifstream & openInputAsciiFile(const TString & path,
                                   const TString & name,
                                   const TString & extension,
                                   bool verbose)
{
  TString fileName = path;
  // make sure that if an path is given, it ends with '/'
  int slash = fileName.First('/');
  int len = fileName.Length();
  //  std::cout << slash << std::endl;
  //  std::cout << len << std::endl;
  if (len>0  &&  (len-1)!=slash) fileName += "/";
  fileName += name;
  if (!fileName.EndsWith(extension)) fileName += extension;
  std::ios_base::openmode mode = std::ios_base::in;
  if (verbose)
    {
    printCR();
    printValue("openInputAsciiFile(...) Open input file named",fileName);
    printValue("openInputAsciiFile(...) with option",mode);
    }
  std::ifstream * file = new std::ifstream(fileName.Data(),mode);
  if (!file || !file->is_open())
    {
    cout << "File not found: " <<fileName << endl;
    exit(1);
    }
  if (verbose)
    std::cout << "openInputAsciiFile(...) File: " << fileName << " successfully opened." << std::endl;
  return *file;
}

std::ofstream & openOutputAsciiFile(const TString & path,
                                    const TString & name,
                                    const TString & extension,
                                    bool verbose)
{
  TString fileName = path;
  // make sure that if an path is given, it ends with '/'
  int slash = fileName.First('/');
  int len = fileName.Length();
  //  std::cout << slash << std::endl;
  //  std::cout << len << std::endl;
  if (len>0  &&  (len-1)!=slash) fileName += "/";
  fileName += name;
  if (!fileName.EndsWith(extension)) fileName += extension;
  std::ios_base::openmode mode = std::ios_base::out|std::ios_base::app;
  if (verbose)
    {
    printCR();
    printValue("openInputAsciiFile(...) Open output file named",fileName);
    printValue("openInputAsciiFile(...) with option",mode);
    }
  std::ofstream * file = new std::ofstream(fileName.Data(),mode);
  if (!file || !file->is_open())
    {
    cout << "File not found: " <<fileName << endl;
    exit(1);
    }

  if (verbose)
    std::cout << "openOutputAsciiFile(...) File: " << fileName << " successfully opened." << std::endl;
  return *file;
}


void skipLines(std::ofstream & out, int n=1)
{
  for (int k=0; k<n; k++) out << endl;
}

void createHeader(std::ofstream & out,
                  const TString & theme,
                  const TString & author,
                  const TString & institution,
                  const TString & title,
                  const TString & subtitle="")
{
  out << "\\documentclass{beamer}" << endl;
  out << "\\usetheme{" << theme << "}" << endl;
  skipLines(out,2);
  out << "\\title{" << title << "}" << endl;
  if (subtitle.Length()>0)  out << "\\subtitle{" << subtitle << "}" << endl;
  out << "\\author{" << author << "}" << endl;
  out << "\\institute{" << institution << "}" << endl;
  out << "\\date{\\today}" << endl;
  skipLines(out,2);
  out << "\\begin{document}" << endl;
  out << "\\begin{frame}" << endl;
  out << "\\titlepage" << endl;
  out << "\\end{frame}" << endl;
  skipLines(out,1);
}

void createTrailer(std::ofstream & out)
{
  skipLines(out,2);
  out << "\\end{document}" << endl;
}

void createSection(std::ofstream & out, const TString & name)
{
  skipLines(out,1);
  out << "\\section{" << name << "}" << endl;
  skipLines(out,1);
}

void createSubsection(std::ofstream & out, const TString & name)
{
  skipLines(out,1);
  out << "\\subsection{" << name << "}" << endl;
  skipLines(out,1);
}

void createSubsubsection(std::ofstream & out, const TString & name)
{
  skipLines(out,1);
  out << "\\subsubsection{" << name << "}" << endl;
  skipLines(out,1);
}

void createTextSlide(std::ofstream & out, const TString & title, const TString & text)
{
  skipLines(out,1);
  out << "\\begin{frame}" << endl;
  out << "\\frametitle{" << title << "}" << endl;
  out << text << endl;
  out << "\\end{frame}" << endl;
  skipLines(out,1);
}

void createFigureSlide(std::ofstream & out, const TString & title, const TString & text, const TString & figureFileName)
{
  skipLines(out,1);
  out << "\\begin{frame}" << endl;
  out << "\\frametitle{" << title << "}" << endl;
  out << "\\begin{figure}" << endl;
  out << "\\includegraphics[scale=0.75]{" << figureFileName << "}" << endl;
  out << "\\caption{" << text << "}" << endl;
  out << "\\end{figure}" << endl;
  out << "\\end{frame}" << endl;
  skipLines(out,1);
}


int Plots(bool print=1, bool latex=1)
{
  TString includeBasePath = getenv("CAP_SRC_PATH");
  loadLatex(includeBasePath);

  setDefaultOptions(1);
  CanvasConfiguration canvasConfig;
  CanvasConfiguration canvasConfigLogY;
  CanvasConfiguration canvasConfigLogXLogY;
  CanvasConfiguration canvasConfig2D;
  canvasConfig.setSize(800,800);
  canvasConfigLogY.setSize(800,800);
  canvasConfigLogY.logY = 1;
  canvasConfigLogXLogY.setSize(800,800);
  canvasConfigLogXLogY.logX = 1;
  canvasConfigLogXLogY.logY = 1;

  canvasConfig2D.setSize(800,800);
  canvasConfig2D.logZ = 1;
  canvasConfig2D.topMargin   = 0.05;
  canvasConfig2D.rightMargin = 0.15;
  canvasConfig2D.leftMargin = 0.2;

  float  fontSize = 30;
  int    fontCode = 42;
  float  fontSize2D = 27;
  int    fontCode2D = 42;
  vector<TCanvas*> canvasList;
  vector<TFile*>  files;
  vector<TString> fileNames;
  vector<TString> canvasNames;
  vector<TString> histoNames;
  vector<TString> labels;
  vector<TH1*>    histos;
  vector<GraphConfiguration *> graphConfigs;
  for (unsigned int k=0; k<6; k++) graphConfigs.push_back(new GraphConfiguration());
  float ss = 2.1;
  graphConfigs[0]->setMyFavorites(kBlack,kFullSquare,ss);
  graphConfigs[1]->setMyFavorites(kBlue,kOpenCircle,ss);
  graphConfigs[2]->setMyFavorites(kRed, kOpenCircle,ss);
  graphConfigs[3]->setMyFavorites(kBlue,kOpenCircle,ss);
  graphConfigs[4]->setMyFavorites(kRed,kFullSquare,ss);
  graphConfigs[5]->setMyFavorites(kRed,kOpenCircle,ss);


  TString inputPath      = "/Volumes/ClaudeDisc4/OutputFiles/test/";
  TString outDirectory   = "/Volumes/ClaudeDisc4/OutputFiles/test/Glauber/";
  TString baseName       = "GB";
  fileNames.push_back(TString("Glauber.root"));
  files = openRootFiles(inputPath,fileNames,"OLD",true);
  createDirectory(outDirectory);


  //

  labels.clear();
  labels.push_back(TString("Pb-Pb #sqrt{s}=13 TeV"));
  histos.clear();
  histos.push_back(loadH1(files[0], createName(baseName,"impact")));
  histos[0]->Rebin(5); //histos[0]->Scale(1.0/5.0);
  histos[0]->SetTitle("");
  histos[0]->GetXaxis()->SetTitle("b (fm)");
  histos[0]->GetYaxis()->SetTitle("P(b)");
  canvasList.push_back(plot("ImpactParameter",canvasConfig,histos,graphConfigs,labels,
                            0.0, 20.0, 0.0, 0.2,
                            0.3,0.65,0.55,0.85,fontSize,1,1));


  graphConfigs[0]->setMyFavorites(kBlack,kFullCircle,ss);
  graphConfigs[1]->setMyFavorites(kBlue,kOpenCircle,ss);
  graphConfigs[2]->setMyFavorites(kRed, kOpenCircle,ss);
  labels.clear();
  labels.push_back(TString("A+B"));
  labels.push_back(TString("A"));
  labels.push_back(TString("B"));
  histos.clear();
  histos.push_back(loadH1(files[0], createName(baseName,"nParts")));
  histos.push_back(loadH1(files[0], createName(baseName,"nPartsA")));
  histos.push_back(loadH1(files[0], createName(baseName,"nPartsB")));

  //  float rebin = 5;
  //  histos[0]->Rebin(rebin); histos[0]->Scale(1.0/rebin);
  //  histos[1]->Rebin(rebin); histos[0]->Scale(1.0/rebin);
  //  histos[2]->Rebin(rebin); histos[0]->Scale(1.0/rebin);
  histos[0]->SetTitle("");
  histos[0]->GetXaxis()->SetTitle("N_{parts}");
  histos[0]->GetYaxis()->SetTitle("P(N)");
  canvasList.push_back(plot("nParts",canvasConfigLogY,histos,graphConfigs,labels,
                            0.0, 500.0, 0.001, 0.1,
                            0.7,0.65,0.85,0.85,fontSize,1,1));


  graphConfigs[0]->setMyFavorites(kBlack,kFullSquare,ss);
  graphConfigs[1]->setMyFavorites(kBlue,kOpenSquare,ss);
  graphConfigs[2]->setMyFavorites(kRed, kOpenSquare,ss);
  graphConfigs[3]->setMyFavorites(kBlack,kFullCircle,ss);
  graphConfigs[4]->setMyFavorites(kBlue,kOpenCircle,ss);
  graphConfigs[5]->setMyFavorites(kRed, kOpenCircle,ss);
  labels.clear();
  labels.push_back(TString("protons A+B"));
  labels.push_back(TString("protons A"));
  labels.push_back(TString("protons B"));
  labels.push_back(TString("neutrons A+B"));
  labels.push_back(TString("neutrons A"));
  labels.push_back(TString("neutrons B"));
  histos.clear();
  histos.push_back(loadH1(files[0], createName(baseName,"nPartsProton")));
  histos.push_back(loadH1(files[0], createName(baseName,"nPartsProtonA")));
  histos.push_back(loadH1(files[0], createName(baseName,"nPartsProtonB")));
  histos.push_back(loadH1(files[0], createName(baseName,"nPartsNeutron")));
  histos.push_back(loadH1(files[0], createName(baseName,"nPartsNeutronA")));
  histos.push_back(loadH1(files[0], createName(baseName,"nPartsNeutronB")));
  histos[0]->SetTitle("");
  histos[0]->GetXaxis()->SetTitle("N_{parts}");
  histos[0]->GetYaxis()->SetTitle("P(N)");
  canvasList.push_back(plot("nPartsPN",canvasConfigLogY,histos,graphConfigs,labels,
                            0.0, 500.0, 0.0, 0.2,
                            0.7,0.65,0.85,0.85,fontSize,1,1));

  graphConfigs[0]->setMyFavorites(kBlack,kFullSquare,ss);
  graphConfigs[1]->setMyFavorites(kBlue,kOpenSquare,ss);
  graphConfigs[2]->setMyFavorites(kRed, kOpenSquare,ss);
  labels.clear();
  labels.push_back(TString("A+B"));
  //  labels.push_back(TString("A"));
  //  labels.push_back(TString("B"));
  histos.clear();
  histos.push_back(loadH1(files[0], createName(baseName,"nBinaries")));
  //  histos.push_back(loadH1(files[0], createName(baseName,"nBinariesA")));
  //  histos.push_back(loadH1(files[0], createName(baseName,"nBinariesB")));
  histos[0]->SetTitle("");
  histos[0]->GetXaxis()->SetTitle("N_{colls}");
  histos[0]->GetYaxis()->SetTitle("P(N)");
  canvasList.push_back(plot("nBinaries",canvasConfigLogY,histos,graphConfigs,labels,
                            0.0, 5000.0, 0.0, 0.2,
                            0.7,0.65,0.85,0.85,fontSize,1,1));

  graphConfigs[0]->setMyFavorites(kBlack,kFullSquare,ss);
  graphConfigs[1]->setMyFavorites(kBlue,kOpenSquare,ss);
  graphConfigs[2]->setMyFavorites(kRed, kOpenSquare,ss);
  labels.clear();
  labels.push_back(TString("A+B"));
  labels.push_back(TString("A"));
  labels.push_back(TString("B"));
  histos.clear();
  histos.push_back(loadH1(files[0], createName(baseName,"nBinariesAvgVsImpact")));
  histos.push_back(loadH1(files[0], createName(baseName,"nBinariesAAvgVsImpact")));
  histos.push_back(loadH1(files[0], createName(baseName,"nBinariesBAvgVsImpact")));
  histos[0]->SetTitle("");
  histos[0]->GetXaxis()->SetTitle("b (fm)");
  histos[0]->GetYaxis()->SetTitle("N_{colls}");
  canvasList.push_back(plot("nBinariesVsImpact",canvasConfigLogY,histos,graphConfigs,labels,
                            0.0, 20.0, 0.0, 5.0E3,
                            0.7,0.65,0.85,0.85,fontSize,1,1));

  labels.clear();
  labels.push_back(TString("A+B"));
  histos.clear();
  histos.push_back(loadH1(files[0], createName(baseName,"nPartsVsImpact")));
  histos[0]->SetTitle("");
  histos[0]->GetXaxis()->SetTitle("b (fm)");
  histos[0]->GetYaxis()->SetTitle("N_{part}");
  canvasList.push_back(plot("nPartsVsImpact2D",canvasConfig2D,histos,graphConfigs,labels,
                            0.0, 20.0, 0.0, 500.0,
                            0.7,0.65,0.85,0.85,fontSize2D,1,1));





  labels.clear();
  labels.push_back(TString("A+B"));
  histos.clear();
  histos.push_back(loadH1(files[0], createName(baseName,"nBinariesVsImpact")));
  histos[0]->SetTitle("");
  histos[0]->GetXaxis()->SetTitle("b (fm)");
  histos[0]->GetYaxis()->SetTitle("N_{col}");
  canvasList.push_back(plot("nCollsVsImpact2D",canvasConfig2D,histos,graphConfigs,labels,
                            0.0, 20.0, 0.0, 5000.0,
                            0.7,0.65,0.85,0.85,fontSize,1,1));

  labels.clear();
  labels.push_back(TString("A+B"));
  labels.push_back(TString("A"));
  labels.push_back(TString("B"));
  histos.clear();
  histos.push_back(loadH1(files[0], createName(baseName,"nPartsAvgVsImpact")));
  histos.push_back(loadH1(files[0], createName(baseName,"nPartsAAvgVsImpact")));
  histos.push_back(loadH1(files[0], createName(baseName,"nPartsBAvgVsImpact")));
  histos[0]->SetTitle("");
  histos[0]->GetXaxis()->SetTitle("b (fm)");
  histos[0]->GetYaxis()->SetTitle("<N_{parts}>");
  canvasList.push_back(plot("nPartsVsImpact",canvasConfig,histos,graphConfigs,labels,
                            0.0, 20.0, 0.0, 500.0,
                            0.7,0.65,0.85,0.85,fontSize,1,1));



  labels.clear();
  labels.push_back(TString("protons A+B"));
  labels.push_back(TString("protons A"));
  labels.push_back(TString("protons B"));
  labels.push_back(TString("neutrons A+B"));
  labels.push_back(TString("neutrons A"));
  labels.push_back(TString("neutrons B"));
  histos.clear();
  histos.push_back(loadH1(files[0], createName(baseName,"nPartsProtonAvgVsImpact")));
  histos.push_back(loadH1(files[0], createName(baseName,"nPartsProtonAAvgVsImpact")));
  histos.push_back(loadH1(files[0], createName(baseName,"nPartsProtonBAvgVsImpact")));
  histos.push_back(loadH1(files[0], createName(baseName,"nPartsNeutronAvgVsImpact")));
  histos.push_back(loadH1(files[0], createName(baseName,"nPartsNeutronAAvgVsImpact")));
  histos.push_back(loadH1(files[0], createName(baseName,"nPartsNeutronBAvgVsImpact")));
  histos[0]->SetTitle("");
  histos[0]->GetXaxis()->SetTitle("b (fm)");
  histos[0]->GetYaxis()->SetTitle("<N_{parts}>");
  canvasList.push_back(plot("nPartsVsImpactNP",canvasConfig,histos,graphConfigs,labels,
                            0.0, 20.0, 0.0, 500.0,
                            0.7,0.65,0.85,0.85,fontSize,1,1));



  labels.clear();
  labels.push_back(TString("A+B"));
  labels.push_back(TString("A"));
  labels.push_back(TString("B"));
  histos.clear();
  histos.push_back(loadH1(files[0], createName(baseName,"nSpectAvgVsImpact")));
  histos.push_back(loadH1(files[0], createName(baseName,"nSpectAAvgVsImpact")));
  histos.push_back(loadH1(files[0], createName(baseName,"nSpectBAvgVsImpact")));
  histos[0]->SetTitle("");
  histos[0]->GetXaxis()->SetTitle("N_{spec}");
  histos[0]->GetYaxis()->SetTitle("P(N)");
  canvasList.push_back(plot("nSpectVsImpactNP",canvasConfig,histos,graphConfigs,labels,
                            0.0, 500.0, 0.0, 500.0,
                            0.3,0.65,0.55,0.85,fontSize,1,1));



  labels.clear();
  labels.push_back(TString("protons A+B"));
  labels.push_back(TString("protons A"));
  labels.push_back(TString("protons B"));
  labels.push_back(TString("neutrons A+B"));
  labels.push_back(TString("neutrons A"));
  labels.push_back(TString("neutrons B"));
  histos.clear();
  histos.push_back(loadH1(files[0], createName(baseName,"nSpectProton")));
  histos.push_back(loadH1(files[0], createName(baseName,"nSpectProtonA")));
  histos.push_back(loadH1(files[0], createName(baseName,"nSpectProtonB")));
  histos.push_back(loadH1(files[0], createName(baseName,"nSpectNeutron")));
  histos.push_back(loadH1(files[0], createName(baseName,"nSpectNeutronA")));
  histos.push_back(loadH1(files[0], createName(baseName,"nSpectNeutronB")));
  histos[0]->SetTitle("");
  histos[0]->GetXaxis()->SetTitle("N_{spec}");
  histos[0]->GetYaxis()->SetTitle("P(N)");
  canvasList.push_back(plot("nSpectNP",canvasConfigLogY,histos,graphConfigs,labels,
                            0.0, 500.0, 0.0, 0.3,
                            0.7,0.65,0.85,0.85,fontSize,1,1));

  labels.clear();
  labels.push_back(TString("A+B"));
  labels.push_back(TString("A"));
  labels.push_back(TString("B"));
  histos.clear();
  histos.push_back(loadH1(files[0], createName(baseName,"nSpectAvgVsImpact")));
  histos.push_back(loadH1(files[0], createName(baseName,"nSpectAAvgVsImpact")));
  histos.push_back(loadH1(files[0], createName(baseName,"nSpectBAvgVsImpact")));
  histos[0]->SetTitle("");
  histos[0]->GetXaxis()->SetTitle("b (fm)");
  histos[0]->GetYaxis()->SetTitle("<N_{spect}>");
  canvasList.push_back(plot("nSpectVsImpact",canvasConfig,histos,graphConfigs,labels,
                            0.0, 20.0, 0.0, 500.0,
                            0.3,0.65,0.55,0.85,fontSize,1,1));

  labels.clear();
  labels.push_back(TString("protons A+B"));
  labels.push_back(TString("protons A"));
  labels.push_back(TString("protons B"));
  labels.push_back(TString("neutrons A+B"));
  labels.push_back(TString("neutrons A"));
  labels.push_back(TString("neutrons B"));
  histos.clear();
  histos.push_back(loadH1(files[0], createName(baseName,"nSpectProtonAvgVsImpact")));
  histos.push_back(loadH1(files[0], createName(baseName,"nSpectProtonAAvgVsImpact")));
  histos.push_back(loadH1(files[0], createName(baseName,"nSpectProtonBAvgVsImpact")));
  histos.push_back(loadH1(files[0], createName(baseName,"nSpectNeutronAvgVsImpact")));
  histos.push_back(loadH1(files[0], createName(baseName,"nSpectNeutronAAvgVsImpact")));
  histos.push_back(loadH1(files[0], createName(baseName,"nSpectNeutronBAvgVsImpact")));
  histos[0]->SetTitle("");
  histos[0]->GetXaxis()->SetTitle("b (fm)");
  histos[0]->GetYaxis()->SetTitle("<N_{spect}>");
  canvasList.push_back(plot("nSpectNPVsImpact",canvasConfig,histos,graphConfigs,labels,
                            0.0, 20.0, 0.0, 500.0,
                            0.3,0.65,0.55,0.85,fontSize,1,1));

  labels.clear();
  labels.push_back(TString("N_{pp}"));
  labels.push_back(TString("N_{pn}"));
  labels.push_back(TString("N_{np}"));
  labels.push_back(TString("N_{nn}"));
  labels.push_back(TString("N_{hard}"));
  histos.clear();
  histos.push_back(loadH1(files[0], createName(baseName,"nPP")));
  histos.push_back(loadH1(files[0], createName(baseName,"nPN")));
  histos.push_back(loadH1(files[0], createName(baseName,"nNP")));
  histos.push_back(loadH1(files[0], createName(baseName,"nNN")));
  histos.push_back(loadH1(files[0], createName(baseName,"nHard")));
  histos[0]->SetTitle("");
  histos[0]->GetXaxis()->SetTitle("N");
  histos[0]->GetYaxis()->SetTitle("P(N)");
  canvasList.push_back(plot("probNNInt",canvasConfigLogY,histos,graphConfigs,labels,
                            0.0, 1200.0, 0.0, 0.399,
                            0.7,0.65,0.85,0.85,fontSize,1,1));

  // same labels as above
  histos.clear();
  histos.push_back(loadH1(files[0], createName(baseName,"nPPAvgVsImpact")));
  histos.push_back(loadH1(files[0], createName(baseName,"nPNAvgVsImpact")));
  histos.push_back(loadH1(files[0], createName(baseName,"nNPAvgVsImpact")));
  histos.push_back(loadH1(files[0], createName(baseName,"nNNAvgVsImpact")));
  histos.push_back(loadH1(files[0], createName(baseName,"nHardAvgVsImpact")));
  histos[0]->SetTitle("");
  histos[0]->GetXaxis()->SetTitle("b (fm)");
  histos[0]->GetYaxis()->SetTitle("<N>");
  canvasList.push_back(plot("NXXvsImpact",canvasConfigLogY,histos,graphConfigs,labels,
                            0.0, 20.0, 0.0, 5000.0,
                            0.7,0.65,0.85,0.85,fontSize,1,1));




  labels.clear();
  labels.push_back(TString("#varepsilon_{x,part}"));
  labels.push_back(TString("#varepsilon_{y,part}"));
  labels.push_back(TString("#varepsilon_{part}"));
  histos.clear();
  histos.push_back(loadH1(files[0], createName(baseName,"sysEccX")));
  histos.push_back(loadH1(files[0], createName(baseName,"sysEccY")));
  histos.push_back(loadH1(files[0], createName(baseName,"sysEcc")));
  histos[0]->SetTitle("");
  histos[0]->GetXaxis()->SetTitle("#varepsilon");
  histos[0]->GetYaxis()->SetTitle("P(#varepsilon)");
  canvasList.push_back(plot("SysEccentricity",canvasConfig,histos,graphConfigs,labels,
                            -1.0, 1.0, 0.0, 0.199,
                            0.7,0.65,0.85,0.85,fontSize,1,1));

  labels.clear();
  labels.push_back(TString("<x>_{part}"));
  labels.push_back(TString("<y>_{part}"));
  histos.clear();
  histos.push_back(loadH1(files[0], createName(baseName,"sysMeanXVsImpact")));
  histos.push_back(loadH1(files[0], createName(baseName,"sysMeanYVsImpact")));
  histos[0]->SetTitle("");
  histos[0]->GetXaxis()->SetTitle("b (fm)");
  histos[0]->GetYaxis()->SetTitle("<O> (fm)");
  canvasList.push_back(plot("SysMeansVsImpact",canvasConfig,histos,graphConfigs,labels,
                            0.0, 20.0, -0.2, 0.2,
                            0.7,0.65,0.85,0.85,fontSize,1,1));

  labels.clear();
  labels.push_back(TString("<#Deltax^{2}>^{0.5}"));
  labels.push_back(TString("<#Deltay^{2}>^{0.5}"));
  histos.clear();
  histos.push_back(loadH1(files[0], createName(baseName,"sysRmsXVsImpact")));
  histos.push_back(loadH1(files[0], createName(baseName,"sysRmsYVsImpact")));
  histos[0]->SetTitle("");
  histos[0]->GetXaxis()->SetTitle("b (fm)");
  histos[0]->GetYaxis()->SetTitle("<#DeltaO^{2}>^{0.5}");
  canvasList.push_back(plot("SysRmsVsImpact",canvasConfig,histos,graphConfigs,labels,
                            0.0, 20.0, 0.0, 5.0,
                            0.7,0.65,0.85,0.85,fontSize,1,1));

  labels.clear();
  labels.push_back(TString("#varepsilon_{x,part}"));
  labels.push_back(TString("#varepsilon_{y,part}"));
  labels.push_back(TString("#varepsilon_{part}"));
  histos.clear();
  histos.push_back(loadH1(files[0], createName(baseName,"sysEccXVsImpact")));
  histos.push_back(loadH1(files[0], createName(baseName,"sysEccYVsImpact")));
  histos.push_back(loadH1(files[0], createName(baseName,"sysEccVsImpact")));
  histos[0]->SetTitle("");
  histos[0]->GetXaxis()->SetTitle("b (fm)");
  histos[0]->GetYaxis()->SetTitle("#varepsilon");
  canvasList.push_back(plot("SysEccentricityVsImpact",canvasConfig,histos,graphConfigs,labels,
                            0.0, 20.0, -0.2, 1.5,
                            0.7,0.65,0.85,0.85,fontSize,1,1));

  if (print) printCanvases(canvasList,outDirectory);

  if (latex) createPresentation(canvasList,outDirectory,"GlauberResults");

  return 0;
}

void loadLatex(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Latex/";
  gSystem->Load(includePath+"LatexDocument.hpp");
  gSystem->Load(includePath+"LatexElement.hpp");
  gSystem->Load(includePath+"LatexFigure.hpp");
  gSystem->Load(includePath+"LatexFigureGroup.hpp");
  gSystem->Load(includePath+"LatexFrame.hpp");
  gSystem->Load(includePath+"LatexList.hpp");
  gSystem->Load(includePath+"LatexSection.hpp");
  gSystem->Load(includePath+"LatexSubSection.hpp");
  gSystem->Load(includePath+"LatexText.hpp");
  gSystem->Load("libLatex.dylib");
}

void createPresentation(vector<TCanvas*> canvasList, const TString & outDirectory, const TString & fileName)
{
  CAP::LatexDocument doc;
  doc.setDocumentClassName("beamer");
  doc.setThemeName("Warsaw");
  doc.setAuthor("C. Pruneau");
  doc.setInstitution("Wayne State University");
  doc.setTitle("Selected Glauber Calculations");
  doc.setSubtitle("A New CAP Module");
  doc.setDate("");
  doc.setUseToday(true);
  doc.setOutputPath(outDirectory);
  doc.setOutFileName(fileName);

  CAP::LatexSection * goals   = new CAP::LatexSection(); goals->setName("Goals");  doc.addChild(goals);
  CAP::LatexFrame   * goalsF  = new CAP::LatexFrame();   goalsF->setName("Goals");  doc.addChild(goalsF);
  goalsF->addText("Testing our new GlauberMC Code");
  goalsF->addEquation("E=mc^2","eq:emc2");

  CAP::LatexSection * methods = new CAP::LatexSection(); methods->setName("Methods");  doc.addChild(methods);
  CAP::LatexFrame   * methodsF = new CAP::LatexFrame();  methodsF->setName("Methods");  doc.addChild(methodsF);
  std::vector<TString> list;
  list.push_back(TString("Method 1"));
  list.push_back(TString("Method 2"));
  list.push_back(TString("Method 3"));
  list.push_back(TString("Method 4"));
  methodsF->addList(list);
  CAP::LatexSection * results = new CAP::LatexSection(); results->setName("Results");  doc.addChild(results);

  for (auto & canvas : canvasList)
    {
    TString name = canvas->GetName();
    TString canvasFileName;
    canvasFileName = outDirectory;
    canvasFileName += name;
    canvasFileName += ".pdf";

    CAP::LatexFrame * frame = new CAP::LatexFrame(); frame->setName(name);  doc.addChild(frame);
    CAP::LatexFigure * fig = new CAP::LatexFigure();
    fig->setFileName(canvasFileName);
    fig->setCaption("caption to be added here");
    fig->setLabel(name);
    frame->addChild(fig);

    }

  CAP::LatexSection * conclusions = new CAP::LatexSection(); conclusions->setName("Conclusions");  doc.addChild(conclusions);
  doc.create();

}
