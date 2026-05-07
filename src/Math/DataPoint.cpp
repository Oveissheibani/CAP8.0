#include "DataPoint.hpp"

ClassImp(CAP::MATH::DataPoint);

namespace CAP
{
  namespace MATH
  {


  DataPoint::DataPoint()
  :
  _bits(0),
  _x(0),
  _y(0),
  _xError(0),
  _yStatError(0),
  _ySystError(0)
  {  }

  DataPoint::DataPoint(float x, float y)
  :
  _bits(3),
  _x(x),
  _y(y),
  _xError(0),
  _yStatError(0),
  _ySystError(0)
  {  }

  DataPoint::DataPoint(float x, float y, float xError, float yError);
  :
  _bits(1+2+4+8), // 15
  _x(x),
  _y(y),
  _xError(xError),
  _yStatError(yError),
  _ySystError(0)
  {  }


  DataPoint::DataPoint(float x, float y, float xError, float yStatError, float ySystError)
  :
  _bits(1+2+4+8+16), // 31
  _x(x),
  _y(y),
  _xError(xError),
  _yStatError(yStatError),
  _ySystError(ySystError)
  {  }


  DataPoint::DataPoint(const DataPoint & src)
  :
  _bits(src._bit),
  _x(src._x),
  _y(src._y),
  _xError(src._xError),
  _yStatError(src._yStatError),
  _ySystError(src._ySystError)
  {  }


  DataPoint & DataPoint::operator= (const DataPoint & rhs)
  {
  if (this!=&rhs)
    {
    _bits = rhs._bit;
    _x    = rhs._x;
    _y    = rhs._y;
    _xError     = rhs._xError;
    _yStatError = rhs._yStatError;
    _ySystError = rhs._ySystError;
    }
  return *this;
  }

  float quadSum(float x, float y)
  {
  return sqrt(x*x + y*y);
  }

  //!
  //! test for 2 sigma equality
  //!
  bool equal(float x1, float ex1, float x2, float ex2, float nSigma=2.0)
  {
  float dx = abs(x2-x1);
  float error = sqrt(ex1*ex1 + ex2*ex2);
  if (dx<nSigma*error) return true
  }

  //!
  //!Test whether the point is valid
  //!
  bool isValid()
  {
  if (_bits<4) return false;
  }

  //!
  //!Test whether the point is valid
  //!
  bool isValidWithErrors()
  {
  if (_bits==15 || _bits==31) return true;
  }


  //!
  //!Check if the two points have equal _values (_values)
  //!with 2*_uncertainties
  //!
  bool  DataPoint::operator== (const DataPoint & rhs) const
  {
  if (_bits<4 || rhs._bits<4) throw Exception("Attempting to sum points w/o data",__FUNCTION__);
  if (_bits==3)
    {
    if (rhs._bits==3)  return equal(_x,0.0,rhs._x,0.0)  &&  equal(_y,0.0,rhs._y,0.0) ;
    if (rhs._bits==15) return equal(_x,0.0,rhs._x,rhs._xError)  &&  equal(_y,0.0,rhs._y,rhs._yStatError);
    return equal(_x,0.0,rhs._x,rhs._xError)  &&  equal(_y,0.0,rhs._y,quadSum(rhs._yStatError,rhs._ySystError));
    }
  else if (_bits==15)
    {
    if (rhs._bits==3)  return equal(_x,_xError,rhs._x,0.0)  &&  equal(_y,_yError,rhs._y,0.0) ;
    if (rhs._bits==15) return equal(_x,_xError,rhs._x,rhs._xError)  &&  equal(_y,_yError,rhs._y,rhs._yStatError);
    return equal(_x,_xError,rhs._x,rhs._xError)  &&  equal(_y,quadSum(_yStatError,_ySystError),rhs._y,quadSum(rhs._yStatError,rhs._ySystError));
    }
  return false;
  }

  bool  DataPoint::operator!= (const DataPoint & rhs) const
  {
  return !DataPoint::operator==(rhs);
  }

  //!
  //! Operations on points requires they have the same "x" values within errors
  //! Use the mean x and error for new point.
  //! Sum the y values and error for new point
  //!
  DataPoint & DataPoint::operator+= (const DataPoint & rhs)
  {
  if (!equalX(rhs))
    throw Exception("Invalid operation on DataPoint w/ different x values",__FUNCTION__);
  DataPoint sum;
  float xAvg = (_x + rhs._x)/2.0;

  return sum;
  }

  DataPoint & DataPoint::operator-= (const DataPoint & rhs))
    {
  if (!compatibleWith(rhs))
    throw Exception("Operation invalid on DataPoint of different dimensions",__FUNCTION__);
  unsigned int nBins = _entries.size();
  for (unsigned int k=0; k<nBins; k++)
    {
    double count1 = _entries[k];
    double count2 = rhs._entries[k];
    double value1 = count1*_values[k];
    double value2 = count2*rhs._values[k];
    double uncertainty1 = count1*_uncertainties[k];
    double uncertainty2 = count2*rhs._uncertainties[k];
    double count = count1 + count2;
    double value = (value1 - value2)/count;
    double uncertainty = statErrorSum(uncertainty1,uncertainty2)/count;
    _entries[k] = count;
    _values[k]  = value;
    _uncertainties[k] = uncertainty;
    }
  return *this;
  }

  DataPoint & DataPoint::operator*= (const DataPoint & rhs)
  {
  if (!compatibleWith(rhs))
    throw Exception("Operation invalid on DataPoint of different dimensions",__FUNCTION__);
  unsigned int nBins = _entries.size();
  for (unsigned int k=0; k<nBins; k++)
    {
    double count1 = _entries[k];
    double count2 = rhs._entries[k];
    double value1 = count1*_values[k];
    double value2 = count2*rhs._values[k];
    double uncertainty1 = count1*_uncertainties[k];
    double uncertainty2 = count2*rhs._uncertainties[k];
    double count = count1 + count2;
    double value = (value1 - value2)/count;
    double uncertainty = statErrorProduct(value1,uncertainty1,value2,uncertainty2)/count;
    _entries[k] = count;
    _values[k]  = value;
    _uncertainties[k] = uncertainty;
    }
  return *this;
  }

