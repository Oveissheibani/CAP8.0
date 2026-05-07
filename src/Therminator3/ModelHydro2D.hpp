#ifndef CAP__ModelHydro2D
#define CAP__ModelHydro2D
#include "Model.hpp"

namespace CAP
{

  class ModelHydro2D
  :
  public Model
  {
  public:
  ModelHydro2D();
  virtual ~ModelHydro2D() {}
  virtual double calculateIntegrand(ThermalHadron & hadron);
  virtual void readFromXmlFile(const char * inputPath,
                               const char * inputFileName);

  protected:

  // velocity on the hypersurface
  ScalarField _vtFluidField;
  ScalarField _phiFluidField;

  ClassDef(ModelHydro2D,0)
  };

} // namespace CAP

#endif

/*! @file ModelHydro2D.h
 * @brief Definition of ModelHydro2D class. Hydro based 2+1 freeze-out Model with boost-invariance.
 */
/*! @class ModelHydro2D
 * @brief Lhyquid2DBI is a boost-invariant freeze-out model that uses a hypersurface from
 * hydrodynamic calculation as input.
 *
 * <table>
 *   <tr><th>Therminator3::therminator3()->sModel</th>	<th>Model::mModelName</th>	<th>Description</th></tr>
 *   <tr><td>11</td>		<td>"Lhyquid2DBI"</td>		<td>Hydro based freeze-out model with boost-invariance</td></tr>
 * </table>
 *
 * @fn ModelHydro2D::ModelHydro2D()
 * @brief Default constructor.
 *
 * @fn ModelHydro2D::ModelHydro2D(TRandom2* aRandom)
 * @brief Constructor that links a random number generator form Integrator.
 * @param [in] aRandom pointer to <a href="http://root.cern.ch/root/html/TRandom2.html">TRandom2</a> random number generator
 *
 * @fn ModelHydro2D::~ModelHydro2D()
 * @brief Destructor.
 *
 * @fn double ModelHydro2D::integrand(ParticleType& particleType)
 * @brief Generates random space-time coordinates and four-_momentum of a particle of a given _type
 * and returns the value of the _integrand of the Cooper-Frye formula.
 * @param [in] particleType pointer to the ParticleType class holding particle properties.
 *
 * @fn void ModelHydro2D::addPropertyBranch(TTree* aTree)
 * @brief Function called by EventGenerator to add a new <a href="http://root.cern.ch/root/html/TBranch.html">TBranch</a>
 * structure to an opened event <a href="http://root.cern.ch/root/html/TFile.html">TFile</a>.
 *
 * This branch contains model parameters and model description.
 * @param [in] aTree pointer to the current <a href="http://root.cern.ch/root/html/TTree.html">TTree</a> object.
 *
 */


