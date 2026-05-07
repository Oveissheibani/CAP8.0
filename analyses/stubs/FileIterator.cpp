#include "FileIterator.hpp"
ClassImp(CAP::FileIterator);
namespace CAP {
FileIterator::FileIterator() : Task() {
  appendClassName("FileIterator");
  setMinimumReportLevel(Object::kInfo);
  setName("FileIterator"); setTitle("FileIterator");
}
FileIterator::FileIterator(const FileIterator & s) : Task(s) {}
FileIterator & FileIterator::operator=(const FileIterator & r) {
  if (this != &r) Task::operator=(r); return *this;
}
}
