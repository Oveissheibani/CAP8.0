#ifndef CAP__GlauberDb
#define CAP__GlauberDb
#include <ostream>
#include "Configuration.hpp"
#include "GlauberNucleus.hpp"
#include "GlauberNucleusType.hpp"
#include "ManagedObject.hpp"
#include "MathConstants.hpp"
#include "NNXSectVsSqrtS.hpp"
#include "NNProfile.hpp"
#include "NucleonProfile.hpp"
#include "PTotProfile.hpp"
#include "SmearingProfile.hpp"

using namespace std;
using CAP::MATH::pi;
using CAP::MATH::twoPi;

namespace CAP
{
  class GlauberDb
  :
  public ManagedObject
  {
  public:

  GlauberDb();
  GlauberDb(const GlauberDb & src);
  GlauberDb operator=(const GlauberDb & rhs);
  virtual ~GlauberDb() {}
  void findNucleus(const String & name, GlauberNucleusType & nucleus);

  virtual void configure(const String & taskName __attribute__((unused)),
                         const String & objectType __attribute__((unused)),
                         const Configuration & configuration __attribute__((unused)),
                         unsigned int index __attribute__((unused)) )
  { }


  virtual void loadFromAscii2(std::ifstream & file1 __attribute__((unused)),
                              std::ifstream & file2 __attribute__((unused))) {} // fix me/


  ClassDef(GlauberDb,0)
  };

} // namespace CAP

#endif



