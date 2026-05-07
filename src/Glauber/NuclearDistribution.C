void createDirectory(const TString  & dirName)
{
  gSystem->mkdir(dirName,1);
}


//!
//! Create a canvas
//!
TCanvas * createCanvas(const TString  & canvasName, int inc, int xSize=700, int ySize=700)
{
  TCanvas * canvas = new TCanvas(canvasName,canvasName,inc, 20, xSize, ySize);
  canvas->SetLogx(false);
  canvas->SetLogy(false);
  canvas->SetLogz(false);
  canvas->SetRightMargin(0.10);
  canvas->SetLeftMargin(0.20);
  canvas->SetBottomMargin(0.20);
  canvas->SetTopMargin(0.05);
  canvas->SetTicky(true);
  canvas->SetTickx(true);
  canvas->SetGridx(false);
  canvas->SetGridy(false);
//  canvas->SetTheta(0.0);
//  canvas->SetPhi(0.0);
//  canvas->SetFillColor( kWhite );
//  canvas->SetFillStyle(   configuration.getValueInt("windowFillStyle") );
//  canvas->SetBorderSize(  configuration.getValueInt("windowBorderSize") );
//  canvas->SetBorderMode(  configuration.getValueInt("windowBorderMode") );
  return canvas;
}

void printCanvas(TCanvas * canvas,
                 const TString  & outputPath,
                 bool printGif=false,
                 bool printPdf=true,
                 bool printSvg=false,
                 bool printPng=false,
                 bool printC=false)
{
  createDirectory(outputPath);
  TString  fileName = outputPath;
  fileName += "/";
  fileName += canvas->GetName();
  if (printGif) canvas->Print(fileName+".gif");
  if (printPdf) canvas->Print(fileName+".pdf");
  if (printSvg) canvas->Print(fileName+".svg");
  if (printPng) canvas->Print(fileName+".png");
  if (printC)   canvas->Print(fileName+".C");
}


