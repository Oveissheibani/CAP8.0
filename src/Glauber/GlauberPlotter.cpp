#include "GlauberPlotter.hpp"

using CAP::MATH::pi;


namespace CAP
{
  void draw(const GlauberEvent & event)
  {
  int partLineColor = kRed;
  int partLineStyle = 1;
  int partLineWidth = 1;
  int partFillColor = kRed;
  int partFillStyle = 1002;

  int spectLineColor = kBlue;
  int spectLineStyle = 1;
  int spectLineWidth = 1;
  int spectFillColor = kBlue;
  int spectFillStyle = 1001;

  GlauberNucleus & nucleusA = event.nucleusA();
  GlauberNucleus & nucleusB = event.nucleusB();

  double r = 0.5*std::sqrt(xs/pi()/10.);
  TEllipse spectEllipse;
  spectEllipse.SetLineStyle(spectLineStyle);
  spectEllipse.SetLineWidth(spectLineWidth);
  spectEllipse.SetFillStyle(spectFillStyle);
  spectEllipse.setFillColor(spectFillColor);
  spectEllipse.setFillStyle(spectFillStyle);

  TEllipse partEllipse;
  partEllipse.SetLineStyle(partLineStyle);
  partEllipse.SetLineWidth(partLineWidth);
  partEllipse.SetFillStyle(partFillStyle);
  partEllipse.setFillColor(spectFillColor);
  partEllipse.setFillStyle(spectFillStyle);

  for (auto & nucleon : nucleusA.nucleons())
    {
    VectorLorentz & pos = nucleon.position();
    if (nucleon->isSpectator())
      spectEllipse.DrawEllipse(pos.x(),pos.y(),r,r,0,360,0,"");
    else
      partEllipse.DrawEllipse(pos.x(),pos.y(),r,r,0,360,0,"");
    }
  for (auto & nucleon : nucleusB.nucleons())
    {
    VectorLorentz & pos = nucleon.position();
    if (nucleon->isSpectator())
      spectEllipse.DrawEllipse(pos.x(),pos.y(),r,r,0,360,0,"");
    else
      partEllipse.DrawEllipse(pos.x(),pos.y(),r,r,0,360,0,"");
    }
  }




#endif



} // namespace CAP
