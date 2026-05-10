#include "GlauberExceptions.hpp"
#include "NuclearDistribution.hpp"
#include "PrintHelpers.hpp"
using namespace std;
using CAP::MATH::pi;
using CAP::MATH::twoPi;



namespace CAP
{
  int NuclearDistribution::_instances = 0;

  NuclearDistribution::NuclearDistribution()
  :
  _type(0),
  _radius(0),
  _skin(0),
  _w(0),
  _a(0),
  _b(0),
  _beta2(0),
  _beta4(0),
  _r0(0),
  _r1(0),
  _r2(0),
  _maxRadius(0),
  _radiusGen(nullptr)
  { }

  NuclearDistribution::NuclearDistribution(const NuclearDistribution & src)
  :
  _type(src._type),
  _radius(src._radius),
  _skin(src._skin),
  _w(src._w),
  _a(src._a),
  _b(src._b),
  _beta2(src._beta2),
  _beta4(src._beta4),
  _r0(src._r0),
  _r1(src._r1),
  _r2(src._r2),
  _maxRadius(src._maxRadius),
  _radiusGen(src._radiusGen)
  { }


  NuclearDistribution & NuclearDistribution::operator=(const NuclearDistribution & rhs)
  {
  if (this!=&rhs)
    {
    _type   = rhs._type;
    _radius = rhs._radius;
    _skin   = rhs._skin;
    _w      = rhs._w;
    _a      = rhs._a;
    _b      = rhs._b;
    _beta2  = rhs._beta2;
    _beta4  = rhs._beta4;
    _r0     = rhs._r0;
    _r1     = rhs._r1;
    _r2     = rhs._r2;
    _maxRadius = rhs._maxRadius;
    _radiusGen = rhs._radiusGen;
    }
  return *this;
  }


