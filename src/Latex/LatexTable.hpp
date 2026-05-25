#ifndef CAP__LatexTable
#define CAP__LatexTable
#include <vector>
#include "LatexElement.hpp"

namespace CAP
{
  // ----------------------------------------------------------------------
  //  LatexTable — a LaTeX {table} float wrapping a {tabular}.
  //
  //  The one element the Latex module was missing (it had LatexList but
  //  no table).  Cells are stored row-major in a single flat vector so
  //  the class stays simple and ROOT-dictionary friendly.
  //
  //  Usage:
  //     LatexTable t;
  //     t.setColumnSpec("l r r");
  //     t.setCaption("Origin breakdown");
  //     t.setHeaderRows(1);
  //     t.addRow({"class", "count", "fraction"});   // header
  //     t.addRow({"Primary", "587869", "31.7\\%"});
  //  The first row added fixes the column count.
  // ----------------------------------------------------------------------
  class LatexTable : public LatexElement
  {
  public:

  LatexTable();
  LatexTable(const LatexTable & src);
  LatexTable & operator=(const LatexTable & rhs);
  virtual ~LatexTable()  { }

  void setColumnSpec(const String & spec) { _columnSpec = spec; }
  void setCaption(const String & caption) { _caption    = caption; }
  void setPlacement(const String & p)     { _placement  = p; }
  void setHeaderRows(int n)               { _headerRows = n; }

  // Append one row.  The first row added fixes the column count;
  // later rows are padded / truncated to that width.
  void addRow(const std::vector<String> & cells);

  const String & columnSpec() const { return _columnSpec; }
  const String & caption()    const { return _caption; }
  int  nColumns() const { return _nColumns; }
  int  nRows()    const { return _nColumns > 0
                                 ? int(_cells.size()) / _nColumns : 0; }

  virtual void writeHeader(std::ofstream & out);
  virtual void writeContent(std::ofstream & out);
  virtual void writeTrailer(std::ofstream & out);

  protected:

  String _columnSpec;
  String _caption;
  String _placement;
  int    _nColumns;
  int    _headerRows;
  std::vector<String> _cells;     // row-major

  ClassDef(LatexTable,0)
  };

} // namespace CAP

#endif // !CAP__LatexTable
