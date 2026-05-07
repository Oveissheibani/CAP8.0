#ifndef CAP_GlauberNucleusType
#define CAP_GlauberNucleusType

#include "GlauberExceptions.hpp"

namespace CAP
{
using namespace std;

class GlauberNucleusType
{
public:
  friend class GlauberNucleus;
  GlauberNucleusType();
  GlauberNucleusType(const GlauberNucleusType & src);
  GlauberNucleusType operator=(const GlauberNucleusType & rhs);
  virtual ~GlauberNucleusType() { }
  virtual void print();

  String name() const           { return _name;      }
  int    type() const           { return _type;      }
  int    nProtons() const       { return _nProtons;  }
  int    nNeutrons() const      { return _nNeutrons; }
  int    nNucleons() const      { return _nProtons + _nNeutrons; }
  double radiusProton()const    { return _radiusProton; }
  double skinProton() const     { return _skinProton; }
  double wProton() const        { return _wProton;    }
  double radiusNeutron() const  { return _radiusNeutron; }
  double skinNeutron() const    { return _skinNeutron; }
  double wNeutron() const       { return _wNeutron;    }
  double beta2() const          { return _beta2; }
  double beta4() const          { return _beta4; }
  double maxRadius() const      { return _maxRadius; }
  int    radialDistType() const { return _radialDistType;}
  double a()    { return _a;   }
  double b()    { return _b;   }
  double r0()   { return _r0;  }
  double r1()   { return _r1;  }
  double r2()   { return _r2;  }

  void setName(const String name)  { _name       = name; }
  void setType(int type)           { _type       = type; }
  void setNProtons(int n)          { _nProtons   = n;  }
  void setNNeutrons(int n)         { _nNeutrons  = n; }
  void setUseHulten(bool v)        { _useHulthen = v; }
  void setRadiusProton(double v)   { _radiusProton  = v; }
  void setSkinProton(double v)     { _skinProton    = v; }
  void setWProton(double v)        { _wProton = v;};
  void setRadiusNeutron(double v)  { _radiusNeutron = v; }
  void setSkinNeutron(double v)    { _skinNeutron   = v; }
  void setWNeutron(double v)       { _wNeutron = v;};
  void setBeta2(double v)          { _beta2 = v; }
  void setBeta4(double v)          { _beta4 = v; }
  void setA(double v)            { _a = v;  }
  void setB(double v)            { _b = v;  }
  void setR0(double v)           { _r0 = v; }
  void setR1(double v)           { _r1 = v; }
  void setR2(double v)           { _r2 = v; }
  void setMaxRadius(double v)    { _maxRadius = v; }
  void setRadialDistType(int v)  { _radialDistType = v;}

protected:

  String _name;
  int    _type;
  int    _nProtons;
  int    _nNeutrons;
  bool   _useHulthen;
  double _radiusProton;
  double _skinProton;
  double _wProton;
  double _radiusNeutron;
  double _skinNeutron;
  double _wNeutron;
  double _beta2;
  double _beta4;
  double _a;
  double _b;
  double _r0;
  double _r1;
  double _r2;
  double _maxRadius;
  int    _radialDistType;

  ClassDef(GlauberNucleusType,1) // GlauberNucleus class
};

}  // namespace CAP

#endif // !CAP_GlauberNucleusType