TF1 * createNucleonDistribution( const  TString  & name,
                                 double radius,
                                 double skin,
                                 double w,
                                 double beta2,
                                 double beta4,
                                 double a,
                                 double b,
                                 double r0,
                                 double r1,
                                 double r2,
                                 double maximumRadius,
                                 int radialDistType)
{
  TF1 * f1;
  TF2 * f2;
  bool gen = false;
  switch (radialDistType)
    {
      case 0: // exponential profile
      if (gen)
        f1 = new TF1(name,"x*x*exp(-x/[0])",0.0,maximumRadius);
      else
        f1 = new TF1(name,"exp(-x/[0])",0.0,maximumRadius);
      f1->SetParameter(0,radius);
      break;

      case 1: // 3pF
      if (gen)
        f1 = new TF1(name,"x*x*(1+[2]*(x/[0])**2)/(1+exp((x-[0])/[1]))",0.0,maximumRadius);
      else
        f1 = new TF1(name,"(1+[2]*(x/[0])**2)/(1+exp((x-[0])/[1]))",0.0,maximumRadius);
      f1->SetParameters(radius,skin,w);
      break;

      case 2: // 3pG
      if (gen)
        f1 = new TF1(name,"x*x*(1+[2]*(x/[0])**2)/(1+exp((x**2-[0]**2)/[1]**2))",0.0,maximumRadius);
      else
        f1 = new TF1(name,"(1+[2]*(x/[0])**2)/(1+exp((x**2-[0]**2)/[1]**2))",0.0,maximumRadius);
      f1->SetParameters(radius,skin,w);
      break;

      case 3: // useHulthen (see nucl-ex/0603010)
      case 4: // same but constrain the neutron opposite to the proton event-by-event
      if (true)
        f1 = new TF1(name,"2*[0]*[1]*([0]+[1])*pow(exp(-[0]*x)-exp(-[1]*x),2)/(([0]-[1])*([0]-[1]))",0.0,maximumRadius);
      else
        f1 = new TF1(name,"4*pi*([0]*[1]*([0]+[1]))/(2*pi*(pow([0]-[1],2)))*pow((exp(-[0]*x)-exp(-[1]*x))/x,2)",0.0,maximumRadius);
      cout << "a:" << a << endl;
      cout << "b:" << b << endl;
      f1->SetParameters(a,b);
      break;

      case 5: // Ellipsoid (e.g., Uranium)
      if (gen)
        f1 = new TF1(name,"x*x*(1+[2]*(x/[0])**2)/(1+exp((x-[0])/[1]))",0.0,maximumRadius);
      else
        f1 = new TF1(name,"(1+[2]*(x/[0])**2)/(1+exp((x-[0])/[1]))",0.0,maximumRadius);
      f1->SetParameters(radius,skin,0.0); // same as 3pF but setting W to zero
      break;

      case 6: // He3/H3 -- not implemented
      break;

      case 7: // Deformed nuclei, box method -- not implemented
      break;

      case 8: // Deformed nuclei, TF2 method
      if (gen)
        f2 = new TF2(name,"x*x*TMath::Sin(y)/(1+exp((x-[0]*(1+[2]*0.315*(3*pow(cos(y),2)-1.0)+[3]*0.105*(35*pow(cos(y),4)-30*pow(cos(y),2)+3)))/[1]))",0.0,maximumRadius, 0.0,3.1415927);
      else
        f2 = new TF2(name,"TMath::Sin(y)/(1+exp((x-[0]*(1+[2]*0.315*(3*pow(cos(y),2)-1.0)+[3]*0.105*(35*pow(cos(y),4)-30*pow(cos(y),2)+3)))/[1]))",0.0,maximumRadius, 0.0,3.1415927);
      f2->SetParameters(radius,skin,beta2,beta4);
      break;

      case 9: // Proton gaus
      if (gen)
        f1  = new TF1(name,"x*x*exp(-0.5*(x/[0])*(x/[0]))",0.0,maximumRadius);
      else
        f1  = new TF1(name,"exp(-0.5*(x/[0])*(x/[0]))",0.0,maximumRadius);

      f1->SetParameter(0,skin);
      break;

      case 10: // Proton dgaus
      if (gen)
        f1 = new TF1(name,"x*x*((1-[0])/[1]^3*exp(-x*x/[1]/[1])+[0]/(0.4*[1])^3*exp(-x*x/(0.4*[1])^2))",0.0,maximumRadius);
      else
        f1 = new TF1(name,"((1-[0])/[1]^3*exp(-x*x/[1]/[1])+[0]/(0.4*[1])^3*exp(-x*x/(0.4*[1])^2))",0.0,maximumRadius);

      f1->SetParameter(0,0.5);
      f1->SetParameter(1,skin);
      break;

      case 11: // 3pF for proton
      if (gen)
        f1 = new TF1(name,"x*x*(1+[2]*(x/[0])**2)/(1+exp((x-[0])/[1]))",0,maximumRadius);
      else
        f1 = new TF1(name,"(1+[2]*(x/[0])**2)/(1+exp((x-[0])/[1]))",0,maximumRadius);

      f1->SetParameters(radius,skin,w);
      break;

      case 12: // reweighted
      if (gen)
        f1 = new TF1(name,"x*x*(1+[2]*(x/[0])**2)/(1+exp((x-[0])/[1]))/([3]+[4]*x+[5]*x^2)",0,maximumRadius);
      else
        f1 = new TF1(name,"(1+[2]*(x/[0])**2)/(1+exp((x-[0])/[1]))/([3]+[4]*x+[5]*x^2)",0,maximumRadius);
      f1->SetParameters(radius,skin,w,r0,r1,r2);
      break;

      case 13: // Pb for proton and neutrons reweighted
      if (gen)
        f1  = new TF1(name,"x*x*(1+[2]*(x/[0])**2)/(1+exp((x-[0])/[1]))/([3]+[4]*x+[5]*x^2)",0,maximumRadius);
      else
        f1  = new TF1(name,"(1+[2]*(x/[0])**2)/(1+exp((x-[0])/[1]))/([3]+[4]*x+[5]*x^2)",0,maximumRadius);
      f1->SetParameters(radius,skin,w,1.00866,-0.000461484,-0.000203571);
      break;

      case 14: // Deformed nuclei, TF2 method, reweighted
      if (gen)
        f2 = new TF2(name,"x*x*TMath::Sin(y)/(1+exp((x-[0]*(1+[2]*0.315*(3*pow(cos(y),2)-1.0)+[3]*0.105*(35*pow(cos(y),4)-30*pow(cos(y),2)+3)))/[1]))/([4]+[5]*x+[6]*x^2)",0.0,maximumRadius,0.0,3.1415927);
      else
        f2 = new TF2(name,"TMath::Sin(y)/(1+exp((x-[0]*(1+[2]*0.315*(3*pow(cos(y),2)-1.0)+[3]*0.105*(35*pow(cos(y),4)-30*pow(cos(y),2)+3)))/[1]))/([4]+[5]*x+[6]*x^2)",0.0,maximumRadius,0.0,3.1415927);
      f2->SetParameters(radius,skin,beta2,beta4,r0,r1,r2);
      break;

      case 15: // harmonic oscillator model
      if (gen)
        f1 = new TF1(name,"x^2*(1+[0]*(x^2/[1]^2))*exp(-x^2/[1]^2)",0.0,maximumRadius);
      else
        f1 = new TF1(name,"(1+[0]*(x^2/[1]^2))*exp(-x^2/[1]^2)",0.0,maximumRadius);
      f1->SetParameters(radius,skin);
      break;

      default:
      cout << "Unknown radial type!!!!" << endl;
      exit(1);
    }
  return f1;
}

