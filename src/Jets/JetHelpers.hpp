#ifndef CAP__JetHelpers
#define CAP__JetHelpers
#include "TVector3.h"

namespace CAP
{

double calculateJt(TVector3 a, TVector3 b);

void calculateJtTheta(double jet_px,
                      double jet_py,
                      double jet_pz,
                      double jet_p,
                      double part_px,
                      double part_py,
                      double part_pz,
                      double part_p,
                      double & part_jt,
                      double & part_theta);

} // namespace CAP
#endif // JetSingleHistos_HPP
