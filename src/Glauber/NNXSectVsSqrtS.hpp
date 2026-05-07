#ifndef CAP__NNXSectVsSqrtS
#define CAP__NNXSectVsSqrtS

namespace CAP
{
  class NNXSectVsSqrtS
  {
  public:

  NNXSectVsSqrtS();
  NNXSectVsSqrtS(const NNXSectVsSqrtS & src);
  NNXSectVsSqrtS & operator=(const NNXSectVsSqrtS & rhs);
  virtual~NNXSectVsSqrtS(){}
  virtual double evaluate(double x);
  virtual void setOption(unsigned int option);
  virtual void setA(double value) { _a = value; }
  virtual void setB(double value) { _b = value; }
  virtual void setC(double value) { _c = value; }
  virtual void setD(double value) { _d = value; }
  virtual void setN(double value) { _n = value; }
  virtual void setEta(double value) { _eta = value; }

  unsigned int option() { return _option; }
  unsigned int a() { return _a; }
  unsigned int b() { return _b; }
  unsigned int c() { return _c; }
  unsigned int d() { return _d; }
  unsigned int n() { return _n; }
  unsigned int eta() { return _eta; }
  virtual void reset();

  protected:

  unsigned int _option;
  double _a;
  double _b;
  double _c;
  double _d;
  double _n;
  double _eta;

  };

} // namespace CAP

#endif // !CAP__NNXSectVsSqrtS



