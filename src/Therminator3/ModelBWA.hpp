#ifndef CAP__ModelBWA_H_
#define CAP__ModelBWA_H_
#include "Model.hpp"

namespace CAP
{
  class ModelBWA
  :
  public Model
  {
  public:
  ModelBWA();
  virtual ~ModelBWA() {}
  virtual double calculateIntegrand(ThermalHadron & hadron);
  ClassDef(ModelBWA,0)
  };

} // namespace CAP

#endif // !CAP__ModelBWA_H_

/*! @file ModelBWA.h
 * @brief Definition of ModelBWA class. Modified versions of Blast-Wave Model.
 */
/*! @class ModelBWA
 * @brief Modified Blast-Wave freeze-out models. Parameter <em>ModelBWA::_amp</em> tilts the hypersurface in the @f$ \_rho - \_tau @f$ plane.
 *
 * <table>
 *   <tr><th>Therminator3::therminator3()->sModel</th>	<th>Model::mModelName</th>	<th>Description</th></tr>
 *   <tr><td>2</td>		<td>"BWAVT"</td>		<td>Blast-Wave A-_type</td></tr>
 *   <tr><td>3</td>		<td>"BWAVTDelay"</td>		<td>Blast-Wave A-_type with delayed emission time</td></tr>
 *   <tr><td>4</td>		<td>"BWAVLinear"</td>		<td>Blast-Wave A-_type with linearized transverse flow</td></tr>
 *   <tr><td>5</td>		<td>"BWAVLinearDelay"</td>	<td>Blast-Wave A-_type with linearized transverse flow and delayed emission time</td></tr>
 *   <tr><td>6</td>		<td>"BWAVLinearForamption"</td>	<td>Blast-Wave A-_type with linearized transverse flow, delayed emission time and space foramption</td></tr>
 * </table>
 * <ul>
 *   <li> linearized transverse flow: @f$ v^{lin}_{T}(\_rho) = \frac{\_rho / \rho_{ampx}}{v_T + \_rho / \rho_{ampx}} @f$
 *   <li> delayed emission time: @f$ t(X) = t_0 + \_lambda * \std::log(X) @f$
 *        where random variable @f$ X \in (0,1) @f$ and @f$ t_0 @f$ is the particle time on the hypersurface </li>
 *   <li> space foramption: @f$ x_{i}(t(X)) = x^{0}_{i} + \frac{p_{i}}{E} \,t @f$
 *        where @f$ x^{0}_{i} @f$ - particle _position on the hypersurface,
 *        @f$ p_{i} @f$ - particle _momentum and @f$ E @f$ - _energy. Index @f$ i @f$ takes values x,y or z.</li>
 * </ul>
 *
 * The _integrand has the form
 * @f{eqnarray}{
 *   I			&=& \frac{2 s+1}{\left(2\pi\right)^3} \,d\Sigamp_\mu p^\mu \left[ \exp \left( \frac{p_\mu u^\mu - \mu_{ch}}{T}\right) \pm 1\right]^{-1} p_T \,dp_T \,d\phi_p \,dy \nonumber \\
 *   \nonumber\\ \nonumber\\
 *   d\Sigamp_\mu p^\mu	&=& \left[ m_T \std::cosh(\eta_\parallel - y) - A  \,p_T \std::cos(\phi - \phi_p) \right] \left( \_tau  + A \,\_rho \right) \,\_rho \,d\_rho \,d\phi \,d\eta_\parallel \nonumber \nonumber \\
 *   p_\mu u^\mu	&=& \frac{1}{\std::sqrt{1-v_T^2}} \left[ m_T \std::cosh(\eta_\parallel - y) - v_T \,p_T \std::cos(\phi - \phi_p)\right] \nonumber
 * @f}
 * where (link to all variable tables)
 *
 * Standard Blast-Wave model (ModelBlastWave) is recreated for <em>ModelBWA::_amp = 0</em>.<br />
 * Parameter ModelBWA::mDelay in some model versions delays the particle emittion according to
 * the exponetial distribution.<br />
 *
 * @fn ModelBWA::ModelBWA()
 * @brief Default constructor.
 *
 * @fn ModelBWA::ModelBWA(TRandom2* aRandom)
 * @brief Constructor that links a random number generator form Integrator.
 * @param [in] aRandom pointer to <a href="http://root.cern.ch/root/html/TRandom2.html">TRandom2</a> random number generator
 *
 * @fn ModelBWA::~ModelBWA()
 * @brief Destructor.
 *
 * @fn double ModelBWA::integrand(ParticleType& particleType)
 * @brief Generates random space-time coordinates and four-_momentum of a particle of a given _type
 * and returns the value of the _integrand of the Cooper-Frye formula.
 * @param [in] particleType pointer to the ParticleType class holding particle properties.
 *
 * @fn void ModelBWA::addPropertyBranch(TTree* aTree)
 * @brief Function called by EventGenerator to add a new <a href="http://root.cern.ch/root/html/TBranch.html">TBranch</a>
 * structure to an opened event <a href="http://root.cern.ch/root/html/TFile.html">TFile</a>.
 *
 * This branch contains model parameters and model description.
 * @param [in] aTree pointer to the current <a href="http://root.cern.ch/root/html/TTree.html">TTree</a> object.
 *
 * @var double ModelBWA::_pRapidityRange
 * @brief @f$ Y^{rng} @f$, rapidity range
 *
 * @var double ModelBWA::_sRapidityRange
 * @brief @f$ \eta_\parallel^{rng} @f$, spatial rapidity range
 *
 * @var double ModelBWA::_rhoMax
 * @brief @f$ \rho_{ampx} \,[GeV^{-1}] @f$, size of the fire-cylinder
 *
 * @var double ModelBWA::_tau
 * @brief @f$ \_tau  \,[GeV^{-1}] @f$, proper time
 *
 * @var double ModelBWA::_vT
 * @brief @f$ v_T \,[c] @f$, transverse velocity
 *
 * @var double ModelBWA::_amp;
 * @brief @f$ A @f$, the A-parameter
 *
 * @var double ModelBWA::mDelay;
 * @brief @f$ \_lambda \,[GeV^{-1}]@f$, particle time emission _delay
 *
 * @var Thermodynamics* ModelBWA::thermodynamics
 * @brief Pointer to the Thermodynamics class
 */