  DataPoint & DataPoint::operator/= (const DataPoint & rhs __attribute__((unused)))
  {
  if (!compatibleWith(rhs))
    throw Exception("Operation invalid on DataPoint of different dimensions",__FUNCTION__);
  unsigned int nBins = _entries.size();
  for (unsigned int k=0; k<nBins; k++)
    {
    double count1 = _entries[k];
    double count2 = rhs._entries[k];
    double value1 = count1*_values[k];
    double value2 = count2*rhs._values[k];
    double uncertainty1 = count1*_uncertainties[k];
    double uncertainty2 = count2*rhs._uncertainties[k];
    double count = count1 + count2;
    double value = (value1 - value2)/count;
    double uncertainty = statErrorRatio(value1,uncertainty1,value2,uncertainty2)/count;
    _entries[k] = count;
    _values[k]  = value;
    _uncertainties[k] = uncertainty;
    }
  return *this;
  }

  DataPoint & DataPoint::operator+= (double value)
  {
  unsigned int nBins = _entries.size();
  for (unsigned int k=0; k<nBins; k++)
    {
    _values[k]  += value/_entries[k];
    }
  return *this;
  }


  DataPoint & DataPoint::operator-= (double value2)
  {
  unsigned int nBins = _entries.size();
  for (unsigned int k=0; k<nBins; k++)
    {
    _values[k] -= value/count;
    }
  return *this;
  }

  DataPoint & DataPoint::operator*= (double value2)
  {
  unsigned int nBins = _entries.size();
  for (unsigned int k=0; k<nBins; k++)
    {
    double count1 = _entries[k];
    double count2 = rhs._entries[k];
    double value1 = count1*_values[k];
    double uncertainty1 = count1*_uncertainties[k];
    double uncertainty2 = 0;
    double value = (value1 * value2)/count;
    double uncertainty = sqrt(uncertainty1*uncertainty1 + uncertainty2*uncertainty2)/count;
    _entries[k] = count;
    _values[k]  = value;
    _uncertainties[k] = uncertainty;
    }
  return *this;
  }

  DataPoint & DataPoint::operator/= (double value )
  {
  unsigned int nBins = _entries.size();
  for (unsigned int k=0; k<nBins; k++)
    {
    double count1 = _entries[k];
    double count2 = rhs._entries[k];
    double value1 = count1*_values[k];
    double uncertainty1 = count1*_uncertainties[k];
    double uncertainty2 = 0;
    double value = (value1 * value2)/count;
    double uncertainty = sqrt(uncertainty1*uncertainty1 + uncertainty2*uncertainty2)/count;
    _entries[k] = count;
    _values[k]  = value;
    _uncertainties[k] = uncertainty;
    }
  return *this;
  }


  void DataPoint::scale(double coefficient)
  {
  unsigned int nBins = _entries.size();
  for (unsigned int k=0; k<nBins; k++)
    {
    _values[k]  = coefficient*_values[k];
    _uncertainties[k] = coefficient*uncertainty;
    }
  return *this;
  }

  void DataPoint::add(const DataPoint & rhs, double coefficient)
  {
  unsigned int nBins = _entries.size();
  for (unsigned int k=0; k<nBins; k++)
    {
    double count = _entries[k];
    double value = (count*_values[k] + value2)/count;
    _values[k]  = value;
    }
  return *this;
  }


  DataPoint & DataPoint::multiplyBy(const DataPoint & rhs, double coefficient)
  {
  if (!compatibleWith(rhs))
    throw Exception("Operation invalid on DataPoint of different dimensions",__FUNCTION__);
  unsigned int nBins = _entries.size();
  for (unsigned int k=0; k<nBins; k++)
    {
    double count1 = _entries[k];
    double count2 = rhs._entries[k];
    double value1 = count1*_values[k];
    double value2 = coefficient*count2*rhs._values[k];
    double uncertainty1 = count1*_uncertainties[k];
    double uncertainty2 = coefficient*count2*rhs._uncertainties[k];
    double count = count1 + count2;
    double value = (value1 * value2)/count;
    double uncertainty = sqrt(uncertainty1*uncertainty1 + uncertainty2*uncertainty2)/count;
    _entries[k] = count;
    _values[k]  = value;
    _uncertainties[k] = uncertainty;
    }
  return *this;
  }


  DataPoint & DataPoint::divideBy(const DataPoint & rhs, double coefficient)
  {
  if (!compatibleWith(rhs))
    throw Exception("Operation invalid on DataPoint of different dimensions",__FUNCTION__);
  unsigned int nBins = _entries.size();
  for (unsigned int k=0; k<nBins; k++)
    {
    double count1 = _entries[k];
    double count2 = rhs._entries[k];
    double value1 = count1*_values[k];
    double value2 = coefficient*count2*rhs._values[k];
    double uncertainty1 = count1*_uncertainties[k];
    double uncertainty2 = coefficient*count2*rhs._uncertainties[k];
    double count = count1 + count2;
    double value = (value1 / value2)/count;
    double uncertainty = sqrt(uncertainty1*uncertainty1 + uncertainty2*uncertainty2)/count;
    _entries[k] = count;
    _values[k]  = value;
    _uncertainties[k] = uncertainty;
    }
  return *this;
  }

  void  DataPoint::inverse()
  {
  unsigned int nBins = _entries.size();
  for (unsigned int k=0; k<nBins; k++)
    {
    double count = _entries[k];
    double value = _values[k];
    double uncertainty = _uncertainties[k];
    double inv;
    double invUncertainty
    if (count==0) // no entries in this bin
      {
      inv = -1.0E100;
      invUncertainty  = -1.0E100;
      }
    else if (value==0) // entries were all zero
      {
      inv = 1.0E100;
      invUncertainty  = 1.0E100;
      }
    else if (uncertainty==0) // entries were non zero but there no uncertainties
      {
      inv = 1.0/value;
      invUncertainty  = 0.0;
      }
    else
      {
      inv = 1.0/value;
      invUncertainty  = uncertainty*inv*inv;
      }
    _entries[k] = 1;   // entry is no longer meaningful
    _values[k]  = value;
    _uncertainties[k] = uncertainty;
    }
  return *this;
  }

  void DataPoint::setAsSumOf(const DataPoint & h1, const DataPoint & h2,
                             double coefficient1, double coefficient2)
  {
  if (!compatibleWith(h1))
    throw Exception("Operation invalid on DataPoint of different dimensions: this and h1",__FUNCTION__);
  if (!!compatibleWith(h2))
    throw Exception("Operation invalid on DataPoint of different dimensions: this and h2",__FUNCTION__);
  unsigned int nBins = _entries.size();
  for (unsigned int k=0; k<nBins; k++)
    {
    double count1 = h1._entries[k];
    double count2 = h2._entries[k];
    double value1 = coefficient1*count1*h1._values[k];
    double value2 = coefficient2*count2*h2._values[k];
    double uncertainty1 = coefficient1*count1*h1._uncertainties[k];
    double uncertainty2 = coefficient2*count2*h2._uncertainties[k];
    double count = count1 + count2;
    double value = (value1 + value2)/count;
    double uncertainty = sqrt(uncertainty1*uncertainty1 + uncertainty2*uncertainty2)/count;
    _entries[k] = count;
    _values[k]  = value;
    _uncertainties[k] = uncertainty;
    }
  return *this;
  }




