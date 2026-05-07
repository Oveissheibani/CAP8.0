#include "ScalarField.hpp"
#include "MathExceptions.hpp"

ClassImp(CAP::MATH::ScalarField);

namespace CAP
{
  namespace MATH
  {

  double ScalarField::_xIn;
  double ScalarField::_xI0;
  double ScalarField::_xIp;
  double ScalarField::_x0;
  double ScalarField::_xD;

  ScalarField::ScalarField()
  :
  Object("Null","Null","ScalarField"),
  _type(0),
  _xNPts(0), _yNPts(0), _zNPts(0),
  _constValue(0.0),
  _xMin(0.0), _xMax(1.0), _xWidth(1.0),_xRange(1.0),
  _yMin(0.0), _yMax(1.0), _yWidth(1.0),_yRange(1.0),
  _zMin(0.0), _zMax(1.0), _zWidth(1.0),_zRange(1.0),
  _field(nullptr)
  {
  // no _field array initialization -- this _field is constant _type == 0, value == 0
  }

  ScalarField::ScalarField(const String & aName, double _constValue)
  :
  Object(aName,aName,"ScalarField"),
  _type(0),
  _xNPts(0), _yNPts(0), _zNPts(0),
  _constValue(_constValue),
  _xMin(0.0), _xMax(1.0), _xWidth(1.0),_xRange(1.0),
  _yMin(0.0), _yMax(1.0), _yWidth(1.0),_yRange(1.0),
  _zMin(0.0), _zMax(1.0), _zWidth(1.0),_zRange(1.0),
  _field(nullptr)
  {
  // no _field array initialization -- this _field is constant _type == 0, value == _constValue
  }

  ScalarField::ScalarField(const String & aName,
                           double x_min, double x_max, unsigned int x_npts,
                           double y_min, double y_max, unsigned int y_npts,
                           double z_min, double z_max, unsigned int z_npts,
                           double initValue)
  :
  Object(aName,aName,"ScalarField"),
  _type(1),
  _xNPts(x_npts), _yNPts(y_npts), _zNPts(z_npts),
  _constValue(initValue),
  _xMin(x_min), _xMax(x_max), _xWidth(0.0), _xRange(x_max-x_min),
  _yMin(y_min), _yMax(y_max), _yWidth(0.0), _yRange(y_max-y_min),
  _zMin(z_min), _zMax(z_max), _zWidth(0.0), _zRange(z_max-z_min),
  _field(nullptr)
  {
  if (_xMax<=_xMin) throw InvalidBoundsException(_xMin,_xMax,"_xMax<=_xMin",__FUNCTION__);
  if (_yMax<=_yMin) throw InvalidBoundsException(_yMin,_yMax,"_yMax<=_yMin",__FUNCTION__);
  if (_zMax<=_zMin) throw InvalidBoundsException(_zMin,_zMax,"_zMax<=_zMin",__FUNCTION__);
  if (_xNPts < 1) _xNPts = 1;
  if (_yNPts < 1) _yNPts = 1;
  if (_zNPts < 1) _zNPts = 1;
  _xWidth = (double(_xNPts) - 1.0) / _xRange;
  _yWidth = (double(_yNPts) - 1.0) / _yRange;
  _zWidth = (double(_zNPts) - 1.0) / _zRange;
  initialize(_xNPts, _yNPts, _yNPts, initValue);
  }

  ScalarField::ScalarField(const ScalarField& source)
  :
  Object(source),
  _type(source._type),
  _xNPts(source._xNPts), _yNPts(source._yNPts), _zNPts(source._zNPts),
  _constValue(source._constValue),
  _xMin(source._xMin), _xMax(source._xMax), _xWidth(source._xWidth), _xRange(source._xRange),
  _yMin(source._yMin), _yMax(source._yMax), _yWidth(source._yWidth), _yRange(source._yRange),
  _zMin(source._zMin), _zMax(source._zMax), _zWidth(source._zWidth), _zRange(source._zRange),
  _field(nullptr)
  {
  if (_type>0)
    {
    copyField(source._field,_xNPts, _yNPts, _yNPts);
    }
  }

  ScalarField::~ScalarField()
  {
  clear();
  }

