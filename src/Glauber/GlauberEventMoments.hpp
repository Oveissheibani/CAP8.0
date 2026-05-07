#ifndef CAP__GlauberEventMoments
#define CAP__GlauberEventMoments
#include "PrintHelpers.hpp"

namespace CAP
{

class GlauberEventMoments
{
public:

  GlauberEventMoments();
  GlauberEventMoments(const GlauberEventMoments & analyzer);
  GlauberEventMoments operator=(const GlauberEventMoments & analyzer);
  virtual ~GlauberEventMoments();
  virtual void initialize(int nOrder=4);
  virtual void reset();
  virtual void resetPass() { _pass = 0;}
  virtual void clear();
  virtual void fill(double x, double y, double weight=1.0);
  virtual void calculateAverages();
  virtual void save();
  virtual void print() const;

  bool _doN;
  bool _doDensity;
  bool _doArea;
  bool _doLength;

  int    _pass;
  int    _nOrders;
  double _sum;
  double _xSum;
  double _ySum;
  double _x2Sum;
  double _y2Sum;
  double _xySum;
  double _meanX;
  double _meanY;
  double _meanX2;
  double _meanY2;
  double _meanXY;
  double _varX;
  double _varY;
  double _varXY;
  double _rmsX;
  double _rmsY;
  double _eccX;
  double _eccY;
  double _ecc;
  double _psi;

  std::vector<double> _cosPhiN;
  std::vector<double> _sinPhiN;
  std::vector<double> _rN;
  std::vector<double> _psiN;
  std::vector<double> _eccN;

  ClassDef(GlauberEventMoments,0)
};

} // namespace CAP

#endif