  double NuclearDistribution::evaluate(double x, double theta __attribute__((unused)) )
  {
  double result = -1.0;
  double t0, t1, t2, t3, t4, t5, t6, tm, xx;
  double tp __attribute__((unused));
  switch (_type)
    {
      case 0: // exponential profile
      {
      result = x*x*exp(-x/_radius);
      break;
      }

      case 1: // 3pF
      {
      //f1 = new TF1(name,"x*x* (1+[2]*(x/[0])**2) / (1+exp((x-[0])/[1]) )",0.0,maxRadius);
      //f1->SetParameters(radius,skin,w);
      t1 = x/_radius;
      t2 = (x-_radius)/_skin;
      result = x*x*(1+_w*t1*t1) / (1+std::exp(t2));
      break;
      }

      case 2: // 3pG
      {
      //      f1 = new TF1(name,"x*x*(1+[2]*(x/[0])**2)/(1+exp((x**2-[0]**2)/[1]**2))",0.0,maxRadius);
      //      f1->SetParameters(radius,skin,w);
      xx = x*x;
      t1 = x/_radius;
      t2 = (x - _radius)/_skin;
      t3 = (xx - _radius*_radius)/_skin/_skin;
      result = x*x*(1+_w*t1*t1) / (1+std::exp(t3));
      break;
      }

      case 3: // useHulthen (see nucl-ex/0603010)
      case 4: // same but constrain the neutron opposite to the proton event-by-event
      {

      //      f1 = new TF1(name,"x*x*([0]*[1]*([0]+[1])) /( 2*pi* (pow([0]-[1],2)) )* pow( (exp(-[0]*x)-exp(-[1]*x) )/x,2)",0.0,maxRadius);
      //      f1->SetParameters(radius,a,b);
      t0 = 0; // fix me...
      tp = _radius + _a;
      tm = _radius - _a;
      t1 = _radius*_a*t0;
      t2 = 2*pi()*tm*tm;
      t3 = std::exp(-_radius*x);
      t4 = std::exp(-_skin*x);
      t5 = (t3-t4)/x;
      t6 = t5*t5;
      result = x*x*t1 /t2*t6;
      break;
      }

      case 5: // Ellipsoid (e.g., Uranium)
      {
      //      f1 = new TF1(name,"x*x*(1+[2]*(x/[0])**2)/(1+exp((x-[0])/[1]))",0.0,maxRadius);
      //      f1->SetParameters(radius,skin,0.0); // same as 3pF but setting W to zero
      t1 = x / _radius;
      t2 = (x - _radius)/_skin;
      result = x*x/(1+std::exp(t2));
      break;
      }

      case 6: // He3/H3 -- not implemented
      break;

      case 7: // Deformed nuclei, box method -- not implemented
      break;

      case 8: // Deformed nuclei, TF2 method
      {
      t2= 0;
      //      f2 = new TF2(name,"x*x*TMath::Sin(y)/(1+exp((x-[0]*(1+[2]*0.315*(3*pow(std::cos(y),2)-1.0)+[3]*0.105*(35*pow(std::cos(y),4)-30*pow(std::cos(y),2)+3)))/[1]))",0.0,maxRadius, 0.0,CAP::MATH::pi());
      //      f2->SetParameters(radius,skin,beta2,beta4);
      result = x*x/(1+std::exp(t2));
      break;
      }

      case 9: // Proton gaus
      {
      //      f1  = new TF1(name,"x*x*exp(-0.5*(x/[0])*(x/[0]))",0.0,maxRadius);
      //      f1->SetParameter(0,skin);
      t1 = x / _skin;
      result = x*x*std::exp(-0.5*t1*t1);
      break;
      }

      case 10: // Proton dgaus
      {
      //      f1 = new TF1(name,"x*x*((1-[0])/[1]^3*exp(-x*x/[1]/[1])+[0]/(0.4*[1])^3*exp(-x*x/(0.4*[1])^2))",0.0,maxRadius);
      //      f1->SetParameter(0,0.5);
      //      f1->SetParameter(1,skin);
      xx = x*x;
      t1 = x/_skin;
      t2 = 0.4*_skin;
      t3 = t2*t2*t2;
      t4 = _skin*_skin*_skin; // fix me
      result = -10; //xx*((1-_skin)/t4*std::exp(-t1*t1) + 0.5/t3*std::exp(-xx/(t2*t2));
      break;
      }

      case 11:
      {
      // 3pF for proton
      //      f1 = new TF1(name,"x*x*(1+[2]*(x/[0])**2)/(1+exp((x-[0])/[1]))",0,maxRadius);
      //      f1->SetParameters(radius,skin,w);
      result =  -11.0; //xx*((1-[0])/[1]^3*std::exp(-t1*t1) + 0.5/t3*std::exp(-xx/(t2*t2));
      break;
      }

      case 12:
      {
      // reweighted
      //      f1 = new TF1(name,"x*x*(1+[2]*(x/[0])**2)/(1+std::exp((x-[0])/[1]))/([3]+[4]*x+[5]*x^2)",0,maxRadius);
      //      f1->SetParameters(radius,skin,w,r0,r1,r2);
      result =  -12.0;
      break;
      }

      case 13:
      {
      // Pb for proton and neutrons reweighted
      //      f1  = new TF1(name,"x*x*(1+[2]*(x/[0])**2)/(1+std::exp((x-[0])/[1]))/([3]+[4]*x+[5]*x^2)",0,maxRadius);
      //      f1->SetParameters(radius,skin,w,1.00866,-0.000461484,-0.000203571);
      result =  -13.0;
      break;
      }

      case 14: // Deformed nuclei, TF2 method, reweighted
      {
      //      f2 = new TF2(name,"x*x*TMath::Sin(y)/(1+std::exp((x-[0]*(1+[2]*0.315*(3*pow(std::cos(y),2)-1.0)+[3]*0.105*(35*pow(std::cos(y),4)-30*pow(std::cos(y),2)+3)))/[1]))/([4]+[5]*x+[6]*x^2)",0.0,maxRadius,0.0,CAP::MATH::pi());
      //      f2->SetParameters(radius,skin,beta2,beta4,r0,r1,r2);
      result =  -14.0;
      break;
      }

      case 15:
      {
      // harmonic oscillator model
      //      f1 = new TF1(name,"x^2*(1+[0]*(x^2/[1]^2))*std::exp(-x^2/[1]^2)",0.0,maxRadius);
      //      f1->SetParameters(radius,skin);
      result =  -15.0;
      break;
      }

      default:
      throw RadialTypeGlauberException(_type,"Radial type not found", __FUNCTION__);
    }
  return result;
  }

  double NuclearDistribution::random()
  {
  if (!_radiusGen) createHistogram();
  return _radiusGen->GetRandom();
  }

  void NuclearDistribution::reset()
  {
  _type = 0;
  _radius = 0;
  _skin = 0;
  _w = 0;
  _a = 0;
  _b = 0;
  _beta2 = 0;
  _beta4 = 0;
  _r0 = 0;
  _r1 = 0;
  _r2 = 0;
  delete _radiusGen;
  _radiusGen = nullptr;
  }

  void NuclearDistribution::createHistogram()
  {
  int nBins = 200*_maxRadius;
  String nucProf = "nucProf";
  nucProf += _instances;
  _instances++;
  if (_radiusGen) delete _radiusGen;
  _radiusGen = new TH1F(nucProf,nucProf,nBins,0.0,_maxRadius);
  double dr = _maxRadius/double(nBins);
  double r  = 0;
//  printValue("createHistogram() nBins",nBins);
//  printValue("createHistogram() nucProf",nucProf);
//  printValue("createHistogram() dr",dr);
//  printValue("createHistogram() r",r);

  for (int k=0; k<nBins; k++)
    {
    double v = evaluate(r);
    double ev = 0;
    _radiusGen->SetBinContent(k+1, v);
    _radiusGen->SetBinError(k+1, ev);
    r += dr;
    }
  //printValue("first ran",_radiusGen->GetRandom());
  }


}  // namespace CAP



