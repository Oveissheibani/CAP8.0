#ifndef CAP_DataPoint
#define CAP_DataPoint
#include "Object.hpp"
#include "DataPoints.hpp"

namespace CAP
{
  namespace MATH
  {

  //!
  //!Class encapsulating data point in 2D
  //!x : independent or control variable
  //!y : dependent variable
  //!xError : uncertainty on x
  //!yStatError : statistical (usually) uncertainty on y
  //!ySystError : systematic uncertainty on y
  //!yStatError2 : statistical uncertainty on y (used for asymetric uncertainties)
  //!ySystError2: systematic uncertainty on y (used for asymetric uncertainties)
  //!Errors are set to -1E25 when not in
  //!
  //!
  class DataPoint
  {
  protected:

  char  _bits;
  float _x;
  float _y;
  float _xError;
  float _yStatError;
  float _ySystError;

  public:

  DataPoint();
  DataPoint(float x, float y);
  DataPoint(float x, float y, float xError, float yError);
  DataPoint(float x, float y, float xError, float yStatError, float ySystError);
  DataPoint(const DataPoint & source);
  virtual ~DataPoint() {};
  DataPoint & operator= (const DataPoint & rhs);

  bool  operator== (const DataPoint & rhs) const;
  bool  operator!= (const DataPoint & rhs) const;

  DataPoint & operator+= (const DataPoint & rhs);
  DataPoint & operator-= (const DataPoint & rhs);
  DataPoint & operator*= (const DataPoint & rhs);
  DataPoint & operator/= (const DataPoint & rhs);


  DataPoint & operator+= (double rhs);
  DataPoint & operator-= (double rhs);
  DataPoint & operator*= (double rhs);
  DataPoint & operator/= (double rhs);

  float x() const { return _x; }
  float y() const { return _y; }
  float xError() const { return _xError; }
  float yStatError() const { return _yStatError; }
  float ySystError() const { return _ySystError; }
  float yError();

  void scale(double coefficient);
  void inverse();

  void clear();
  void reset();
  void setPoint(double x, double y);
  void setPoint(double x, double y, double statError);
  void setPoint(double x, double y, double statError, double systError);


  void setAsSumOf(const DataPoint & h1, const DataPoint & h2,
                  double coefficient1, double coefficient2);
  void setAsSumOf(const std::vector<const DataPoint *> & histograms,
                  const std::vector<double> & coefficientw);

  void setAsProductOf(const DataPoint & h1, const DataPoint & h2,
                      double coefficient1, double coefficient2);
  void setAsProductOf(const std::vector<const DataPoint *> & histograms,
                      const std::vector<double> & coefficientw);
  void setAsRatioOf(const DataPoint & h1, const DataPoint & h2,
                    double coefficient1, double coefficient2);

  virtual void print() const;

  friend DataPoint operator+(const DataPoint & left, const DataPoint & right);
  friend DataPoint operator-(const DataPoint & left, const DataPoint & right);
  friend DataPoint operator*(const DataPoint & left, const DataPoint & right);
  friend DataPoint operator/(const DataPoint & left, const DataPoint & right);

  friend DataPoint operator+(const DataPoint & left, double right);
  friend DataPoint operator+(double left, const DataPoint & right);
  friend DataPoint operator-(const DataPoint & left, double right);
  friend DataPoint operator-(double left, const DataPoint & right);
  friend DataPoint operator*(const DataPoint & left, double right);
  friend DataPoint operator*(double left, const DataPoint & right);
  friend DataPoint operator/(const DataPoint & left, double right);
  friend DataPoint operator/(double left, const DataPoint & right);

  friend std::ostream& operator<<(std::ostream& out, const DataPoint & v);

  ClassDef(DataPoint,1)

  };


  DataPoint operator+(const DataPoint & left, const DataPoint & right);
  DataPoint operator-(const DataPoint & left, const DataPoint & right);
  DataPoint operator*(const DataPoint & left, const DataPoint & right);
  DataPoint operator/(const DataPoint & left, const DataPoint & right);

  DataPoint operator+(const DataPoint & left, double right);
  DataPoint operator+(double left, const DataPoint & right);
  DataPoint operator-(const DataPoint & left, double right);
  DataPoint operator-(double left, const DataPoint & right);
  DataPoint operator*(const DataPoint & left, double right);
  DataPoint operator*(double left, const DataPoint & right);
  DataPoint operator/(const DataPoint & left, double right);
  DataPoint operator/(double left, const DataPoint & right);

  DataPoint operator+(const DataPoint & right);
  DataPoint operator-(const DataPoint & right);

  std::ostream& operator<<(std::ostream& out, const DataPoint & v);

  } // Math
} // namespace CAP

#endif
