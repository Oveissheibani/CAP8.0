#ifndef CAP__ModelHadronGas
#define CAP__ModelHadronGas
#include "Model.hpp"

namespace CAP
{
  class ModelHadronGas
  :
  public Model
  {
  public:
  ModelHadronGas();
  virtual ~ModelHadronGas() {}
  virtual double calculateIntegrand(ThermalHadron & hadron);
  ClassDef(ModelHadronGas,0)
  };

} // namespace CAP

#endif // !CAP__ModelthermalHadronGas