  ScalarField ScalarField::operator=(const ScalarField & rhs)
  {
  if (this!=&rhs)
    {
    Object::operator=(rhs);
    _constValue = rhs._constValue;
    _xNPts  = rhs._xNPts;
    _yNPts  = rhs._yNPts;
    _zNPts  = rhs._zNPts;
    _xMin   = rhs._xMin;
    _xMax   = rhs._xMax;
    _xWidth = rhs._xWidth;
    _xRange = rhs._xRange;
    _yMin   = rhs._yMin;
    _yMax   = rhs._yMax;
    _yWidth = rhs._yWidth;
    _yRange = rhs._yRange;
    _zMin   = rhs._zMin;
    _zMax   = rhs._zMax;
    _zWidth = rhs._zWidth;
    _zRange = rhs._zRange;
    copyField(rhs._field,_xNPts,_yNPts,_zNPts);
    }
  return *this;
  }

  double& ScalarField::operator()(unsigned int iX, unsigned int iY, unsigned int iZ)
  {
  if (_type==0)
    return _constValue;
  else if (iX>=_xNPts || iY>=_yNPts || iZ>=_zNPts)
    throw MathException("Invalid input",__FUNCTION__);
  else
    return _field[iX][iY][iZ];
  }

  void ScalarField::initialize(unsigned int nX, unsigned int nY, unsigned int nZ, double initialValue)
  {
  if (_field) clear();
  _type = 3;
  _constValue = initialValue;
  if (nX<1 || nY<1 || nZ<1) throw MathException("nX<1 || nY<1 || nZ<1",__FUNCTION__);
  _field = new double** [nX];
  for (unsigned int i=0; i<nX; i++)
    {
    _field[i] = new double* [nY];
    for (unsigned int j=0; j<nY; j++)
      {
      _field[i][j] = new double [nZ];
      for (unsigned int k=0; k<nZ; k++)
        _field[i][j][k] = initialValue;
      }
    }
  }

  void ScalarField::reset()
  {
  if (_type==0)
    _constValue = 0.0;
  else
    {
    for (unsigned int i=0; i<_xNPts; i++)
      {
      for (unsigned int j=0; j<_yNPts; j++)
        {
        for (unsigned int k=0; k<_zNPts; k++)
          _field[i][j][k] = 0.0;
        }
      }
    }
  }

  void ScalarField::clear()
  {
  if (_field)
    {
    for (unsigned int i=0; i<_xNPts; i++)
      {
      for (unsigned int j=0; j<_yNPts; j++)
        delete[] _field[i][j];
      delete[] _field[i];
      }
    delete[] _field;
    }
  }


  void ScalarField::setValue(double value)
  {
  clear();
  _type = 0;
  _constValue = value;
  }

  void ScalarField::setValue(const ScalarField & source)
  {
  clear();
  _type = source._type;
  _constValue = source._constValue;
  _xMin = source._xMin; _xMax = source._xMax; _xNPts = source._xNPts; _xWidth = source._xWidth; _xRange = source._xRange;
  _yMin = source._yMin; _yMax = source._yMax; _yNPts = source._yNPts; _yWidth = source._yWidth; _yRange = source._yRange;
  _zMin = source._zMin; _zMax = source._zMax; _zNPts = source._zNPts; _zWidth = source._zWidth; _zRange = source._zRange;
  initialize(_xNPts,_yNPts,_zNPts);
  }

  void ScalarField::setValue(const String & aName,
                             double xMin, double xMax, unsigned int xNpts,
                             double yMin, double yMax, unsigned int yNpts,
                             double zMin, double zMax, unsigned int zNpts,
                             double initValue)
  {
  if (xMax<=xMin) throw InvalidBoundsException(xMin,xMax,"Invalid input: xMin>=xMax",__FUNCTION__);
  if (yMax<=yMin) throw InvalidBoundsException(yMin,yMax,"Invalid input: yMin>=yMax",__FUNCTION__);
  if (zMax<=zMin) throw InvalidBoundsException(zMin,zMax,"Invalid input: zMin>=zMax",__FUNCTION__);
  clear();
  _type = 1;
  setName(aName);
  setTitle(aName);
  _constValue = 0.0;
  _xMin = xMin; _xMax = xMax; _xNPts = xNpts; _xWidth = 0.0;
  _yMin = yMin; _yMax = yMax; _yNPts = yNpts; _yWidth = 0.0;
  _zMin = zMin; _zMax = zMax; _zNPts = zNpts; _zWidth = 0.0;
  _xRange = _xMax - _xMin;
  _yRange = _yMax - _yMin;
  _zRange = _zMax - _zMin;
  if(_xNPts < 1) _xNPts = 1;
  if(_yNPts < 1) _yNPts = 1;
  if(_zNPts < 1) _zNPts = 1;
  _xWidth = (_xNPts - 1) / _xRange;
  _yWidth = (_yNPts - 1) / _yRange;
  _zWidth = (_zNPts - 1) / _zRange;
  initialize(_xNPts, _yNPts, _yNPts,initValue);
  }

