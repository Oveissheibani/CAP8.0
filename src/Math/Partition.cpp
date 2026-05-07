#include "Partition.hpp"
#include "Exceptions.hpp"

ClassImp(CAP::MATH::Partition);


namespace CAP
{
  namespace MATH
  {

  Partition::Partition()
  :
  _rank(0),
  _type(0),
  _nBinsTotal(0),
  _nBins(),
  _minima(),
  _maxima(),
  _widths(),
  _titles(),
  { }

  Partition::Partition(const Partition & src)
  :
  _rank(src._rank),
  _type(src._type),
  _nBinsTotal(src._nBinsTotal),
  _nBins(src._nBins),
  _minima(src._minima),
  _maxima(src._maxima),
  _widths(src._widths),
  _titles(src._titles),
  { }

  Partition::~Partition()
  {
  clear();
  }

  Partition & Partition::operator=(const Partition & rhs)
  {
  if (this!=&rhs)
    {
    _rank       = rhs._rank;
    _type       = rhs._type;
    _nBinsTotal = rhs._nBinsTotal;
    _nBins  = rhs._nBins;
    _minima = rhs._minima;
    _maxima = rhs._maxima;
    _widths = rhs._widths;
    _titles = rhs._titles;
    _rank   = rhs._rank;
    _type   = rhs._type;
    }
  return *this;
  }

  void Partition::addDimension(const Partition & partition)
  {
  unsigned int addedRank = partition.rank();
  for (unsigned int k=0; k<addedRank; k++)
    {
    addDimension(partition._nBin[k],partition._minimum[k],partition._maximum[k],partition._title[k]);
    }
  }

  void Partition::addDimension(unsigned int nBin, float minimum, float maximum, String title)
  {
  if (nBin<1) throw Exception("Invalid dimension size nBins<1",__FUNCTION__);
  if (minimum>=maximum) throw Exception("Invalid dimension range minimum>=maximum",__FUNCTION__);
  _nBinsTotal *= nBin;
  _nBins.push_back(nBin);
  _minima.push_back(minimum);
  _maxima.push_back(maximum);
  _widths.push_back((maximum-minimum)/double(nBin));
  _titles.push_back(title);
  _rank++;
  }

  bool Partition::compatibleWith(const Partition & rhs, bool sizeAndLimits) const
  {
  if (_rank!=rhs.rank()) return false;
  for (unsigned int k=0; k<_rank; k++)
    {
    if (_nBins[k]!=rhs._nBins[k])  return false;
    if (sizeAndLimits)
      {
      if (_minima[k]!=rhs._minima[k])  return false;
      if (_maxima[k]!=rhs._maxima[k])  return false;
      }
    }
  return true;
  }

  void Partition::clear()
  {
  _rank = 0;
  _type = 0;
  _nBinsTotal = 0;
  _nBins.clear();
  _minima.clear();
  _maxima.clear();
  _widths.clear();
  _titles.clear();
  }

  void Partition::createPartition(const Partition & partition1,const Partition & partition2)
  {
  clear();
  addDimension(partition1);
  addDimension(partition2);
  }

  unsigned int Partition::nBins(unsigned int dimIndex=0) const
  {
  if (dimIndex>=_rank) throw IndexOutOfBoundException(dimIndex,"Dimension index out of bound",__FUNCTION__);
  return nBins[dimIndex];
  }

  unsigned int Partition::nBinsTotal() const
  {
  unsigned int n = 1;
  for (auto & nBin : _nBins) n *= nBin;
  return n;
  }

  int Partition::binIndex(unsigned int index, bool isStrict) const
  {
  if (_rank!=1) throw Exception("Partition has rank!=1",__FUNCTION__);
  if (index >= _nBins[0])
    {
    if (isStrict)
      throw IndexOutOfBoundException(index,_nBins[0],"Out of bound index",__FUNCTION__);
    else
      return -1;
    }
  return index;
  }

