#include "ParticleDbTask.hpp"
#include "ParticleDb.hpp"
#include "EnvironmentVariables.hpp"
#include "PrintHelpers.hpp"
#include <fstream>
ClassImp(CAP::ParticleDbTask);
namespace CAP {
ParticleDbTask::ParticleDbTask() : EventProcessor() {
  appendClassName("ParticleDbTask");
  setMinimumReportLevel(Object::kInfo);
  setName("ParticleDbTask"); setTitle("ParticleDbTask");
}
ParticleDbTask::ParticleDbTask(const ParticleDbTask & s) : EventProcessor(s) {}
ParticleDbTask & ParticleDbTask::operator=(const ParticleDbTask & r) {
  if (this != &r) EventProcessor::operator=(r); return *this;
}
void ParticleDbTask::execute() { _taskExecuted.increment(); }

void ParticleDbTask::initialize() {
  EventProcessor::initialize();
  EnvironmentVariables * env = EnvironmentVariables::environmentVariables();
  String base = env->variable("CAP_DATABASE_PATH");
  if (base.IsNull() || base.Length() == 0) base = "DB";
  String particlesFile = base; particlesFile += "/ParticleData/particles.data";
  String decaysFile    = base; decaysFile    += "/ParticleData/decays.data";
  std::ifstream f1(particlesFile.Data()), f2(decaysFile.Data());
  if (!f1.is_open() || !f2.is_open()) {
    printCR();
    printString("ParticleDbTask::initialize() — could NOT open particle DB files");
    return;
  }
  for (auto * pdb : _managedParticleDbs.getObjects()) {
    if (!pdb) continue;
    f1.clear(); f1.seekg(0);
    f2.clear(); f2.seekg(0);
    pdb->loadFromAscii2(f1, f2);
  }
}
}
