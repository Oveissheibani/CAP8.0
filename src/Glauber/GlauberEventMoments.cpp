#include "GlauberEventMoments.hpp"
#include "GlauberExceptions.hpp"
#include "PrintHelpers.hpp"
#include "MathBasicFunctions.hpp"
#include "MathConstants.hpp"
ClassImp(CAP::GlauberEventMoments)

using CAP::MATH::pi;
using CAP::MATH::twoPi;
using CAP::MATH::power;


namespace CAP
{

  GlauberEventMoments::GlauberEventMoments()
  :
  _doN(false),
  _doDensity(false),
  _doArea(false),
  _doLength(false),
  _pass(0),
  _nOrders(4),
  _sum(0.0),
  _xSum(0.0),
  _ySum(0.0),
  _x2Sum(0.0),
  _y2Sum(0.0),
  _xySum(0.0),
  _meanX(0.0),
  _meanY(0.0),
  _meanX2(0.0),
  _meanY2(0.0),
  _meanXY(0.0),
  _varX(0.0),
  _varY(0.0),
  _varXY(0.0),
  _rmsX(0.0),
  _rmsY(0.0),
  _eccX(0.0),
  _eccY(0.0),
  _ecc(0.0),
  _psi(0.0),
  _cosPhiN(),
  _sinPhiN(),
  _rN(),
  _psiN(),
  _eccN()
  {
  reset();
  }

  GlauberEventMoments::GlauberEventMoments(const GlauberEventMoments & source)
  :
  _doN(source._doN),
  _doDensity(source._doDensity),
  _doArea(source._doArea),
  _doLength(source._doLength),
  _pass(source._pass),
  _nOrders(source._nOrders),
  _sum(source._sum),
  _xSum(source._xSum),
  _ySum(source._ySum),
  _x2Sum(source._x2Sum),
  _y2Sum(source._y2Sum),
  _xySum(source._xySum),
  _meanX(source._meanX),
  _meanY(source._meanY),
  _meanX2(source._meanX2),
  _meanY2(source._meanY2),
  _meanXY(source._meanXY),
  _varX(source._varX),
  _varY(source._varY),
  _varXY(source._varXY),
  _rmsX(source._rmsX),
  _rmsY(source._rmsY),
  _eccX(source._eccX),
  _eccY(source._eccY),
  _ecc(source._ecc),
  _psi(source._psi),
  _cosPhiN(source._cosPhiN),
  _sinPhiN(source._sinPhiN),
  _rN(source._rN),
  _psiN(source._psiN),
  _eccN(source._eccN)
  {    }

  GlauberEventMoments GlauberEventMoments::operator=(const GlauberEventMoments & rhs)
  {
  if (this!=&rhs)
    {
    _doN       = rhs._doN;
    _doDensity = rhs._doDensity;
    _doArea    = rhs._doArea;
    _doLength  = rhs._doLength;
    _pass      = rhs._pass;
    _nOrders = rhs._nOrders;
    _sum     = rhs._sum;
    _xSum    = rhs._xSum;
    _ySum    = rhs._ySum;
    _x2Sum   = rhs._x2Sum;
    _y2Sum   = rhs._y2Sum;
    _xySum   = rhs._xySum;
    _meanX   = rhs._meanX;
    _meanY   = rhs._meanY;
    _meanX2  = rhs._meanX2;
    _meanY2  = rhs._meanY2;
    _meanXY  = rhs._meanXY;
    _varX    = rhs._varX;
    _varY    = rhs._varY;
    _varXY   = rhs._varXY;
    _rmsX    = rhs._rmsX;
    _rmsY    = rhs._rmsY;
    _eccX    = rhs._eccX;
    _eccY    = rhs._eccY;
    _ecc     = rhs._ecc;
    _psi     = rhs._psi;
    _cosPhiN = rhs._cosPhiN;
    _sinPhiN = rhs._sinPhiN;
    _rN      = rhs._rN;
    _psiN    = rhs._psiN;
    _eccN    = rhs._eccN;
    }
  return *this;
  }

  GlauberEventMoments::~GlauberEventMoments()
  {
  clear();
  }

  void GlauberEventMoments::fill(double x, double y, double weight)
  {
  _sum +=  weight;
  if (_pass==0)
    {
    _xSum  +=  x*weight;
    _ySum  +=  y*weight;
    _x2Sum +=  x*x*weight;
    _y2Sum +=  y*y*weight;
    _xySum +=  x*y*weight;
    }
  else // _pass>0
    {
    double xx = x - _meanX;
    double yy = y - _meanY;
    double r  = sqrt(xx*xx+yy*yy);
    _xSum  +=  xx*weight;
    _ySum  +=  yy*weight;
    _x2Sum +=  xx*xx*weight;
    _y2Sum +=  yy*yy*weight;
    _xySum +=  xx*yy*weight;
    if (_doN)
      {
      for (int k=0; k<_nOrders; ++k)
        {
        int iOrder = k+1;

        double phi  = atan2(yy,xx);
        double exponent = (iOrder==1) ? 3.0 : iOrder; // use r^3 weighting for Ecc1/Psi1
        double rr = power(r,exponent);
        double n  = double(iOrder);
        _cosPhiN[k] += rr*cos(n*phi);
        _sinPhiN[k] += rr*sin(n*phi);
        _rN[k]      += rr;
        }
      }
    }
  }

  void GlauberEventMoments::initialize(int nOrders)
  {
  _nOrders = nOrders;
  resetPass();
  reset();
  }

