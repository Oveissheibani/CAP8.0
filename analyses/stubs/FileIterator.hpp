/* Stub of CAP::FileIterator — drives post-processing loops over input
   histogram files. Real implementation needs to walk a list of input
   ROOT files; this stub just registers the class name. */
#ifndef CAP_USER__FileIterator
#define CAP_USER__FileIterator
#include "Task.hpp"
namespace CAP {
class FileIterator : public Task {
public:
  FileIterator();
  FileIterator(const FileIterator & source);
  FileIterator & operator=(const FileIterator & rhs);
  virtual ~FileIterator() {}
  ClassDef(FileIterator, 0)
};
}
#endif
