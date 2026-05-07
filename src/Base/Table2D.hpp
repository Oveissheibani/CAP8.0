#ifndef TableParser_h
#define TableParser_h
#include <string>
#include <vector>
#include "FileParser.hpp"
using namespace std;

namespace CAP
{

class TableParser : public FileParser
{
public:

  TableParser();
  TableParser(const TableParser & parser);
  virtual ~TableParser() {}
  TableParser & operator=(const TableParser & parser);


  virtual int  loadFrom(const String & _inputPath,
                    const String & _inputFileName,
                    const String & _extension = ".dat");
  virtual int  saveTo(const String & _outputPath,
                     const String & _outputFileName,
                     const String & _extension = ".dat",
                     const String & _option="RECREATE");

  vector<double> stringToDoubles(string str);

  vector< vector<double> >  readBlockData(istream & inputStream)

  ClassDef(TableParser,0)
};

} // namespace CAP
#endif