  double ScalarField::value() const
  {
  if (_type==0)
    return _constValue;
  else
    throw MathException("Invalid input: Must specify iX, iY, iZ",__FUNCTION__);
  }

  double ScalarField::valueAt(unsigned int iX, unsigned int iY, unsigned int iZ) const
  {
  if (_type==0)
    return _constValue;
  if (iX>=_xNPts) throw IndexOutOfBoundException(iX,_xNPts,"iX>=xNPts",__FUNCTION__);
  if (iY>=_yNPts) throw IndexOutOfBoundException(iX,_yNPts,"iY>=yNPts",__FUNCTION__);
  if (iZ>=_zNPts) throw IndexOutOfBoundException(iX,_zNPts,"iZ>=zNPts",__FUNCTION__);
  return _field[iX][iY][iZ];
  }

  double ScalarField::valueAt(double aX, double aY, double aZ) const
  {
  if (_type==0)
    return _constValue;
  else
    return interpolate(aX,aY,aZ);
  }


  double ScalarField::interpolate(double aX, double aY, double aZ) const
  {
  if(_zNPts>1)
    return interpolate3D(aX, aY, aZ);
  else if (_yNPts>1)
    return interpolate2D(aX, aY);
  else if (_xNPts>1)
    return interpolate1D(aX);
  else
    return _field[0][0][0];
  }

  ScalarField ScalarField::derivativeX(const String & aName)
  {
  unsigned int ti;
  if (_xNPts<=1) throw DivByZeroException("_xNPts<=1","Too few bins in X",__FUNCTION__);
  ScalarField result(*this);
  _xD = (_xMax - _xMin) / (_xNPts - 1);
  result.setName(aName);
  for (unsigned int i=0; i<_xNPts; i++)
    {
    ti = initDerivative(i, _xMin, _xMax, _xNPts);
    for (unsigned int j=0; j<_yNPts; j++)
      for (unsigned int k=0; k<_zNPts; k++)
        result(i,j,k) = derivative(_field[ti-1][j][k], _field[ti][j][k], _field[ti+1][j][k]);
    }
  return result;
  }

  ScalarField ScalarField::derivativeY(const String & aName)
  {
  unsigned int tj;
  if (_yNPts<=1) throw DivByZeroException("_yNPts<=1","Too few bins in Y",__FUNCTION__);
  ScalarField result(*this);
  _xD = (_yMax - _yMin) / (_yNPts - 1);
  result.setName(aName);
  for (unsigned int i=0; i<_xNPts; i++)
    for (unsigned int j=0; j<_yNPts; j++) {
      tj = initDerivative(j, _yMin, _yMax, _yNPts);
      for (unsigned int k=0; k<_zNPts; k++)
        result(i,j,k) = derivative(_field[i][tj-1][k], _field[i][tj][k], _field[i][tj+1][k]);
    }
  return result;
  }

  ScalarField ScalarField::derivativeZ(const String & aName)
  {
  unsigned int tk;
  if (_zNPts<=1) throw DivByZeroException("_zNPts<=1","Too few bins in Z",__FUNCTION__);
  ScalarField result(*this);
  _xD = (_zMax - _zMin) / (_zNPts - 1);
  result.setName(aName);
  for (unsigned int i=0; i<_xNPts; i++)
    for (unsigned int j=0; j<_yNPts; j++)
      for (unsigned int k=0; k<_zNPts; k++) {
        tk = initDerivative(k, _zMin, _zMax, _zNPts);
        result(i,j,k) = derivative(_field[i][j][tk-1], _field[i][j][tk], _field[i][j][tk+1]);
      }
  return result;
  }