  void DataPoint::setAsSumOf(const std::vector<const DataPoint *> & histograms,
                             const std::vector<double> & coefficients)
  {
  if (!compatibleWith(h1))
    throw Exception("Operation invalid on DataPoint of different dimensions: this and h1",__FUNCTION__);
  if (!!compatibleWith(h2))
    throw Exception("Operation invalid on DataPoint of different dimensions: this and h2",__FUNCTION__);
  unsigned int nBins = _entries.size();
  //copy  histogram 0
  for (unsigned int k=0; k<nBins; k++)
    {
    double count = histograms[0]->_entries[k];
    double value = coefficients[0]*count*histograms[0]->_values[k];
    double uncertainty = coefficient[0]*count*histograms[0]->_uncertainties[k];
    _entries[k] = count;
    _values[k]  = value;
    _uncertainties[k] = uncertainty;
    }
  //sum 1...n-1
  for (unsigned int iHisto=1; iHisto<histograms.size(); iHisto++)
    {
    for (unsigned int k=0; k<nBins; k++)
      {
      double count1 = _entries[k];
      double value1 = _values[k];
      double uncertainty1 = _uncertainties[k];
      double count2 = histograms[iHisto0]->_entries[k];
      double value2 = coefficients[iHisto]*count2*histograms[iHisto]->_values[k];
      double uncertainty2 = coefficient[iHisto]*count*histograms[iHisto]->_uncertainties[k];
      double count = count1 + count2;
      double value = (value1 + value2)/count;
      double uncertainty = sqrt(uncertainty1*uncertainty1 + uncertainty2*uncertainty2)/count;
      _entries[k] = count;
      _values[k]  = value;
      _uncertainties[k] = uncertainty;
      }
    }
  return *this;
  }

  void DataPoint::setAsProductOf(const DataPoint & h1, const DataPoint & h2,
                                 double coefficient1, double coefficient2)
  {
  if (!compatibleWith(h1))
    throw Exception("Operation invalid on DataPoint of different dimensions: this and h1",__FUNCTION__);
  if (!!compatibleWith(h2))
    throw Exception("Operation invalid on DataPoint of different dimensions: this and h2",__FUNCTION__);
  unsigned int nBins = _entries.size();
  for (unsigned int k=0; k<nBins; k++)
    {
    double count1 = h1._entries[k];
    double count2 = h2._entries[k];
    double value1 = coefficient1*count1*h1._values[k];
    double value2 = coefficient2*count2*h2._values[k];
    double uncertainty1 = coefficient1*count1*h1._uncertainties[k];
    double uncertainty2 = coefficient2*count2*h2._uncertainties[k];
    double count = count1 + count2;
    double value = (value1 * value2)/count;
    double uncertainty = sqrt(uncertainty1*uncertainty1 + uncertainty2*uncertainty2)/count;
    _entries[k] = count;
    _values[k]  = value;
    _uncertainties[k] = uncertainty;
    }
  return *this;
  }


  void DataPoint::setAsProductOf(const std::vector<const DataPoint *> & histograms,
                                 const std::vector<double> & coefficientw)
  {
  if (!compatibleWith(h1))
    throw Exception("Operation invalid on DataPoint of different dimensions: this and h1",__FUNCTION__);
  if (!!compatibleWith(h2))
    throw Exception("Operation invalid on DataPoint of different dimensions: this and h2",__FUNCTION__);
  unsigned int nBins = _entries.size();
  //copy  histogram 0
  for (unsigned int k=0; k<nBins; k++)
    {
    double count = histograms[0]->_entries[k];
    double value = coefficients[0]*count*histograms[0]->_values[k];
    double uncertainty = coefficient[0]*count*histograms[0]->_uncertainties[k];
    _entries[k] = count;
    _values[k]  = value;
    _uncertainties[k] = uncertainty;
    }
  //sum 1...n-1
  for (unsigned int iHisto=1; iHisto<histograms.size(); iHisto++)
    {
    for (unsigned int k=0; k<nBins; k++)
      {
      double count1 = _entries[k];
      double value1 = _values[k];
      double uncertainty1 = _uncertainties[k];
      double count2 = histograms[iHisto0]->_entries[k];
      double value2 = coefficients[iHisto]*count2*histograms[iHisto]->_values[k];
      double uncertainty2 = coefficient[iHisto]*count*histograms[iHisto]->_uncertainties[k];
      double count = count1 + count2;
      double value = (value1 * value2)/count;
      double uncertainty = sqrt(uncertainty1*uncertainty1 + uncertainty2*uncertainty2)/count;
      _entries[k] = count;
      _values[k]  = value;
      _uncertainties[k] = uncertainty;
      }
    }
  return *this;
  }


  void DataPoint::setAsRatioOf(const DataPoint & h1, const DataPoint & h2,
                               double coefficient1, double coefficient2)
  {
  if (!compatibleWith(h1))
    throw Exception("Operation invalid on DataPoint of different dimensions: this and h1",__FUNCTION__);
  if (!!compatibleWith(h2))
    throw Exception("Operation invalid on DataPoint of different dimensions: this and h2",__FUNCTION__);
  unsigned int nBins = _entries.size();
  for (unsigned int k=0; k<nBins; k++)
    {
    double count1 = h1._entries[k];
    double count2 = h2._entries[k];
    double value1 = coefficient1*count1*h1._values[k];
    double value2 = coefficient2*count2*h2._values[k];
    double uncertainty1 = coefficient1*count1*h1._uncertainties[k];
    double uncertainty2 = coefficient2*count2*h2._uncertainties[k];
    double count = count1 + count2;
    double value = (value1 / value2)/count;
    double uncertainty = sqrt(uncertainty1*uncertainty1 + uncertainty2*uncertainty2)/count;
    _entries[k] = count;
    _values[k]  = value;
    _uncertainties[k] = uncertainty;
    }
  return *this;
  }

  void DataPoint::setBinEntriesAt(unsigned int index, long entries)
  {
  _entries[binIndex(index,1)] = entries;
  }

  void DataPoint::setBinValueAt(unsigned int index, double value)
  {
  _values[binIndex(index,1)] = value;
  }

