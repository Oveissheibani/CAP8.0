#ifndef CAP__ModelBlastWave
#define CAP__ModelBlastWave
#include "Model.hpp"

namespace CAP
{

  class ModelBlastWave
  :
  public Model
  {
  public:
  ModelBlastWave();
  virtual ~ModelBlastWave() {}
  virtual double calculateIntegrand(ThermalHadron & hadron);
  ClassDef(ModelBlastWave,0)
  };

} // namespace CAP


#endif // !CAP__ModelBlastWave
