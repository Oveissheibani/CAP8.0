#include "JetHelpers.hpp"

namespace CAP
{

double computeJt(TVector3 partP, TVector3 jetP )
{
  double jetMod   = jetP.Mag();
  double dotProdN = jetP.Dot(partP)/jetMod;
  double jetPxHat =  jetP.Px()/jetMod;
  double jetPyHat =  jetP.Py()/jetMod;
  double jetPzHat =  jetP.Pz()/jetMod;
  double jTx = partP.X() - dotProdN*jetPxHat;
  double jTy = partP.Y() - dotProdN*jetPyHat;
  double jTz = partP.Z() - dotProdN*jetPzHat;
  return sqrt( jTx*jTx + jTy*jTy + jTz*jTz );
}

void calculateJtTheta(double jet_px,
                      double jet_py,
                      double jet_pz,
                      double jet_p,
                      double part_px,
                      double part_py,
                      double part_pz,
                      double part_p,
                      double & part_jt,
                      double & part_theta)
{
  double jetPxHat =  jet_px/jet_p;
  double jetPyHat =  jet_py/jet_p;
  double jetPzHat =  jet_pz/jet_p;
  double dotProdN =  jetPxHat*part_px + jetPyHat*part_py + jetPzHat*part_pz;
  double jTx = part_px - dotProdN*jetPxHat;
  double jTy = part_py - dotProdN*jetPyHat;
  double jTz = part_pz - dotProdN*jetPzHat;
  part_jt = sqrt( jTx*jTx + jTy*jTy + jTz*jTz );
  part_theta = acos(dotProdN/part_p);
}

} // namespace CAP