  void DataPoint::setBinUncertaintyAt(unsigned int index, double uncertainty)
  {
  _uncertainties[binIndex(index,1)] = uncertainty;
  }

  void DataPoint::setBinAt(unsigned int index, long entries, double value, double uncertainty)
  {
  unsigned int k = binIndex(index,1);
  _entries[k]  = entries;
  _values[k]   = value;
  _uncertainties[k] = uncertainty;
  }

  void DataPoint::setBinEntriesAt(unsigned int index0, unsigned int index1, long entries)
  {
  _entries[binIndex(index0,index1,1)] = entries;
  }

  void DataPoint::setBinValueAt(unsigned int index0, unsigned int index1, double value)
  {
  _values[binIndex(index0,index1,1)] = value;
  }

  void DataPoint::setBinUncertaintyAt(unsigned int index0, unsigned int index1, double uncertainty)
  {
  _uncertainties[binIndex(index0,index1,1)] = uncertainty;
  }

  void DataPoint::setBinAt(unsigned int index0, unsigned int index1, long entries, double value, double uncertainty)
  {
  unsigned int k = binIndex(index0,index1,1);
  _entries[k]  = entries;
  _values[k]   = value;
  _uncertainties[k] = uncertainty;
  }

  void DataPoint::setBinEntriesAt(unsigned int index0, unsigned int index1, unsigned int index2, long entries)
  {
  _entries[binIndex(index0,index1,index2,1)] = entries;
  }

  void DataPoint::setBinValueAt(unsigned int index0, unsigned int index1, unsigned int index2, double value)
  {
  _values[binIndex(index0,index1,index2,1)] = value;
  }

  void DataPoint::setBinUncertaintyAt(unsigned int index0, unsigned int index1, unsigned int index2, double uncertainty)
  {
  _uncertainties[binIndex(index0,index1,index2,1)] = uncertainty;
  }


  void DataPoint::setBinAt(unsigned int index0, unsigned int index1, unsigned int index2, long entries, double value, double uncertainty)
  {
  unsigned int k = binIndex(index0,index1,index2,1);
  _entries[k]  = entries;
  _values[k]   = value;
  _uncertainties[k] = uncertainty;
  }

  void DataPoint::setBinEntriesAt(const std::vector<unsigned int> & indices, long entries)
  {
  _entries[binIndex(indices,1)] = entries;
  }

  void DataPoint::setBinValueAt(const std::vector<unsigned int> & indices, double value)
  {
  _values[binIndex(indices,1)] = values;
  }

  void DataPoint::setBinUncertaintyAt(const std::vector<unsigned int> & indices, double uncertainty)
  {
  _uncertainties[binIndex(indices,1)] = uncertainty;
  }

  void DataPoint::setBinAt(const std::vector<unsigned int> & indices, long entries, double value, double uncertainty)
  {
  unsigned int k = binIndex(indices,1);
  _entries[k]  = entries;
  _values[k]   = value;
  _uncertainties[k] = uncertainty;
  }

  void DataPoint::setBinEntriesAt(double x, long entries)
  {
  _entries[binIndex(x)] = entries;
  }

  void DataPoint::setBinValueAt(double x, double value)
  {
  _values[binIndex(x)] = value;
  }

  void DataPoint::setBinUncertaintyAt(double x, double uncertainty)
  {
  _uncertainties[binIndex(x)] = uncertainty;
  }

  void DataPoint::setBinAt(double x, long entries, double value, double uncertainty)
  {
  unsigned int k = binIndex(x);
  _entries[k]  = entries;
  _values[k]   = value;
  _uncertainties[k] = uncertainty;
  }

  void DataPoint::setBinEntriesAt(double x0, double x1, long entries)
  {
  _entries[binIndex(x0,x1)] = entries;
  }

  void DataPoint::setBinValueAt(double x0, double x1, , double value)
  {
  _values[binIndex(x0,x1)] = value;
  }

  void DataPoint::setBinUncertaintyAt(double x0, double x1, , double uncertainty)
  {
  _uncertainties[binIndex(x0,x1)] = uncertainty;
  }

  void DataPoint::setBinAt(double x0, double x1, , long entries, double value, double uncertainty)
  {
  unsigned int k = binIndex(x0,x1);
  _entries[k]  = entries;
  _values[k]   = value;
  _uncertainties[k] = uncertainty;
  }

  void DataPoint::setBinEntriesAt(double x0, double x1, double x2, long entries)
  {
  _entries[binIndex(x0,x1,x2)] = entries;
  }

  void DataPoint::setBinValueAt(double x0, double x1, double x2, double value)
  {
  _values[binIndex(x0,x1,x2)] = value;
  }

  void DataPoint::setBinUncertaintyAt(double x0, double x1, double x2, double uncertainty)
  {
  _uncertainties[binIndex(x0,x1)] = uncertainty;
  }

  void DataPoint::setBinAt(double x0, double x1, double x2, long entries, double value, double uncertainty)
  {
  unsigned int k = binIndex(x0,x1,x2);
  _entries[k]  = entries;
  _values[k]   = value;
  _uncertainties[k] = uncertainty;
  }

  void DataPoint::setBinEntriesAt(const std::vector<double> & x, long entries)
  {
  _entries[binIndex(x)] = entries;
  }

  void DataPoint::setBinValueAt(const std::vector<double> & x, double value)
  {
  _values[binIndex(x)] = value;
  }

  void DataPoint::setBinUncertaintyAt(const std::vector<double> & x, double uncertainty)
  {
  _uncertainties[binIndex(x)] = uncertainty;
  }

  void DataPoint::setBinAt(const std::vector<double> & x, long entries, double value, double uncertainty)
  {
  unsigned int k = binIndex(x);
  _entries[k]  = entries;
  _values[k]   = value;
  _uncertainties[k] = uncertainty;
  }

