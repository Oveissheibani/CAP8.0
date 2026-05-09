#ifndef CAP_Histogram
#define CAP_Histogram
#include "Object.hpp"
#include "Partition.hpp"
#include "DataPoints.hpp"
#include "DataVectors.hpp"

namespace CAP
{
  namespace MATH
  {

  class Histogram
  :
  public Object,
  public Partition
  {
  protected:
  unsigned int nBins;
  std::vector<long>  _entries;
  std::vector<float> _values;
  std::vector<float> _uncertainties;

  inline double statErrorSum(double & e1, double & e2)
  {
  return std::sqrt(e1*e1+e2*e2);
  }

  inline double statErrorProduct(double & v1, double & e1, double & v2, double & e2)
  {
  // do not use relative errors and avoid divisions
  double ep1 = v1*e2;
  double ep2 = v2*e1;
  return std::sqrt(ep1*ep1+ep2*ep2);
  }

  inline double statErrorRatio(double & v1, double & e1, double & v2, double & e2)
  {
  // do not use relative errors and minimize number of divisions
  if (v2==0) return 1.0E100;
  double er2 = e2*v1/v2;
  return std::sqrt(e1*e1+er2*er2)/v2;
  }



  public:

  Histogram();
  Histogram(const Partition & partition);
  Histogram(const String & name, const String & title,
            unsigned int n, double minimum, double maximum);
  Histogram(const String & name, const String & title, unsigned int n0, double minimum_0, double maximum_0, unsigned int n1, double minimum_1, double maximum_1);
  Histogram(const Histogram & source);
  virtual ~Histogram() {};
  Histogram & operator= (const Histogram & rhs);

  bool compatibleWith(const Histogram & source, bool sizeAndLimits=1);

  bool  operator== (const Histogram & rhs) const;
  bool  operator!= (const Histogram & rhs) const;
  Histogram & operator+= (const Histogram & rhs);
  Histogram & operator-= (const Histogram & rhs);
  Histogram & operator*= (const Histogram & rhs);
  Histogram & operator/= (const Histogram & rhs);
  Histogram & operator+= (double value);
  Histogram & operator-= (double value);
  Histogram & operator*= (double value);
  Histogram & operator/= (double value);

  void scale(double coefficient);
  void add(const Histogram & h, double coefficient);
  void multiplyBy(const Histogram & rhs, double coefficient);
  void divideBy(const Histogram & rhs, double coefficient);
  void inverse();

  void setAsSumOf(const Histogram & h1, const Histogram & h2,
                  double coefficient1, double coefficient2);
  void setAsSumOf(const std::vector<const Histogram *> & histograms,
                  const std::vector<double> & coefficientw);

  void setAsProductOf(const Histogram & h1, const Histogram & h2,
                      double coefficient1, double coefficient2);
  void setAsProductOf(const std::vector<const Histogram *> & histograms,
                      const std::vector<double> & coefficientw);
  void setAsRatioOf(const Histogram & h1, const Histogram & h2,
                    double coefficient1, double coefficient2);

  void setBinEntriesAt(unsigned int binIndex, long entries);
  void setBinValueAt(unsigned int binIndex, double value);
  void setBinUncertaintyAt(unsigned int binIndex, double uncertainty);
  void setBinAt(unsigned int binIndex, long entries, double value, double uncertainty);

  void setBinEntriesAt(unsigned int binIndex0, unsigned int binIndex1, long entries);
  void setBinValueAt(unsigned int binIndex0, unsigned int binIndex1, double value);
  void setBinUncertaintyAt(unsigned int binIndex0, unsigned int binIndex1, double uncertainty);
  void setBinAt(unsigned int binIndex0, unsigned int binIndex1, long entries, double value, double uncertainty);

  void setBinEntriesAt(unsigned int binIndex0, unsigned int binIndex1, unsigned int binIndex2, long entries);
  void setBinValueAt(unsigned int binIndex0, unsigned int binIndex1, unsigned int binIndex2, double value);
  void setBinUncertaintyAt(unsigned int binIndex0, unsigned int binIndex1, unsigned int binIndex2, double uncertainty);
  void setBinAt(unsigned int binIndex0, unsigned int binIndex1, unsigned int binIndex2, long entries, double value, double uncertainty);

  void setBinEntriesAt(const std::vector<unsigned int> & binIndices, long entries);
  void setBinValueAt(const std::vector<unsigned int> & binIndices, double value);
  void setBinUncertaintyAt(const std::vector<unsigned int> & binIndices, double uncertainty);
  void setBinAt(const std::vector<unsigned int> & binIndices, long entries, double value, double uncertainty);

  void setBinEntriesAt(double x, long entries);
  void setBinValueAt(double x, double value);
  void setBinUncertaintyAt(double x, double uncertainty);
  void setBinAt(double x, long entries, double value, double uncertainty);

  void setBinEntriesAt(double x0, double x1, long entries);
  void setBinValueAt(double x0, double x1, , double value);
  void setBinUncertaintyAt(double x0, double x1, , double uncertainty);
  void setBinAt(double x0, double x1, , long entries, double value, double uncertainty);

  void setBinEntriesAt(double x0, double x1, double x2, long entries);
  void setBinValueAt(double x0, double x1, double x2, double value);
  void setBinUncertaintyAt(double x0, double x1, double x2, double uncertainty);
  void setBinAt(double x0, double x1, double x2, long entries, double value, double uncertainty);

  void setBinEntriesAt(const std::vector<double> & binIndices, long entries);
  void setBinValueAt(const std::vector<double> & binIndices, double value);
  void setBinUncertaintyAt(const std::vector<double> & binIndices, double uncertainty);
  void setBinAt(const std::vector<double> & binIndices, long entries, double value, double uncertainty);

  void setWith(const DataVectors & dataVector);
  void setWith(const DataPoints & dataVector);

  void setWith(const std::vector<float> & values);
  void setWith(const std::vector<double> & values);
  void setWith(const std::vector<float> & values, const std::vector<float> & uncertainties);
  void setWith(const std::vector<double> & values, const std::vector<double> & uncertainties);
  void setWith(const std::vector<long> & entries,  const std::vector<float> & values);
  void setWith(const std::vector<long> & entries,  const std::vector<double> & values);
  void setWith(const std::vector<long> & entries,  const std::vector<float> & values,  const std::vector<float> & uncertainties);
  void setWith(const std::vector<long> & entries,  const std::vector<double> & values, const std::vector<double> & uncertainties);

  Histogram extract1(unsigned int first, unsigned int last);
  Histogram extract2(unsigned int first1, unsigned int last1,
                     unsigned int first2, unsigned int last2);
  Histogram extract3(unsigned int first1, unsigned int last1,
                     unsigned int first2, unsigned int last2,
                     unsigned int first3, unsigned int last3);

  Histogram project2DOnto1D(unsigned int targetAxisIndex, unsigned int first, unsigned int last);
  Histogram project3DOnto1D(unsigned int targetAxisIndex,
                            unsigned int first1, unsigned int last1,
                            unsigned int first2, unsigned int last2);
  Histogram project3DOnto2D(unsigned int targetAxisIndex1, unsigned int targetAxisIndex2,
                            unsigned int first, unsigned int last);

  double totalEntries() const;
  double totalEntries(unsigned int first, unsigned int last) const;
  double totalEntries(unsigned int first0, unsigned int last0,
                      unsigned int first1, unsigned int last1) const;
  double totalEntries(unsigned int first0, unsigned int last0,
                      unsigned int first1, unsigned int last1,
                      unsigned int first2, unsigned int last2) const;

  double integral(bool binSumOnly=1) const;
  double integral(unsigned int first, unsigned int last,
                  bool binSumOnly=1)  const;
  double integral(unsigned int first0, unsigned int last0,
                  unsigned int first1, unsigned int last1,
                  bool binSumOnly=1) const;
  double integral(unsigned int first0, unsigned int last0,
                  unsigned int first1, unsigned int last1,
                  unsigned int first2, unsigned int last2,
                  bool binSumOnly=1) const;

  void maximumEntries(unsigned int & index, long & value, unsigned int first=0, unsigned int last= 0);
  void minimumEntries(unsigned int & index, long & value, unsigned int first=0, unsigned int last= 0);
  void maximumValue(unsigned int & index, long & value, unsigned int first=0, unsigned int last= 0);
  void minimumValue(unsigned int & index, long & value, unsigned int first=0, unsigned int last= 0);
  void maximumAvgValue(unsigned int & index, long & value, unsigned int first=0, unsigned int last= 0);
  void minimumAvgValue(unsigned int & index, long & value, unsigned int first=0, unsigned int last= 0);
  double maximumEntries(unsigned int first=0, unsigned int last= 0);
  double minimumEntries(unsigned int first=0, unsigned int last= 0);
  double maximumValue(unsigned int first=0, unsigned int last= 0);
  double minimumValue(unsigned int first=0, unsigned int last= 0);
  double maximumAvgValue(unsigned int first=0, unsigned int last= 0);
  double minimumAvgValue(unsigned int first=0, unsigned int last= 0);
  unsigned int  binWithMaxEntries(unsigned int first=0, unsigned int last= 0);
  unsigned int  binWithMinEntries(unsigned int first=0, unsigned int last= 0);
  unsigned int  binWithMaxValue(unsigned int first=0, unsigned int last= 0);
  unsigned int  binWithMinValue(unsigned int first=0, unsigned int last= 0);
  unsigned int  binWithMaxAvgValue(unsigned int first=0, unsigned int last= 0);
  unsigned int  binWithMinAvgValue(unsigned int first=0, unsigned int last= 0);



  void clear();
  void reset();

  void fill(double x0, double weight=1.0);
  void fill(double x0, double x1, double weight);
  void fill(double x0, double x1, double x2, double weight);
  void fill(std::vector<double> & x, double weight);

  double binEntriesAt(unsigned int binIndex) const;
  double binEntriesAt(unsigned int binIndex0, unsigned int binIndex1) const;
  double binEntriesAt(unsigned int binIndex0, unsigned int binIndex1, unsigned int binIndex2) const;
  double binEntriesAt(const std::vector<unsigned int> & indices) const;
  double binValueAt(unsigned int binIndex) const;
  double binValueAt(unsigned int binIndex0, unsigned int binIndex1) const;
  double binValueAt(unsigned int binIndex0, unsigned int binIndex1, unsigned int binIndex2) const;
  double binValueAt(const std::vector<unsigned int> & indices) const;
  double binUncertaintyAt(unsigned int binIndex) const;
  double binUncertaintyAt(unsigned int binIndex0, unsigned int binIndex1) const;
  double binUncertaintyAt(unsigned int binIndex0, unsigned int binIndex1, unsigned int binIndex2) const;
  double binUncertaintyAt(const std::vector<unsigned int> & indices) const;
  double binAvgValueAt(unsigned int binIndex) const;
  double binAvgValueAt(unsigned int binIndex0, unsigned int binIndex1) const;
  double binAvgValueAt(unsigned int binIndex0, unsigned int binIndex1, unsigned int binIndex2) const;
  double binAvgValueAt(const std::vector<unsigned int> & indices) const;
  double binAvgValueUncertaintyAt(unsigned int binIndex) const;
  double binAvgValueUncertaintyAt(unsigned int binIndex0, unsigned int binIndex1) const;
  double binAvgValueUncertaintyAt(unsigned int binIndex0, unsigned int binIndex1, unsigned int binIndex2) const;
  double binAvgValueUncertaintyAt(const std::vector<unsigned int> & indices) const;




  const std::vector<long> & binEntries() const { return _entries; }
  const std::vector<float> & binAvgValues() const { return _values; }
  const std::vector<float> & AvgValueUncertainties() const { return _uncertainties; }
  std::vector<long>  & binEntries() { return _entries; }
  std::vector<float> & binAvgValues() { return _values; }
  std::vector<float> & binAvgValueUncertainties() { return _uncertainties; }
  std::vector<double> & binValues();
  std::vector<double> & binValueUncertainties();

  DataPoints   dataPoints(bool average=0, bool excludeEmptyBins=1) const;
  DataVectors  dataVectors(bool average=0, bool excludeEmptyBins=1) const;


  virtual void print() const;

  friend Histogram operator+(const Histogram & left, const Histogram & right);
  friend Histogram operator-(const Histogram & left, const Histogram & right);
  friend Histogram operator*(const Histogram & left, const Histogram & right);
  friend Histogram operator/(const Histogram & left, const Histogram & right);

  friend Histogram operator+(const Histogram & left, double right);
  friend Histogram operator+(double left, const Histogram & right);
  friend Histogram operator-(const Histogram & left, double right);
  friend Histogram operator-(double left, const Histogram & right);
  friend Histogram operator*(const Histogram & left, double right);
  friend Histogram operator*(double left, const Histogram & right);
  friend Histogram operator/(const Histogram & left, double right);
  friend Histogram operator/(double left, const Histogram & right);

  friend std::ostream& operator<<(std::ostream& out, const Histogram & v);




  ClassDef(Histogram,1)

  };


  Histogram operator+(const Histogram & left, const Histogram & right);
  Histogram operator-(const Histogram & left, const Histogram & right);
  Histogram operator*(const Histogram & left, const Histogram & right);
  Histogram operator/(const Histogram & left, const Histogram & right);

  Histogram operator+(const Histogram & left, double right);
  Histogram operator+(double left, const Histogram & right);
  Histogram operator-(const Histogram & left, double right);
  Histogram operator-(double left, const Histogram & right);
  Histogram operator*(const Histogram & left, double right);
  Histogram operator*(double left, const Histogram & right);
  Histogram operator/(const Histogram & left, double right);
  Histogram operator/(double left, const Histogram & right);

  Histogram operator+(const Histogram & right);
  Histogram operator-(const Histogram & right);

  std::ostream& operator<<(std::ostream& out, const Histogram & v);

  } // Math
} // namespace CAP

#endif
