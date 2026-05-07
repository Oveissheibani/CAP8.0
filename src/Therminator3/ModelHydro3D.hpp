#ifndef CAP__ModelHydro3D_H_
#define CAP__ModelHydro3D_H_
#include "Model.hpp"

namespace CAP
{

  class ModelHydro3D
  :
  public Model
  {
  public:
  ModelHydro3D();
  virtual ~ModelHydro3D() {}
  virtual double calculateIntegrand(ThermalHadron& hadron);
  virtual void readFromXmlFile(const char * inputPath,
                               const char * inputFileName);

  protected:

  // hypersurface distance and derivatives
  ScalarField _distanceFieldDTheta;
  // four-velocity on the hypersurface
  ScalarField uxFluidVelocityField;
  ScalarField uyFluidVelocityField;
  ScalarField rapidityFluidField;

  ClassDef(ModelHydro3D,0)
  };


} // namespace CAP

#endif

/*! @file ModelHydro3D.h
 * @brief Definition of ModelHydro3D class. Hydro based 3+1 freeze-out Model.
 */
/*! @class ModelHydro3D
 * @brief Lhyquid3D is a 3+1 freeze-out model that uses a hypersurface from hydrodynamic calculation as input.
 *
 * <table>
 *   <tr><th>Therminator3::therminator3()->sModel</th>	<th>Model::mModelName</th>	<th>Description</th></tr>
 *   <tr><td>10</td>		<td>"Lhyquid3D"</td>		<td>Hydro based 3+1 freeze-out model</td></tr>
 * </table>
 *
 * @fn ModelHydro3D::ModelHydro3D()
 * @brief Default constructor.
 *
 * @fn ModelHydro3D::ModelHydro3D(TRandom2* aRandom)
 * @brief Constructor that links a random number generator form Integrator.
 * @param [in] aRandom pointer to <a href="http://root.cern.ch/root/html/TRandom2.html">TRandom2</a> random number generator
 *
 * @fn ModelHydro3D::~ModelHydro3D()
 * @brief Destructor.
 *
 * @fn double ModelHydro3D::integrand(ParticleType& particleType)
 * @brief Generates random space-time coordinates and four-_momentum of a particle of a given _type
 * and returns the value of the _integrand of the Cooper-Frye formula.
 * @param [in] particleType pointer to the ParticleType class holding particle properties.
 *
 * @fn void ModelHydro3D::addPropertyBranch(TTree* aTree)
 * @brief Function called by EventGenerator to add a new <a href="http://root.cern.ch/root/html/TBranch.html">TBranch</a>
 * structure to an opened event <a href="http://root.cern.ch/root/html/TFile.html">TFile</a>.
 *
 * This branch contains model parameters and model description.
 * @param [in] aTree pointer to the current <a href="http://root.cern.ch/root/html/TTree.html">TTree</a> object.
 *
 */