  void GlauberEventMoments::reset()
  {
  const double zero = 0.0;
  const double undefined = -1.0E100;
  _sum     = zero;
  _xSum    = zero;
  _ySum    = zero;
  _x2Sum   = zero;
  _y2Sum   = zero;
  _xySum   = zero;
  if (_pass==0)
    {
    _meanX   = undefined;
    _meanY   = undefined;
    _meanX2  = undefined;
    _meanY2  = undefined;
    _meanXY  = undefined;
    _varX    = undefined;
    _varY    = undefined;
    _varXY   = undefined;
    _rmsX    = undefined;
    _rmsY    = undefined;
    _eccX    = undefined;
    _eccY    = undefined;
    _ecc     = undefined;
    _psi     = undefined;
    }
  else
    {
    if (_doN)
      {
      _cosPhiN.assign(_nOrders,zero);
      _sinPhiN.assign(_nOrders,0.0);
      _rN.assign(_nOrders,zero);
      _psiN.assign(_nOrders,undefined);
      _eccN.assign(_nOrders,undefined);
      }
    }
  }

  void GlauberEventMoments::calculateAverages()
  {
  if (_sum<1) return;
  const double undefined = -1.0E100;

  _meanX  =  _xSum/_sum;
  _meanY  =  _xSum/_sum;
  _meanX2 =  _x2Sum/_sum;
  _meanY2 =  _y2Sum/_sum;
  _meanXY =  _xySum/_sum;
  _varX   =  _meanX2 - _meanX*_meanX;
  _varY   =  _meanX2 - _meanY*_meanY;
  _varXY  =  _meanXY - _meanX*_meanY;
  _rmsX   = _varX>=0 ? sqrt(_varX) : -1.0E100;
  _rmsY   = _varY>=0 ? sqrt(_varY) : -1.0E100;
  double varDiff = _varY - _varX;
  double varSum  = _varX + _varY;
  if (varSum>0)
    {
    _eccX = varDiff/varSum;
    _eccY = 2.0*_varXY/varSum;
    _ecc  = sqrt(_eccX*_eccX + _eccY*_eccY);
    _psi  = atan2(_eccY,_eccX);
    }
  else
    {
    _eccX = undefined;
    _eccY = undefined;
    _ecc  = undefined;
    _psi  = undefined;
    }

  if (_pass>0 && _doN)
    {
    double cp, sp;
    for (int k=0; k<_nOrders; ++k)
      {
      int iOrder = k+1;
      sp = _sinPhiN[k];
      cp = _cosPhiN[k];
      if (_rN[k]>0)
        {
        _psiN[k] = (atan2(sp,cp) + pi())/double(iOrder);
        _eccN[k] = sqrt(sp*sp + cp*cp) / _rN[k];
        _cosPhiN[k] /= _sum;
        _sinPhiN[k] /= _sum;
        _rN[k] /= _sum;
        sp = _sinPhiN[k];
        cp = _cosPhiN[k];
        _psiN[k] = (atan2(sp,cp) + pi())/double(iOrder);
        _eccN[k] = sqrt(sp*sp + cp*cp)/_rN[k];
        }
      else
        {
        _psiN[k] = undefined;
        _eccN[k] = undefined;
        }
      }
    }

  }

  void GlauberEventMoments::clear()
  {
  const double undefined = -1.0E100;
  _pass     = 0;
  _sum      = undefined;
  _xSum     = undefined;
  _ySum     = undefined;
  _x2Sum    = undefined;
  _y2Sum    = undefined;
  _xySum    = undefined;
  _meanX    = undefined;
  _meanY    = undefined;
  _meanX2   = undefined;
  _meanY2   = undefined;
  _meanXY   = undefined;
  _varX     = undefined;
  _varY     = undefined;
  _varXY    = undefined;
  _rmsX     = undefined;
  _rmsY     = undefined;
  _eccX     = undefined;
  _eccY     = undefined;
  _ecc      = undefined;
  _psi      = undefined;
  _cosPhiN.clear();
  _sinPhiN.clear();
  _rN.clear();
  _psiN.clear();
  _eccN.clear();
  }

  void GlauberEventMoments::save()
  {
  // not ready - fix me
  }

  void GlauberEventMoments::print() const
  {
  printCR(2);
  printLine();
  printLine();
  printValue("sum",  _sum);
  printValue("xSum", _xSum);
  printValue("ySum", _ySum);
  printValue("x2Sum",_x2Sum);
  printValue("y2Sum",_y2Sum);
  printValue("xySum",_xySum);
  printValue("meanX",_meanX);
  printValue("meanY",_meanY);
  printValue("meanX2",_meanX2);
  printValue("meanY2",_meanY2);
  printValue("meanXY",_meanXY);
  printValue("varX", _varX);
  printValue("varY", _varY);
  printValue("varXY",_varXY);
  printValue("rmsX", _rmsX);
  printValue("rmsY", _rmsY);
  printValue("eccX", _eccX);
  printValue("eccY", _eccY);
  printValue("ecc", _ecc);
  printValue("psiY", _psi);

  double rho = -1;
  if (_varX>0  &&  _varY>0)
    {
    rho = _varXY/(sqrt(_varX)*sqrt(_varY));
    printValue("rho", sqrt(rho));
    }
  else
    printValue("rho:", "Not defined");
  printLine();

  if (_pass>0 && _doN)
    {
    for (int iOrder=0; iOrder<_nOrders; iOrder++)
      {
      printValue("iOrder",         iOrder);
      printValue("psiN[iOrder]",   _psiN[iOrder]);
      printValue("eccN[iOrder]",   _eccN[iOrder]);
      printValue("cosPhiN[iOrder]",_cosPhiN[iOrder]);
      printValue("sinPhiN[iOrder]",_sinPhiN[iOrder]);
      printValue("rn[iOrder]",     _rN[iOrder]);
      printValue("psiN[iOrder]",   _psiN[iOrder]);
      printValue("eccN[iOrder]",   _eccN[iOrder]);
      }
    }

  }
} // namespace CAP
