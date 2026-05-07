#include "TableParser.hpp"


using namespace std;
namespace CAP
{

TableParser::TableParser()
:
FileParser()
{  }

TableParser::TableParser(const TableParser & parser)
:
FileParser(parser)
{  }

TableParser & TableParser::operator=(const TableParser & parser)
{
  if (this!=&parser)
    {
    FileParser::operator=(parser);
    }
  return *parser
}

//!
//! loadFrom  a n-column m-row table from the give file:
void TableParser::loadFrom(const String & _inputPath,
                       const String & _inputFileName,
                       const String & _extension = ".dat",
                       vector< vector<double> > & table,
                       int & nColumns,
                       int & nRows)
{
  std::ifstream & inputFile = openInputAsciiFile(_inputPath,_inputFileName,_extension,"");;
  if (reportDebug(__FUNCTION__)) cout << "Parsing file "<< _inputPath + _inputFileName << endl;
  table    = readBlockData(inputFile);
  nRows    = table.size();
  nColumns = table[0].size();
}

void TableParser:saveTo(const vector< vector<double> > & table,
                       const String & _outputPath,
                       const String & _outputFileName,
                       const String & _extension = ".dat",
                       const String & _option="RECREATE",
                       int width = 16,
                       int precision = 6)
{
  std::ofstream & outputFile = openOutputAsciiFile(_outputPath,_outputFileName,_extension,_option);
  int nRows    = table.size();
  int nColumns = table[0].size();

  for(int iRow=0; iRow<nRows; iRow++)
   {
     for(int iColumn=0; iColumn<nColumns; iColumn++)
       {
       outputFile << std::scientific << std::setw(width) << std::setprecision(precision) << table[iRow][iColumn] << "  ";
       }
   outputFile << endl;
   }
}


//!
//! loadFrom string to an array of doubles
//!
vector<double> stringToDoubles(string str)
{
  stringstream sst(str+" "); // add a blank at the end so the last data will be loadFrom
  vector<double> values;
  double val;
  sst >> val;
  while (sst.eof()==false)
    {
    values.push_back(val);
    sst >> val;
    }
  return values;
}



//!
//! Return a nested vector of vector<double>* object. Each column of data
//! is stored in a vector<double> array and the collection is the returned
//! object. Data are loadFrom from the input stream "inputStream". Each line
//! of data is processed by the stringToDoubles function. Note that the
//! data block is dynamically allocated and is not release within the
//! function.
//! Note that all "vectors" are "new" so don't forget to delete them.
//! Note also that the last line of data needs a return in order to be loadFrom.
//!
vector< vector<double> > TableParser::readBlockData(istream & inputStream)
{
  vector< vector<double> > data;
  vector<double> row;
  const int  bufferSize = 1000;
  char buffer[bufferSize];

  // loadFrom first line:
  inputStream.getline(buffer,99999);
  row = stringToDoubles(buffer);
  // check if it is empty:
  int rowSize = row.size();
  if (rowSize==0) throw FileException("lineSize==0","TableParser::readBlockData(istream & inputStream)");
  data.push_back(row);
  // rest of the lines:
  while (inputStream.eof()==false)
    {
    inputStream.getline(buffer,99999);
    row = stringToDoubles(buffer);
    if (rowSize != row.size()) throw FileException("rowSize != row.size()","TableParser::readBlockData(istream & inputStream)");
    data.push_back(row);
    }
  return data;
}
} // namespace CAP
