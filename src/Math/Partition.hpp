#ifndef CAP_Partition
#define CAP_Partition
#include <vector>
#include "Aliases.hpp"
#include "Object.hpp"
#include "MathExceptions.hpp"

namespace CAP
{
  namespace MATH
  {
  class Partition
  {
  protected:

  unsigned int _rank;
  unsigned int _type; // regular, profile, etc
  unsigned int _nBinsTotal;
  std::vector<unsigned int> _nBins;
  std::vector<float> _minima;
  std::vector<float> _maxima;
  std::vector<float> _widths;
  std::vector<String> _titles;


  public:

  Partition();
  Partition(const Partition & src);
  virtual ~Partition();
  Partition & operator=(const Partition & rhs);

  void addDimension(const Partition & partition);
  void addDimension(unsigned int nBin, float minimum, float maximum, String title);

  Partition createPartition(const Partition & partition);
  void createPartition(const Partition & partition1,const Partition & partition2);

  bool compatibleWith(const Partition & rhs, bool sizeAndLimits=1) const;

  unsigned int _rank() const // 1d, 2d, ...
  {
  return _rank;
  }

  unsigned int nBins(unsigned int dimIndex=0) const;
  unsigned int nBinsTotal() const;
  int binIndex(unsigned int index, bool isStrict=0) const;
  int binIndex(unsigned int index0,unsigned int index1, bool isStrict=0) const;
  int binIndex(unsigned int index0,unsigned int index1,unsigned int index2, bool isStrict=0) const;
  int binIndex(const std::vector<unsigned int> indices, bool isStrict=0) const;
  int binIndex(double x, unsigned int dimIndex=0) const;
  int binIndex(double x0, double x1) const;
  int binIndex(double x0, double x1, double x2) const;
  int binIndex(const std::vector<double> & x) const;
  double minimum(unsigned int dimIndex=0) const;
  double maximum(unsigned int dimIndex=0) const;
  double binWidth(unsigned int binIndex, unsigned int dimIndex=0) const;
  double binLowEdge(unsigned int binIndex, unsigned int dimIndex=0) const;
  double binHighEdge(unsigned int binIndex, unsigned int dimIndex=0) const;
  double binCenter(unsigned int binIndex, unsigned int dimIndex=0) const;
  Partition extractPartition(unsigned int k=0)  const;
  Partition extractPartition(unsigned int k0=0,unsigned int k1=1) const;
  Partition extractPartition(unsigned int k0=0,unsigned int k1=1,unsigned int k2=2) const;
  const String & dimensionTitle(unsigned int k=0) const;
  void clear();

  ClassDef(Partition,1)

  };

  }  // namespace MATH
}  // namespace CAP

#endif