  DataPoint DataPoint::extract1(unsigned int first, unsigned int last)
  {
  if (rank()!=1)
    throw Exception("Operation invalid on DataPoint of different dimensions",__FUNCTION__);
  if (last<=first) Exception("last<=first",__FUNCTION__);
  Partition partition;
  partition.addDimension(last-first, binLowEdge(first), binHighEdge(last),dimensionTitle(0);
                         DataPoint extracted(partition);
                         unsigned int iTarget=0;
                         for (unsigned int iSource=first; iSource<=last; iSource++)
                         {
  extracted._entries[iTarget]       = _entries[iSource];
  extracted._values[iTarget]        = _values[iSource];
  extracted._uncertainties[iTarget] = _uncertainties[iSource];
  iTarget++;
  }
                         return extracted;
  }


  DataPoint DataPoint::extract2(unsigned int first1, unsigned int last1,
                                unsigned int first2, unsigned int last2)
  {
  if (rank()!=2)
    throw Exception("Operation invalid on DataPoint of different dimensions",__FUNCTION__);
  Partition partition = Partition::createPartition(first1,last1,first2,last2);
  DataPoint extracted(partition);
  unsigned int iTarget1=0;
  for (unsigned int iSource1=first1; iSource1<=last1; iSource1++)
    {
    unsigned int iTarget2=0;
    for (unsigned int iSource2=first2; iSource2<=last2; iSource2++)
      {
      unsigned int indexSource = indexFor(iSource1,iSource2);
      unsigned int indexTarget = indexFor(iTarget1,iTarget2);
      extracted._entries[indexTarget]       = _entries[indexSource];
      extracted._values[indexTarget]        = _values[indexSource];
      extracted._uncertainties[indexTarget] = _uncertainties[indexSource];
      iTarget2++;
      }
    iTarget1++;
    }
  return extracted;
  }

  DataPoint DataPoint::extract3(unsigned int first1, unsigned int last1,
                                unsigned int first2, unsigned int last2,
                                unsigned int first3, unsigned int last3)
  {
  if (rank()!=3)
    throw Exception("Operation invalid on DataPoint of different dimensions",__FUNCTION__);
  Partition partition = createPartition(first1,last1,first2,last2,first3,last3);
  DataPoint extracted(partition);
  unsigned int iTarget1=0;
  for (unsigned int iSource1=first1; iSource1<=last1; iSource1++)
    {
    unsigned int iTarget2=0;
    for (unsigned int iSource2=first2; iSource2<=last2; iSource2++)
      {
      unsigned int iTarget2=0;
      for (unsigned int iSource3=first3; iSource3<=last3; iSource3++)
        {
        unsigned int indexSource = indexFor(iSource1,iSource2,iSource3);
        unsigned int indexTarget = indexFor(iTarget1,iTarget2,iTarget3);
        extracted._entries[indexTarget]       = _entries[indexSource];
        extracted._values[indexTarget]        = _values[indexSource];
        extracted._uncertainties[indexTarget] = _uncertainties[indexSource];
        iTarget3++;
        }
      iTarget2++;
      }
    iTarget1++;
    }
  return extracted;
  }

  DataPoint DataPoint::project2DOnto1D(unsigned int targetAxisIndex, unsigned int first, unsigned int last)
  {
  if (rank()!=2)
    throw Exception("Operation invalid on DataPoint of different dimensions",__FUNCTION__);
  Partition partition = Partition::createPartition(targetAxisIndex,first,last);
  DataPoint extracted(partition);
  unsigned int iTarget1=0;
  for (unsigned int iSource1=first1; iSource1<=last1; iSource1++)
    {
    unsigned int iTarget2=0;
    for (unsigned int iSource2=first2; iSource2<=last2; iSource2++)
      {
      unsigned int indexSource = indexFor(iSource1,iSource2);
      unsigned int indexTarget = indexFor(iTarget1,iTarget2);
      extracted._entries[indexTarget]       = _entries[indexSource];
      extracted._values[indexTarget]        = _values[indexSource];
      extracted._uncertainties[indexTarget] = _uncertainties[indexSource];
      iTarget2++;
      }
    iTarget1++;
    }
  return extracted;
  }

  DataPoint DataPoint::project3DOnto1D(unsigned int targetAxisIndex,
                                       unsigned int first1, unsigned int last1,
                                       unsigned int first2, unsigned int last2)
  {
  DataPoint histo;
  return histo;
  }


  DataPoint DataPoint::project3DOnto2D(unsigned int targetAxisIndex1, unsigned int targetAxisIndex2,
                                       unsigned int first, unsigned int last)
  {
  DataPoint histo;
  return histo;
  }

  double totalEntries() const
  {
  double sum = 0;
  for (auto & entry : _entries) sum += entry;
  return sum;
  }

  double totalEntries(unsigned int first, unsigned int last) const
  {
  if (rank()!=1) throw Exception(rank(),"Invalid function for histogram of this rank",__FUNCTION__);
  unsigned int n = nBins(0);
  unsigned int iFirst = first >= n ? 0 : first;
  unsigned int iLast = last  >= n ? 0 : last;
  if (iLast<iFirst) throw InvalidBoundException(iFirst,iLast,"last<first",__FUNCTION__);
  double sum = 0.0;
  for (unsigned int k=iFirst; k<iLast; k++)
    {
    sum += _entries[k];
    }
  return sum;
  }

  double totalEntries(unsigned int first0, unsigned int last0,
                      unsigned int first1, unsigned int last1) const
  {
  if (rank()!=2) throw Exception(rank(),"Invalid function for histogram of this rank",__FUNCTION__);
  unsigned int n0      = nBins(0);
  unsigned int iFirst0 = first0 >= n0 ? 0 : first0;
  unsigned int iLast0  = last0  >= n0 ? 0 : last0;
  if (iLast0<iFirst0) throw InvalidBoundException(iFirst0,iLast0,"last0<first0",__FUNCTION__);
  unsigned int n1      = nBins(1);
  unsigned int iFirst1 = first1 >= n1 ? 0 : first1;
  unsigned int iLast1  = last1  >= n1 ? 0 : last1;
  if (iLast1<iFirst1) throw InvalidBoundException(iFirst1,iLast1,"last1<first1",__FUNCTION__);
  double sum = 0.0;
  for (unsigned int k0=iFirst0; k0<iLast0; k0++)
    {
    for (unsigned int k1=iFirst1; k1<iLast1; k1++)
      {
      unsigned int index = binIndex(k0,k1);
      sum += _entries[index];
      }
    }
  return sum;
  }

  double totalEntries(unsigned int first0, unsigned int last0,
                      unsigned int first1, unsigned int last1,
                      unsigned int first2, unsigned int last2) const
  {
  if (rank()!=3) throw Exception(rank(),"Invalid function for histogram of this rank",__FUNCTION__);
  unsigned int n0      = nBins(0);
  unsigned int iFirst0 = first0 >= n0 ? 0 : first0;
  unsigned int iLast0  = last0  >= n0 ? 0 : last0;
  if (iLast0<iFirst0) throw InvalidBoundException(iFirst0,iLast0,"last0<first0",__FUNCTION__);
  unsigned int n1      = nBins(1);
  unsigned int iFirst1 = first1 >= n1 ? 0 : first1;
  unsigned int iLast1  = last1  >= n1 ? 0 : last1;
  if (iLast1<iFirst1) throw InvalidBoundException(iFirst1,iLast1,"last1<first1",__FUNCTION__);
  unsigned int n2      = nBins(2);
  unsigned int iFirst2 = first2 >= n2 ? 0 : first2;
  unsigned int iLast2  = last2  >= n2 ? 0 : last2;
  if (iLast2<iFirst2) throw InvalidBoundException(iFirst2,iLast2,"last2<first2",__FUNCTION__);
  double sum = 0.0;
  for (unsigned int k0=iFirst0; k0<iLast0; k0++)
    {
    for (unsigned int k1=iFirst1; k1<iLast1; k1++)
      {
      for (unsigned int k2=iFirst2; k2<iLast2; k21++)
        {
        unsigned int index = binIndex(k0,k1,k2);
        sum += _entries[index];
        }
      }
    }
  return sum;
  }

  double integral(bool binSumOnly) const
  {
  double sum = 0.0;
  for (unsigned int k=0; k<_entries.size(); k++)
    {
    sum += _entries[k]*_values[k];
    }
  if (binSumOnly)
    return sum;
  else
    {
    double w = 1;
    for (unsigned int dim=0; dim<rank(); dim++) w *= _widths(dim);
    return sum*s; // needs to be update for variable widths
    }
  }

  //!
  //!Compute/return the sum or integral of the bin content
  //!Use first1==-1  &&  last1==-1 to automatically get the full range of the histogram
  //!
  double DataPoint::integral(unsigned int first, unsigned int last, bool binSumOnly) const
  {
  if (rank()!=1) throw Exception(rank(),"Invalid function for histogram of this rank",__FUNCTION__);
  unsigned int n = nBins(0);
  unsigned int iFirst = first >= n ? 0 : first;
  unsigned int iLast = last  >= n ? 0 : last;
  if (iLast<iFirst) throw InvalidBoundException(iFirst,iLast,"last<first",__FUNCTION__);
  double sum = 0.0;
  for (unsigned int k=iFirst; k<iLast; k++)
    {
    sum += _entries[k]*_values[k];
    }
  if (binSumOnly)
    return sum;
  else
    return sum*width(0); // needs to be update for variable widths
  }

  double DataPoint::integral(unsigned int first0, unsigned int last0,
                             unsigned int first1, unsigned int last1,
                             bool binSumOnly)  const
  {
  if (rank()!=2) throw Exception(rank(),"Invalid function for histogram of this rank",__FUNCTION__);
  unsigned int n0      = nBins(0);
  unsigned int iFirst0 = first0 >= n0 ? 0 : first0;
  unsigned int iLast0  = last0  >= n0 ? 0 : last0;
  if (iLast0<iFirst0) throw InvalidBoundException(iFirst0,iLast0,"last0<first0",__FUNCTION__);
  unsigned int n1      = nBins(1);
  unsigned int iFirst1 = first1 >= n1 ? 0 : first1;
  unsigned int iLast1  = last1  >= n1 ? 0 : last1;
  if (iLast1<iFirst1) throw InvalidBoundException(iFirst1,iLast1,"last1<first1",__FUNCTION__);
  double sum = 0.0;
  for (unsigned int k0=iFirst0; k0<iLast0; k0++)
    {
    for (unsigned int k1=iFirst1; k1<iLast1; k1++)
      {
      unsigned int index = binIndex(k0,k1);
      sum += _entries[index]*_values[index];
      }
    }
  if (binSumOnly)
    return sum;
  else
    return sum*width(0)*width(1); // needs to be update for variable widths
  }



  double DataPoint::integral(unsigned int first0, unsigned int last0,
                             unsigned int first1, unsigned int last1,
                             unsigned int first2, unsigned int last2,
                             bool binSumOnly)  const
  {
  if (rank()!=3) throw Exception(rank(),"Invalid function for histogram of this rank",__FUNCTION__);
  unsigned int n0      = nBins(0);
  unsigned int iFirst0 = first0 >= n0 ? 0 : first0;
  unsigned int iLast0  = last0  >= n0 ? 0 : last0;
  if (iLast0<iFirst0) throw InvalidBoundException(iFirst0,iLast0,"last0<first0",__FUNCTION__);
  unsigned int n1      = nBins(1);
  unsigned int iFirst1 = first1 >= n1 ? 0 : first1;
  unsigned int iLast1  = last1  >= n1 ? 0 : last1;
  if (iLast1<iFirst1) throw InvalidBoundException(iFirst1,iLast1,"last1<first1",__FUNCTION__);
  unsigned int n1      = nBins(1);
  unsigned int iFirst1 = first1 >= n1 ? 0 : first1;
  unsigned int iLast1  = last1  >= n1 ? 0 : last1;
  if (iLast2<iFirst2) throw InvalidBoundException(iFirst2,iLast2,"last2<first2",__FUNCTION__);
  double sum = 0.0;
  for (unsigned int k0=iFirst0; k0<iLast0; k0++)
    {
    for (unsigned int k1=iFirst1; k1<iLast1; k1++)
      {
      for (unsigned int k2=iFirst2; k2<iLast2; k2++)
        {
        unsigned int index = binIndex(k0,k1,k2);
        sum += _entries[index]*_values[index];
        }
      }
    }
  if (binSumOnly)
    return sum;
  else
    return sum*width(0)*width(1)*width(2); // needs to be update for variable widths
  }


  //!
  //!Clear this histogram. Effectively, it becomes undefined.
  //!
  void DataPoint::clear()
  {
  Partition::clear();
  _entries.clear();
  _values.clear();
  _uncertainties.clear();
  }

  //!
  //!Reset the bin contant to zero without chaning the rank and dimensions of this histogram.
  //!
  void DataPoint::reset()
  {
  for (auto & v : _entries) v=0;
  for (auto & v : _values) v=0;
  for (auto & v : _uncertainties) v=0;
  }

  void DataPoint::fill(double x, double weight)
  {
  int  index = binIndex(x);
  if (index<0) return;
  double n = _entries[index];
  _values[n] += (n*_values[index] + weight)/(n+1);
  _entries[index]++;
  }

  void DataPoint::fill(double x0, double x1, double weight)
  {
  int  index = binIndex(x0,x1);
  if (index<0) return;
  double n = _entries[index];
  _values[n] += (n*_values[index] + weight)/(n+1);
  _entries[index]++;
  }

  void DataPoint::fill(double x0, double x1, double x2, double weight)
  {
  int  index = binIndex(x0,x1,x2);
  if (index<0) return;
  double n = _entries[index];
  _values[n] += (n*_values[index] + weight)/(n+1);
  _entries[index]++;
  }

  void DataPoint::fill(std::vector<double> & x, double weight)
  {
  int  index = binIndex(x);
  if (index<0) return;
  double n = _entries[index];
  _values[n] += (n*_values[index] + weight)/(n+1);
  _entries[index]++;
  }

  double DataPoint::binEntriesAt(unsigned int binIndex) const
  {
  if (!isValidIndex(binIndex))
    throw IndexOutOfBoundException(binIndex,"binIndex out of bound",__FUNCTION__);
  return _entries[binIndex];
  }


  double DataPoint::binEntriesAt(unsigned int binIndex0, unsigned int binIndex1) const
  {
  unsigned int binIndex;
  if (!isValidIndex(binIndex0,binIndex1,binIndex))
    throw IndexOutOfBoundException(binIndex0,binIndex1,"bin indices out of bound",__FUNCTION__);
  return _entries[binIndex];
  }

  double DataPoint::binEntriesAt(unsigned int binIndex0, unsigned int binIndex1, unsigned int binIndex2) const
  {
  unsigned int binIndex;
  if (!isValidIndex(binIndex0,binIndex1,binIndex2,binIndex))
    throw IndexOutOfBoundException(binIndex0,binIndex1,,binIndex2,"bin indices out of bound",__FUNCTION__);
  return _entries[binIndex];
  }


  double DataPoint::binEntriesAt(const std::vector<unsigned int> & binIndices) const
  {
  unsigned int binIndex;
  if (!isValidIndex(binIndices,binIndex))
    throw IndexOutOfBoundException(binIndices,"bin indices out of bound",__FUNCTION__);
  return _entries[binIndex];
  }

  double DataPoint::binValueAt(unsigned int binIndex) const
  {
  if (binIndex>=nBinsTotal()) throw IndexOutOfBoundException(binIndex,"binIndex out of bound",__FUNCTION__);
  return _values[binIndex]*_entries[binIndex];
  }

  double DataPoint::binValueAt(unsigned int binIndex0, unsigned int binIndex1) const
  {
  unsigned int binIndex;
  if (!isValidIndex(binIndex0,binIndex1,binIndex))
    throw IndexOutOfBoundException(binIndex0,binIndex1,"bin indices out of bound",__FUNCTION__);
  return _values[binIndex]*_entries[binIndex];
  }

  double DataPoint::binValueAt(unsigned int binIndex0, unsigned int binIndex1, unsigned int binIndex2) const
  {
  unsigned int binIndex;
  if (!isValidIndex(binIndex0,binIndex1,binIndex2,binIndex))
    throw IndexOutOfBoundException(binIndex0,binIndex1,,binIndex2,"bin indices out of bound",__FUNCTION__);
  return _values[binIndex]*_entries[binIndex];
  }


  double DataPoint::binValueAt(const std::vector<unsigned int> & indices) const
  {
  unsigned int binIndex;
  if (!isValidIndex(binIndices,binIndex))
    throw IndexOutOfBoundException(binIndices,"bin indices out of bound",__FUNCTION__);
  return _values[binIndex]*_entries[binIndex];
  }

  double DataPoint::binUncertaintyAt(unsigned int binIndex) const
  {
  if (binIndex>=nBinsTotal()) throw IndexOutOfBoundException(binIndex,"binIndex out of bound",__FUNCTION__);
  return _uncertainties[binIndex]*_entries[binIndex];
  }

  double DataPoint::binUncertaintyAt(unsigned int binIndex0, unsigned int binIndex1) const
  {
  unsigned int binIndex;
  if (!isValidIndex(binIndex0,binIndex1,binIndex))
    throw IndexOutOfBoundException(binIndex0,binIndex1,"bin indices out of bound",__FUNCTION__);
  return _uncertainties[binIndex]*_entries[binIndex];
  }

  double DataPoint::binUncertaintyAt(unsigned int binIndex0, unsigned int binIndex1, unsigned int binIndex2) const
  {
  unsigned int binIndex;
  if (!isValidIndex(binIndex0,binIndex1,binIndex2,binIndex))
    throw IndexOutOfBoundException(binIndex0,binIndex1,,binIndex2,"bin indices out of bound",__FUNCTION__);
  return _uncertainties[binIndex]*_entries[binIndex];
  }

  double DataPoint::binUncertaintyAt(const std::vector<unsigned int> & indices) const
  {
  unsigned int binIndex;
  if (!isValidIndex(binIndices,binIndex))
    throw IndexOutOfBoundException(binIndices,"bin indices out of bound",__FUNCTION__);
  return _values[binIndex]*_entries[binIndex];
  }

  double DataPoint::binAvgValueAt(unsigned int binIndex) const
  {
  if (binIndex>=nBinsTotal()) throw IndexOutOfBoundException(binIndex,"binIndex out of bound",__FUNCTION__);
  return _values[binIndex];
  }

  double DataPoint::binAvgValueAt(unsigned int binIndex0, unsigned int binIndex1) const
  {
  unsigned int binIndex;
  if (!isValidIndex(binIndex0,binIndex1,binIndex))
    throw IndexOutOfBoundException(binIndex0,binIndex1,"bin indices out of bound",__FUNCTION__);
  return _values[binIndex];
  }

  double DataPoint::binAvgValueAt(unsigned int binIndex0, unsigned int binIndex1, unsigned int binIndex2) const
  {
  unsigned int binIndex;
  if (!isValidIndex(binIndex0,binIndex1,binIndex2,binIndex))
    throw IndexOutOfBoundException(binIndex0,binIndex1,binIndex2,"bin indices out of bound",__FUNCTION__);
  return _values[binIndex];
  }

  double DataPoint::binAvgValueAt(const std::vector<unsigned int> & indices) const
  {
  unsigned int binIndex;
  if (!isValidIndex(binIndices,binIndex))
    throw IndexOutOfBoundException(binIndices,"bin indices out of bound",__FUNCTION__);
  return _values[binIndex];
  }

  double DataPoint::binAvgValueUncertaintyAt(unsigned int binIndex) const
  {
  if (binIndex>=nBinsTotal()) throw IndexOutOfBoundException(binIndex,"binIndex out of bound",__FUNCTION__);
  return _uncertainties[binIndex];
  }

  double DataPoint::binAvgValueUncertaintyAt(unsigned int binIndex0, unsigned int binIndex1) const
  {
  unsigned int binIndex;
  if (!isValidIndex(binIndex0,binIndex1,binIndex))
    throw IndexOutOfBoundException(binIndex0,binIndex1,"bin indices out of bound",__FUNCTION__);
  return _uncertainties[binIndex];
  }

  double DataPoint::binAvgValueUncertaintyAt(unsigned int binIndex0, unsigned int binIndex1, unsigned int binIndex2) const
  {
  unsigned int binIndex;
  if (!isValidIndex(binIndex0,binIndex1,binIndex2,binIndex))
    throw IndexOutOfBoundException(binIndex0,binIndex1,binIndex2,"bin indices out of bound",__FUNCTION__);
  return _uncertainties[binIndex];
  }

  double DataPoint::binAvgValueUncertaintyAt(const std::vector<unsigned int> & indices) const
  {
  unsigned int binIndex;
  if (!isValidIndex(binIndices,binIndex))
    throw IndexOutOfBoundException(binIndices,"bin indices out of bound",__FUNCTION__);
  return _values[binIndex];
  }

  std::vector<double> DataPoint::binValues()
  {
  std::vector<double> result;
  for (unsigned int k=0; k<_values.size(); k++)
    {
    double num = _values[k];
    double denum = _entries[k];
    if (denum>0)
      result.push_back(num*denum);
    else
      result.push_back(-1.0E100); // to indicate no data is available in this bin
    }
  }

  std::vector<double> DataPoint::binValueUncertainties()
  {
  std::vector<double> result;
  for (unsigned int k=0; k<_values.size(); k++)
    {
    double num   = _uncertainties[k];
    double denum = _entries[k];
    if (denum>0)
      result.push_back(num*denum);
    else
      result.push_back(-1.0E100); // to indicate no data is available in this bin
    }
  }


  DataPoints   DataPoint::dataPoints(bool average, bool excludeEmptyBins) const
  {
  DataPoints dataPoints;
  for (unsigned int k=0; k<_values.size(); k++)
    {
    double uncertainty = _uncertainties[k];
    double value       = _values[k];
    double entries     = _entries[k];
    if (entries<1  &&  excludeEmptyBins) continue;
    if (average)
      dataPoints.addPoint(DataPoint(value,uncertainty));
    else
      dataPoints.addPoint(DataPoint(entries*value,entries*uncertainty));
    }
  }

  DataVectors  DataPoint::dataVectors() const
  {
  DataVectors dataVectors;
  for (unsigned int k=0; k<_values.size(); k++)
    {
    double uncertainty = _uncertainties[k];
    double value       = _values[k];
    double entries     = _entries[k];
    if (entries<1  &&  excludeEmptyBins) continue;
    if (average)
      dataVectors.addPoint(value,uncertainty);
    else
      dataVectors.addPoint(DataPoint(entries*value,entries*uncertainty));
    }
  }



  void DataPoint::print() const
  {
  printValue("DataPoint Name:",name());
  printValue("DataPoint Title:",title());
  printValue("DataPoint Rank:",rank());
  std::cout << " Bin Index    Entries   Values    dValues    AvgValues   dAvgValue" << endl;
  for (unsigned int k=0; k<nBins(); k++)
    {
    std::cout
    <<  k << "   "
    << _entries[k] << "   "
    << _entries[k]*_values[k] << "  "
    << _entries[k]*_uncertainties[k] << "  "
    << _values[k] << "  "
    << _uncertainties[k] << std::endl;
    }
  }


  DataPoint operator+(const DataPoint & left, const DataPoint & right)
  {
  if (!left.compatibleWith(right))
    throw Exception("Operation invalid on DataPoint of different dimensions",__FUNCTION__);
  DataPoint result(left);
  result += right;
  return result;
  }

  DataPoint operator-(const DataPoint & left, const DataPoint & right)
  {
  if (!left.compatibleWith(right))
    throw Exception("Operation invalid on DataPoint of different dimensions",__FUNCTION__);
  DataPoint result(left);
  result -= right;
  return result;
  }

  DataPoint operator*(const DataPoint & left, const DataPoint & right)
    {
    if (!left.compatibleWith(right))
      throw Exception("Operation invalid on DataPoint of different dimensions",__FUNCTION__);
    DataPoint result(left);
    result *= right;
    return result;
    }

  DataPoint operator/(const DataPoint & left, const DataPoint & right)
  {
  if (!left.compatibleWith(right))
    throw Exception("Operation invalid on DataPoint of different dimensions",__FUNCTION__);
  DataPoint result(left);
  result /= right;
  return result;
  }

  DataPoint operator+(const DataPoint & left, double right)
  {
  DataPoint result(left);
  result.add(right);
  return result;
  }

  DataPoint operator+(double left, const DataPoint & right)
  {
  DataPoint result(right);
  result.add(left);
  return result;
  }

  DataPoint operator-(const DataPoint & left, double right)
  {
  DataPoint result(left);
  result.add(-right);
  return result;
  }

  DataPoint operator-(double left, const DataPoint & right)
  {
  DataPoint result(right);
  result.scale(-1.0)
  result.add(left);
  return result;
  }


  DataPoint operator*(const DataPoint & left, double right)
    {
    DataPoint result(left);
    result.scale(right);
    return result;
    }

  DataPoint operator* (double left, const DataPoint & right)
    {
    DataPoint result(right);
    result.scale(left);
    return result;
    }

  DataPoint operator/(const DataPoint & left, double right)
  {
  DataPoint result(left);
  if (right==0)
    throw Exception("Attempting division by zero",__FUNCTION__);
  result.scale(1.0/right);
  return result;
  }

  DataPoint operator/(double left, const DataPoint & right)
  {
  DataPoint result(right);
  result.inverse(left);
  result.scale(left);
  return result;
  }

  DataPoint operator+(const DataPoint & right)
  {
  DataPoint result(right);
  return result;
  }

  DataPoint operator-(const DataPoint & right)
  {
  DataPoint result(right);
  result.scale(-1.0)
  return result;
  }


  std::ostream& operator<<(std::ostream& out, const DataPoint & right)
  {
  printValue("DataPoint Name:",name());
  printValue("DataPoint Title:",title());
  printValue("DataPoint Rank:",rank());
  std::cout << " Bin Index    Entries   Values    dValues    AvgValues   dAvgValue" << endl;
  for (unsigned int k=0; k<nBins(); k++)
    {
    std::cout
    <<  k << "   "
    << _entries[k] << "   "
    << _entries[k]*_values[k] << "  "
    << _entries[k]*_uncertainties[k] << "  "
    << _values[k] << "  "
    << _uncertainties[k] << std::endl;
    }
  }




  } // Math
} // namespace CAP
