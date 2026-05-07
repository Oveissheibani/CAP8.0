#ifndef CAP_MathBasicIntegration
#define CAP_MathBasicIntegration
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <limits>
#include <cmath>
#include "MathConstants.hpp"
#include "MathBasicFunctions.hpp"
#include "MathExceptions.hpp"

using namespace std;

//!
//! Compendium of basic and essential mathematical  functions
//!  By convention, all function names  begin with a lower case letter.
//!
namespace CAP
{
namespace MATH
{

double integrationSimpsonRule(double* , int, double) ;
double integrationSimpsonRule(const vector<double> & , double) ;


} // namespace MATH

} // namespace CAP

#endif