  double ScalarField::interpolate1D(double aX)  const
  {
  unsigned int i, j, k;
  double ti;

  ti = (aX - _xMin) * _xWidth;	i = (unsigned int) ti;	if(i+1 > _xNPts-1) i--;	ti -= i;
  j  = 0;
  k  = 0;
  return
  _field[i  ][j  ][k  ] * (1-ti) + _field[i+1][j  ][k  ] * ti;
  }

  double ScalarField::interpolate2D(double aX, double aY) const
  {
  unsigned int    i, j, k;
  double ti,tj;

  ti = (aX - _xMin) * _xWidth;	i = (unsigned int) ti;	if(i+1 > _xNPts-1) i--;	ti -= i;
  tj = (aY - _yMin) * _yWidth;	j = (unsigned int) tj;	if(j+1 > _yNPts-1) j--;	tj -= j;
  k  = 0;
  return
  (_field[i  ][j  ][k  ] * (1-ti) + _field[i+1][j  ][k  ] * ti) * (1-tj) +
  (_field[i  ][j+1][k  ] * (1-ti) + _field[i+1][j+1][k  ] * ti) *    tj;
  }

  double ScalarField::interpolate3D(double aX, double aY, double aZ)  const
  {
  unsigned int    i, j, k;
  double ti,tj,tk;

  ti = (aX - _xMin) * _xWidth;	i = (unsigned int) ti;	if(i+1 > _xNPts-1) i--;	ti -= i;
  tj = (aY - _yMin) * _yWidth;	j = (unsigned int) tj;	if(j+1 > _yNPts-1) j--;	tj -= j;
  tk = (aZ - _zMin) * _zWidth;	k = (unsigned int) tk;	if(k+1 > _zNPts-1) k--;	tk -= k;
  return
  (
   (_field[i  ][j  ][k  ] * (1-ti) + _field[i+1][j  ][k  ] * ti) * (1-tj) +
   (_field[i  ][j+1][k  ] * (1-ti) + _field[i+1][j+1][k  ] * ti) *    tj
   ) * (1-tk) + (
                 (_field[i  ][j  ][k+1] * (1-ti) + _field[i+1][j  ][k+1] * ti) * (1-tj) +
                 (_field[i  ][j+1][k+1] * (1-ti) + _field[i+1][j+1][k+1] * ti) *    tj
                 ) *    tk;
  }

  inline int ScalarField::initDerivative(int aIdx, double aAMin, double aAampx, int aAPts)
  {
  if(aIdx == 0) {
    _xIn = aAMin;
    _xI0 = aAMin + _xD;
    _xIp = aAMin + _xD * 2.0;
    _x0  = aAMin;
    return aIdx + 1;
  } else if (aIdx == aAPts - 1) {
    _xIn = aAampx - _xD * 2.0;
    _xI0 = aAampx - _xD;
    _xIp = aAampx;
    _x0  = aAampx;
    return aIdx - 1;
  } else {
    _xIn = aAMin + _xD * (aIdx - 1);
    _xI0 = aAMin + _xD *  aIdx;
    _xIp = aAMin + _xD * (aIdx + 1);
    _x0  = _xI0;
    return aIdx;
  }
  }

  inline double ScalarField::derivative(double aFin, double aFi0, double aFip)
  {
  return (
          _xIn * (_xIn - 2.0 * _x0) * (aFip - aFi0) +
          _xI0 * (_xI0 - 2.0 * _x0) * (aFin - aFip) +
          _xIp * (_xIp - 2.0 * _x0) * (aFi0 - aFin)
          ) / (2.0 * _xD * _xD * _xD);
  }


  void ScalarField::copyField(double*** source,unsigned int nX, unsigned int nY, unsigned int nZ)
  {
  initialize(nX,nY,nZ,0.0);
  for (unsigned int iX=0; iX<nX; iX++)
    {
    for (unsigned int iY=0; iY<nY; iY++)
      {
      for (unsigned int iZ=0; iZ<nZ; iZ++)
        _field[iX][iY][iZ] = source[iX][iY][iZ];
      }
    }
  }
  } // namespace MATH
} // namespace CAP