   int Partition::binIndex(unsigned int index0,unsigned int index1, bool isStrict) const
  {
  if (_rank!=2) throw Exception("Partition has rank!=2",__FUNCTION__);
  if (index0 >= _nBins[0])
    {
    if (isStrict)
      throw IndexOutOfBoundException(index0,_nBins[0],"Out of bound index",__FUNCTION__);
    else
      return -1;
    }
  if (index1 >= _nBins[1])
    {
    if (isStrict)
      throw IndexOutOfBoundException(index1,_nBins[1],"Out of bound index",__FUNCTION__);
    else
      return -1;
    }
  return index1*_nBins[0] + index0;
  }

   int Partition::binIndex(unsigned int index0,unsigned int index1,unsigned int index2, bool isStrict) const
  {
  if (_rank!=3) throw Exception("Partition has rank!=2",__FUNCTION__);
  if (index0 >= _nBins[0])
    {
    if (isStrict)
      throw IndexOutOfBoundException(index0,_nBins[0],"Out of bound index",__FUNCTION__);
    else
      return -1;
    }
  if (index1 >= _nBins[1])
    {
    if (isStrict)
      throw IndexOutOfBoundException(index1,_nBins[1],"Out of bound index",__FUNCTION__);
    else
      return -1;
    }
  if (index2 >= _nBins[2])
    {
    if (isStrict)
      throw IndexOutOfBoundException(index2,_nBins[2],"Out of bound index",__FUNCTION__);
    else
      return -1;
    }
  return index2*_nBins[1]*_nBins[0] + index1*_nBins[0] + index0;
  }

   int Partition::binIndex(const std::vector<unsigned int> indices, bool isStrict) const
  {
  unsigned int n = indices.size();
  if (_rank!=n) throw Exception(n,"indices does not match rank",__FUNCTION__);
  unsigned int index = 0;
  unsigned int mult  = 1;
  for (unsigned int k=0; k<_rank; k++)
    {
    if (indices[k] >= _nBins[k])
      {
      if (isStrict)
        throw IndexOutOfBoundException(index2,_nBins[2],"Out of bound index",__FUNCTION__);
      else
        return -1;
      }
    index += mult*indices[k];
    mult *= indices[k];
    }
  return index;
  }

   int Partition::binIndex(double x, unsigned int dimIndex, bool isStrict) const
  {
  if (dimIndex>=_rank) throw IndexOutOfBoundException(dimIndex,"dimIndex >= rank",__FUNCTION__);
  int index = (x- minima[dimIndex])/widths[dimIndex];
  return binIndex(index);
  }

  int Partition::binIndex(double x0, double x1) const
  {
  if (_rank!=2 || x0<minima[0] || x0>=maxima[0] || x1<minima[1] || x1>=maxima[1]) return -1;
  int index0 = (x0- minima[0])/widths[0];
  int index1 = (x1- minima[1])/widths[1];
  return binIndex(index0,index1);
  }


  int Partition::binIndex(double x0, double x1, double x2) const
  {
  if (_rank!=3 || x0<minima[0] || x0>=maxima[0] || x1<minima[1] || x1>=maxima[1] || x2<minima[2] || x2>=maxima[2]) return -1;
  int index0 = (x0- minima[0])/widths[0];
  int index1 = (x1- minima[1])/widths[1];
  int index2 = (x2- minima[2])/widths[2];
  return binIndex(index0,index1,index2);
  }

  int Partition::binIndex(const std::vector<double> & x) const
  {
  if (x.size()!=_rank) throw IndexOutOfBoundException(x.size(),"Rank mismatch for given x",__FUNCTION__);
  for (unsigned int k=0; k<=_rank; k++) indices.push_back((x[k]- minima[k])/widths[k]);
  return binIndex(indices);
  }

  double Partition::minimum(unsigned int dimIndex=0) const
  {
  if (dimIndex>=_rank) throw IndexOutOfBoundException(dimIndex,"Dimension index out of bound",__FUNCTION__);
  return minima[dimIndex];
  }

