/* ----------------------------------------------------------------------
 * CAP::GlobalCalculator — thin subclass of GlobalAnalyzer.
 * Pruneau's untouched GlobalDerivedHistos.cpp does the math.
 * --------------------------------------------------------------------*/
#ifndef CAP_USER__GlobalCalculator
#define CAP_USER__GlobalCalculator
#include "GlobalAnalyzer.hpp"
namespace CAP {
class GlobalCalculator : public GlobalAnalyzer {
public:
  GlobalCalculator();
  GlobalCalculator(const GlobalCalculator & source);
  GlobalCalculator & operator=(const GlobalCalculator & rhs);
  virtual ~GlobalCalculator() {}
  virtual void execute();
  ClassDef(GlobalCalculator, 0)
};
}
#endif
