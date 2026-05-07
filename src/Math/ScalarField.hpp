#ifndef CAP__ScalarField
#define CAP__ScalarField
#include "Aliases.hpp"
#include "Object.hpp"

namespace CAP
{
  namespace MATH
  {

  //!
  //!Class encapsulate the notion of field in 2 or 3D
  //!Type = 0  : constant field
  //!Type = 0  : constant field
  //!Type = 3  : 3D field
  class ScalarField : public Object
  {
  public:
  // OPERATORS
  double& operator()(unsigned int i, unsigned int j, unsigned int k);

  // CLASS FUNCTIONS
  ScalarField();
  ScalarField(const String & aName, double _constValue);
  ScalarField(const String & aName,
              double x_min, double x_max, unsigned int x_npts,
              double y_min, double y_max, unsigned int y_npts,
              double z_min, double z_max, unsigned int z_npts,
              double initValue=0);
  ScalarField(const ScalarField & _field);
  virtual ~ScalarField();
  ScalarField operator=(const ScalarField & rhs);

  void setValue(double value);
  void setValue(const ScalarField & _field);
  void setValue(const String & aName,
                double x_min, double x_max, unsigned int x_npts,
                double y_min, double y_max, unsigned int y_npts,
                double z_min, double z_max, unsigned int z_npts,
                double initValue=0);

  unsigned int type() const { return _type;}
  double  value() const;
  double  valueAt(unsigned int iX, unsigned int iY, unsigned int iZ) const;
  double  valueAt(double aX, double aY, double aZ) const;
  double  xMin() const    { return _xMin; }
  double  xMax() const    { return _xMax; }
  int     xNPts() const   { return _xNPts; }
  double  yMin() const    { return _yMin; }
  double  yMax() const    { return _yMax; }
  int     yNPts() const   { return _yNPts; }
  double  zMin() const    { return _zMin; }
  double  zMax() const    { return _zMax; }
  int     zNPts() const   { return _zNPts; }
  double  xRange() const  { return _xMax - _xMin; }
  double  yRange() const  { return _yMax - _yMin; }
  double  zRange() const  { return _zMax - _zMin; }
  double  volume() const  { return (_xMax - _xMin)*(_yMax - _yMin)*(_zMax - _zMin); }
  double  randomX(double r) { return _xMin + (_xMax - _xMin)*r; }
  double  randomY(double r) { return _xMin + (_yMax - _yMin)*r; }
  double  randomZ(double r) { return _xMin + (_zMax - _zMin)*r; }

  double	interpolate(double aX, double aY, double aZ) const;
  ScalarField derivativeX(const String & aName);
  ScalarField derivativeY(const String & aName);
  ScalarField derivativeZ(const String & aName);

  protected:

  virtual void initialize(unsigned int nX, unsigned int nY, unsigned int nZ, double initialValue=0);
  virtual void reset();
  virtual void clear();
  virtual void copyField(double*** source,
                         unsigned int nX,
                         unsigned int nY,
                         unsigned int nZ);

  double	interpolate1D(double aX)  const;
  double	interpolate2D(double aX, double aY)  const;
  double	interpolate3D(double aX, double aY, double aZ) const;

  inline int	  initDerivative(int aIdx, double aAMin, double aAampx, int aAPts);
  inline double derivative(double aFin, double aFi, double aFip);

  unsigned int _type;
  unsigned int _xNPts;
  unsigned int _yNPts;
  unsigned int _zNPts;
  double _constValue;

  double _xMin;
  double _xMax;
  double _xWidth;
  double _xRange;

  double _yMin;
  double _yMax;
  double _yWidth;
  double _yRange;

  double _zMin;
  double _zMax;
  double _zWidth;
  double _zRange;

  double***  _field;

  // used by initDerivative() and derivative()
  static double _xIn, _xI0, _xIp, _x0, _xD;

  ClassDef(ScalarField,1)


  };

  } //namespace MATH
} //  namespace CAP

#endif

/*! @file ScalarField.h
 * @brief Definition of ScalarField class.
 */
/*! @class ScalarField
 * @brief Class representing a three dimensional vector _field.
 *
 * The table of numbers can be interpolated with a cubic method. Class also provides a simple calculation of the
 * derivative along all three axes.
 *
 * @fn ScalarField::ScalarField()
 * @brief Default constructor.
 *
 * @fn ScalarField::ScalarField(const char* aName, double _x_min, double _x_max, int _x_npts, double _y_min, double _y_max, int _y_npts, double _z_min, double _z_max, int _z_npts)
 * @brief Initiates a three dimensional vector _field of a given name and limiting range and number of points for each axis.
 * @param [in] aName name of the vector _field
 * @param [in] _x_min lower limit of X-axis
 * @param [in] _x_max higher limit of X-axis
 * @param [in] _x_npts number of points in X direction
 * @param [in] _y_min lower limit of Y-axis
 * @param [in] _y_max higher limit of Y-axis
 * @param [in] _y_npts number of points in Y direction
 * @param [in] _z_min lower limit of Z-axis
 * @param [in] _z_max higher limit of Z-axis
 * @param [in] _z_npts number of points in Z direction
 *
 * @fn ScalarField::ScalarField(const ScalarField& aVector)
 * @brief Copying constructor.
 * @param [in] aVector referance to ScalarField
 *
 * @fn ScalarField::~ScalarField()
 * @brief Destructor.
 *
 * @fn double& ScalarField::operator()(unsigned int i, unsigned int j, unsigned int k)
 * @brief Operator (i,j,k) providing access to ScalarField array element
 * @param [in] i array index for the x-coordinate
 * @param [in] j array index for the y-coordinate
 * @param [in] k array index for the z-coordinate
 *
 * @fn const char* ScalarField::name() const
 * @brief Returns the vector _field name
 *
 * @fn double ScalarField::xMin() const
 * @brief Returns the X-axis lower limit
 *
 * @fn double ScalarField::xMax() const
 * @brief Returns the X-axis higher limit
 *
 * @fn double ScalarField::xNPts() const
 * @brief Returns the number of X-axis points
 *
 * @fn double ScalarField::yMin() const
 * @brief Returns the Y-axis lower limit
 *
 * @fn double ScalarField::yMax() const
 * @brief Returns the Y-axis higher limit
 *
 * @fn double ScalarField::yNPts() const
 * @brief Returns the number of Y-axis points
 *
 * @fn double ScalarField::zMin() const
 * @brief Returns the Z-axis lower limit
 *
 * @fn double ScalarField::zMax() const
 * @brief Returns the Z-axis higher limit
 *
 * @fn double ScalarField::zNPts() const
 * @brief Returns the number of Z-axis points
 *
 * @fn double ScalarField::interpolate(double aX, double aY, double aZ)
 * @brief Returns an interpolated value of the vector _field.
 * @param [in] aX x-coordinate
 * @param [in] aY y-coordinate
 * @param [in] aZ z-coordinate
 *
 * @fn ScalarField* ScalarField::derivativeX(const char* aName)
 * @brief Calculates the derivative of the vector _field along the X-axis
 * @param [in] aName name of the derived vector _field
 * @retval ScalarField pointer to a created vector _field
 *
 * @fn ScalarField* ScalarField::derivativeY(const char* aName)
 * @brief Calculates the derivative of the vector _field along the Y-axis
 * @param [in] aName name of the derived vector _field
 * @retval ScalarField pointer to a created vector _field
 *
 * @fn ScalarField* ScalarField::derivativeZ(const char* aName)
 * @brief Calculates the derivative of the vector _field along the Z-axis
 * @param [in] aName name of the derived vector _field
 * @retval ScalarField pointer to a created vector _field
 */
