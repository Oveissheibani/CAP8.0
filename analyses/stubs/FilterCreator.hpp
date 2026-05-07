/* Stub of CAP::FilterCreator — combined Event+Particle+Jet filter
   creator used by some shipped .inis (RunBf-style configs). */
#ifndef CAP_USER__FilterCreator
#define CAP_USER__FilterCreator
#include "EventProcessor.hpp"
namespace CAP {
class FilterCreator : public EventProcessor {
public:
  FilterCreator();
  FilterCreator(const FilterCreator & source);
  FilterCreator & operator=(const FilterCreator & rhs);
  virtual ~FilterCreator() {}
  virtual void execute();    // override EventProcessor's abstract execute()
  ClassDef(FilterCreator, 0)
};
}
#endif
