#ifndef CAP__ModelKrakowSFO
#define CAP__ModelKrakowSFO
#include "Model.hpp"

namespace CAP
{
  class ModelKrakowSFO
  :
  public Model
  {
  public:
  ModelKrakowSFO();
  virtual ~ModelKrakowSFO() {}
  virtual double calculateIntegrand(ThermalHadron & hadron);
  ClassDef(ModelKrakowSFO,0)
  };

} // namespace CAP

#endif

/*! @file ModelKrakowSFO.h
 * @brief Definition of ModelKrakowSFO class. Krakow Single Freeze-out Model.
 */
/*! @class ModelKrakowSFO
 * @brief Krakow Single Freeze-out model.
 *
 * <table>
 *   <tr><th>Therminator3::therminator3()->sModel</th>	<th>Model::mModelName</th>	<th>Description</th></tr>
 *   <tr><td>0</td>		<td>"KrakowSFO"</td>		<td>Krakow Single Freeze-out model</td></tr>
 * </table>
 *
 * The _integrand has the form
 * @f{eqnarray}{
 *   I			&=& \frac{2 s+1}{\left(2\pi\right)^3} \,d\Sigamp_\mu p^\mu \left[ \exp \left( \frac{p_\mu u^\mu - \mu_{ch}}{T}\right) \pm 1\right]^{-1} p_T \,dp_T \,d\phi_p \,dy \nonumber \\
 *   \nonumber\\ \nonumber\\
 *   d\Sigamp_\mu p^\mu	&=& \left[ \std::sqrt{\_tau _C^2 + \_rho^2} \std::cosh(\eta_\parallel - y) - \_rho \,p_T \std::cos(\phi - \phi_p)\right] \,\_rho \,d\_rho \,d\phi \,d\eta_\parallel \nonumber \nonumber \\
 *   p_\mu u^\mu	&=& \frac{1}{\_tau _C} \left[ \std::sqrt{\_tau _C^2 + \_rho^2} \std::cosh(\eta_\parallel - y) - \_rho \,p_T \std::cos(\phi - \phi_p)\right] \nonumber
 * @f}
 * where (link to all variable tables)
 *
 * @fn ModelKrakowSFO::ModelKrakowSFO()
 * @brief Default constructor.
 *
 * @fn ModelKrakowSFO::ModelKrakowSFO(TRandom2* aRandom)
 * @brief Constructor that links a random number generator form Integrator.
 * @param [in] aRandom pointer to <a href="http://root.cern.ch/root/html/TRandom2.html">TRandom2</a> random number generator
 *
 * @fn ModelKrakowSFO::~ModelKrakowSFO()
 * @brief Destructor.
 *
 * @fn double ModelKrakowSFO::integrand(ParticleType& particleType)
 * @brief Generates random space-time coordinates and four-_momentum of a particle of a given _type
 * and returns the value of the _integrand of the Cooper-Frye formula.
 * @param [in] particleType pointer to the ParticleType class holding particle properties.
 *
 * @fn void ModelKrakowSFO::addPropertyBranch(TTree* aTree)
 * @brief Function called by EventGenerator to add a new <a href="http://root.cern.ch/root/html/TBranch.html">TBranch</a>
 * structure to an opened event <a href="http://root.cern.ch/root/html/TFile.html">TFile</a>.
 *
 * This branch contains model parameters and model description.
 * @param [in] aTree pointer to the current <a href="http://root.cern.ch/root/html/TTree.html">TTree</a> object.
 *
 * @var double ModelKrakowSFO::_pRapidityRange
 * @brief @f$ Y^{rng} @f$, rapidity range
 *
 * @var double ModelKrakowSFO::_sRapidityRange
 * @brief @f$ \eta_\parallel^{rng} @f$, spatial rapidity range
 *
 * @var double ModelKrakowSFO::_rhoMax
 * @brief @f$ \rho_{ampx} \,[GeV^{-1}] @f$, size of the fire-cylinder
 *
 * @var double ModelKrakowSFO::_tauC
 * @brief @f$ \_tau _C \std::sqrt{t^2 - x^2 - y^2 - z^2} \,[GeV^{-1}] @f$, Krakow-_type proper time
 *
 * The well known proper time can be evaluated as @f$ \_tau  = \std::sqrt{\_tau _C^2 + \_rho^2} @f$.
 *
 * @var Thermodynamics* ModelKrakowSFO::thermodynamics
 * @brief Pointer to the Thermodynamics class.
 */
