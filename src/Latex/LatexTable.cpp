#include "LatexTable.hpp"

namespace CAP
{
  LatexTable::LatexTable()
  :
  LatexElement(),
  _columnSpec(),
  _caption(),
  _placement("H"),     // pin tables in place (needs \usepackage{float})
  _nColumns(0),
  _headerRows(1),
  _cells()
  {   }

  LatexTable::LatexTable(const LatexTable & src)
  :
  LatexElement(src),
  _columnSpec(src._columnSpec),
  _caption(src._caption),
  _placement(src._placement),
  _nColumns(src._nColumns),
  _headerRows(src._headerRows),
  _cells(src._cells)
  {   }

  LatexTable & LatexTable::operator=(const LatexTable & rhs)
  {
  if (this != &rhs)
    {
    LatexElement::operator=(rhs);
    _columnSpec = rhs._columnSpec;
    _caption    = rhs._caption;
    _placement  = rhs._placement;
    _nColumns   = rhs._nColumns;
    _headerRows = rhs._headerRows;
    _cells      = rhs._cells;
    }
  return *this;
  }

  void LatexTable::addRow(const std::vector<String> & cells)
  {
  if (_nColumns == 0) _nColumns = int(cells.size());
  for (int c = 0; c < _nColumns; c++)
    {
    if (c < int(cells.size())) _cells.push_back(cells[c]);
    else                       _cells.push_back(String(""));
    }
  }

  void LatexTable::writeHeader(std::ofstream & out)
  {
  skipLines(out,1);
  out << "\\begin{table}[" << (_placement.Length() > 0 ? _placement : String("htbp"))
      << "]" << endl;
  out << "\\centering" << endl;
  String spec = _columnSpec;
  if (spec.Length() < 1)
    for (int c = 0; c < _nColumns; c++) spec += "l";
  out << "\\begin{tabular}{" << spec << "}" << endl;
  out << "\\hline" << endl;
  }

  void LatexTable::writeContent(std::ofstream & out)
  {
  const int nrows = nRows();
  for (int r = 0; r < nrows; r++)
    {
    for (int c = 0; c < _nColumns; c++)
      {
      if (c > 0) out << " & ";
      out << _cells[r * _nColumns + c];
      }
    out << " \\\\" << endl;
    if (_headerRows > 0 && r == _headerRows - 1)
      out << "\\hline\\hline" << endl;
    }
  }

  void LatexTable::writeTrailer(std::ofstream & out)
  {
  out << "\\hline" << endl;
  out << "\\end{tabular}" << endl;
  if (_caption.Length() > 0) out << "\\caption{" << _caption << "}" << endl;
  if (_label.Length()   > 0) out << "\\label{"   << _label   << "}" << endl;
  out << "\\end{table}" << endl;
  skipLines(out,1);
  }

} // namespace CAP