  double Partition::maximum(unsigned int dimIndex=0) const
  {
  if (dimIndex>=_rank) throw IndexOutOfBoundException(dimIndex,"Dimension index out of bound",__FUNCTION__);
  return minima[dimIndex];
  }

  //! return the bin width -- for now use fixed widths...
  double Partition::binWidth(unsigned int binIndex, unsigned int dimIndex=0) const
  {
  if (dimIndex>=_rank) throw IndexOutOfBoundException(dimIndex,"Dimension index out of bound",__FUNCTION__);
  return width[dimIndex];
  }

  double Partition::binLowEdge(unsigned int binIndex, unsigned int dimIndex=0) const
  {
  if (dimIndex>=_rank) throw IndexOutOfBoundException(dimIndex,"Dimension index out of bound",__FUNCTION__);
  return double(binIndex)*width[dimIndex];
  }

  double Partition::binHighEdge(unsigned int binIndex, unsigned int dimIndex=0) const
  {
  if (dimIndex>=_rank) throw IndexOutOfBoundException(dimIndex,"Dimension index out of bound",__FUNCTION__);
  return double(binIndex+1)*width[dimIndex];
  }

  double Partition::binCenter(unsigned int binIndex, unsigned int dimIndex=0) const
  {
  if (dimIndex>=_rank) throw IndexOutOfBoundException(dimIndex,"Dimension index out of bound",__FUNCTION__);
  return (0.5+double(binIndex))*width[dimIndex];
  }

  Partition Partition::extractPartition(unsigned int k=0)  const
  {
  if (k>=_rank) throw IndexOutOfBoundException(k,"Dimension k out of bound",__FUNCTION__);
  Partition newPartition;
  newPartition.addDimension(_nBins[k], _minimum[k], _maximum[k], _title[k])
  return newPartition;
  }

  Partition Partition::extractPartition(unsigned int k0=0,unsigned int k1=1) const
  {
  if (k0==k1)  throw Exception("Dimension k0 and k1 are the same",__FUNCTION__);
  if (k0>=_rank) throw IndexOutOfBoundException(k0,"Dimension k0 out of bound",__FUNCTION__);
  if (k1>=_rank) throw IndexOutOfBoundException(k1,"Dimension k1 out of bound",__FUNCTION__);
  Partition newPartition;
  newPartition.addDimension(_nBins[k0], _minimum[k0], _maximum[k0], _title[k0])
  newPartition.addDimension(_nBins[k1], _minimum[k1], _maximum[k1], _title[k1])
  return newPartition;
  }

  Partition Partition::extractPartition(unsigned int k0=0,unsigned int k1=1,unsigned int k1=2) const
  {
  if (k0>=_rank) throw IndexOutOfBoundException(k0,"Dimension k0 out of bound",__FUNCTION__);
  if (k1>=_rank) throw IndexOutOfBoundException(k1,"Dimension k1 out of bound",__FUNCTION__);
  if (k2>=_rank) throw IndexOutOfBoundException(k2,"Dimension k2 out of bound",__FUNCTION__);
  if (k0==k1)  throw Exception("Dimension k0, k1 are the same",__FUNCTION__);
  if (k0==k2)  throw Exception("Dimension k0, k2 are the same",__FUNCTION__);
  if (k1==k2)  throw Exception("Dimension k1, k2 are the same",__FUNCTION__);
  Partition newPartition;
  newPartition.addDimension(_nBins[k0], _minimum[k0], _maximum[k0], _title[k0])
  newPartition.addDimension(_nBins[k1], _minimum[k1], _maximum[k1], _title[k1])
  newPartition.addDimension(_nBins[k2], _minimum[k2], _maximum[k2], _title[k2])
  return newPartition;
  }

  const String & Partition::dimensionTitle(unsigned int k=0) const
  {
  if (k>=_rank) throw IndexOutOfBoundException(k,"Dimension k0 out of bound",__FUNCTION__);
  return _titles[k];
  }


} // namespace CAP