//switch ()
//{
//  case 0: // exponential profile
//    distribution->setRadius(radius);
//    //f1 = new TF1(name,"x*x*exp(-x/[0])",0.0,maxRadius);
//    //f1->SetParameter(0,radius);
//    break;
//
//  case 1: // 3pF
//    f1 = new TF1(name,"x*x*(1+[2]*(x/[0])**2)/(1+exp((x-[0])/[1]))",0.0,maxRadius);
//    f1->SetParameters(radius,skin,w);
//    break;
//
//  case 2: // 3pG
//    f1 = new TF1(name,"x*x*(1+[2]*(x/[0])**2)/(1+exp((x**2-[0]**2)/[1]**2))",0.0,maxRadius);
//    f1->SetParameters(radius,skin,w);
//    break;
//
//  case 3: // useHulthen (see nucl-ex/0603010)
//  case 4: // same but constrain the neutron opposite to the proton event-by-event
//    f1 = new TF1(name,"x*x*([0]*[1]*([0]+[1]))/(2*pi*(pow([0]-[1],2)))*pow((exp(-[0]*x)-exp(-[1]*x))/x,2)",0.0,maxRadius);
//    f1->SetParameters(radius,a,b);
//    break;
//
//  case 5: // Ellipsoid (e.g., Uranium)
//    f1 = new TF1(name,"x*x*(1+[2]*(x/[0])**2)/(1+exp((x-[0])/[1]))",0.0,maxRadius);
//    f1->SetParameters(radius,skin,0.0); // same as 3pF but setting W to zero
//    break;
//
//  case 6: // He3/H3 -- not implemented
//    break;
//
//  case 7: // Deformed nuclei, box method -- not implemented
//    break;
//
//  case 8: // Deformed nuclei, TF2 method
//    f2 = new TF2(name,"x*x*TMath::Sin(y)/(1+exp((x-[0]*(1+[2]*0.315*(3*pow(std::cos(y),2)-1.0)+[3]*0.105*(35*pow(std::cos(y),4)-30*pow(std::cos(y),2)+3)))/[1]))",0.0,maxRadius, 0.0,CAP::MATH::pi());
//    f2->SetParameters(radius,skin,beta2,beta4);
//    break;
//
//  case 9: // Proton gaus
//    f1  = new TF1(name,"x*x*exp(-0.5*(x/[0])*(x/[0]))",0.0,maxRadius);
//    f1->SetParameter(0,skin);
//    break;
//
//  case 10: // Proton dgaus
//    f1 = new TF1(name,"x*x*((1-[0])/[1]^3*exp(-x*x/[1]/[1])+[0]/(0.4*[1])^3*exp(-x*x/(0.4*[1])^2))",0.0,maxRadius);
//    f1->SetParameter(0,0.5);
//    f1->SetParameter(1,skin);
//    break;
//
//  case 11: // 3pF for proton
//    f1 = new TF1(name,"x*x*(1+[2]*(x/[0])**2)/(1+exp((x-[0])/[1]))",0,maxRadius);
//    f1->SetParameters(radius,skin,w);
//    break;
//
//  case 12: // reweighted
//    f1 = new TF1(name,"x*x*(1+[2]*(x/[0])**2)/(1+exp((x-[0])/[1]))/([3]+[4]*x+[5]*x^2)",0,maxRadius);
//    f1->SetParameters(radius,skin,w,r0,r1,r2);
//    break;
//
//  case 13: // Pb for proton and neutrons reweighted
//    f1  = new TF1(name,"x*x*(1+[2]*(x/[0])**2)/(1+exp((x-[0])/[1]))/([3]+[4]*x+[5]*x^2)",0,maxRadius);
//    f1->SetParameters(radius,skin,w,1.00866,-0.000461484,-0.000203571);
//    break;
//
//  case 14: // Deformed nuclei, TF2 method, reweighted
//    f2 = new TF2(name,"x*x*TMath::Sin(y)/(1+exp((x-[0]*(1+[2]*0.315*(3*pow(std::cos(y),2)-1.0)+[3]*0.105*(35*pow(std::cos(y),4)-30*pow(std::cos(y),2)+3)))/[1]))/([4]+[5]*x+[6]*x^2)",0.0,maxRadius,0.0,CAP::MATH::pi());
//    f2->SetParameters(radius,skin,beta2,beta4,r0,r1,r2);
//    break;
//
//  case 15: // harmonic oscillator model
//    f1 = new TF1(name,"x^2*(1+[0]*(x^2/[1]^2))*exp(-x^2/[1]^2)",0.0,maxRadius);
//    f1->SetParameters(radius,skin);
//    break;
//
//  default:
//    throw RadialTypeGlauberException(radialDistType,"Radial type not found", __FUNCTION__);
//}
//}