TLatex * createLabel(const TString & text, double x, double y, double angle, int color,  double fontSize, bool doDraw=true)
{
  TLatex * label = new TLatex(x,y,text);
  label->SetTextColor(color);
  label->SetTextAngle(angle);
  label->SetTextSize(fontSize);
  if (doDraw) label->Draw();
  return label;
}


void NuclearDistribution()
{
  TCanvas * c1 = createCanvas("NuclearProfilesCuAuPb", 20);

  double cuRadius = 4.20;
  double cuSkin   = 0.596;
  int    cuType   = 1;

  double auRadius = 6.38;
  double auSkin   = 0.535;
  int    auType   = 1;

  double pbRadius = 6.62;
  double pbSkin   = 0.546;
  double pbW      = 0;
  int    pbType   = 1;


  TF1 * copper = createNucleonDistribution("^{63}Cu",cuRadius,cuSkin,0,0,0,0,0,0,0,0,10.0,cuType);
  TF1 * gold = createNucleonDistribution("^{197}Au",auRadius,auSkin,0,0,0,0,0,0,0,0,10.0,auType);
  TF1 * lead = createNucleonDistribution("^{208}Pb",pbRadius,pbSkin,0,0,0,0,0,0,0,0,10.0,pbType);
  copper->GetXaxis()->SetTitle("r (fm)");
  copper->GetXaxis()->SetNdivisions(5);
  copper->GetXaxis()->SetTitleSize(0.05);
  copper->GetXaxis()->SetTitleOffset(1.09);
  copper->GetXaxis()->SetLabelSize(0.04);
  copper->GetXaxis()->SetLabelOffset(0.01);
  copper->GetYaxis()->SetTitle("#rho(r)/#rho(0)");
  copper->GetYaxis()->SetNdivisions(5);
  copper->GetYaxis()->SetTitleSize(0.05);
  copper->GetYaxis()->SetTitleOffset(1.3);
  copper->GetYaxis()->SetLabelSize(0.04);
  copper->GetYaxis()->SetLabelOffset(0.01);
  copper->SetMaximum(1.199);
  copper->SetLineColor(kRed);
  copper->SetTitle("");
  copper->Draw();

  gold->SetLineColor(kBlue);
  gold->Draw("SAME");
  lead->SetLineColor(kBlack);
  lead->Draw("SAME");

  // createLabel(const TString & text, double x, double y, double angle, int color,  double fontSize, bool doDraw=true)
  TLatex latex;
  latex.SetTextSize(0.05);
  latex.SetTextAlign(13);  //align at top
  latex.SetTextColor(kRed);
  latex.DrawLatex(1.8,0.8," ^{64}Cu");
  latex.SetTextColor(kBlue);
  latex.DrawLatex(3.9,0.8," ^{197}Au");
  latex.SetTextColor(kBlack);
  latex.DrawLatex(5.9,0.8," ^{208}Pb");
  latex.DrawLatex(8.0,1.1,"(a)");

  TCanvas * c2 = createCanvas("NuclearProfileDeuteron", 800);
  double a = 0.2283;
  double b   = 1.1765;
  int    dType   = 3;



  TF1 * d = createNucleonDistribution("Deuteron",0,0,0,0,0,a,b,0,0,0,10.0,dType);
  d->GetXaxis()->SetTitle("r_{pn} (fm)");
  d->GetXaxis()->SetNdivisions(5);
  d->GetXaxis()->SetTitleSize(0.05);
  d->GetXaxis()->SetTitleOffset(1.09);
  d->GetXaxis()->SetLabelSize(0.04);
  d->GetXaxis()->SetLabelOffset(0.01);
  d->GetYaxis()->SetTitle("P(r_{pn})");
  d->GetYaxis()->SetNdivisions(5);
  d->GetYaxis()->SetTitleSize(0.05);
  d->GetYaxis()->SetTitleOffset(1.5);
  d->GetYaxis()->SetLabelSize(0.04);
  d->GetYaxis()->SetLabelOffset(0.01);
  d->SetLineColor(kBlack);
  d->SetTitle("");
  d->SetMaximum(0.299);
  d->Draw();
  latex.DrawLatex(4.5,0.2," deuteron");
  latex.DrawLatex(8.0,0.273,"(b)");


//  createLabel(, 0.50,0.5,0.0,kRed,0.25);
//  createLabel("^{63}Cu", 5.50,0.5,0.0,kBlue,0.25);

  printCanvas(c1,"/Users/aa7526/Documents/MyPublications/HighEnergyNuclearPhysics/HEP/Figures/GlauberModel/");
  printCanvas(c2,"/Users/aa7526/Documents/MyPublications/HighEnergyNuclearPhysics/HEP/Figures/GlauberModel/");

}
