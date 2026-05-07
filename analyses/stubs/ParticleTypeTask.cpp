/* **********************************************************************
 *  Stub implementation of CAP::ParticleTypeTask — see header.
 * ******************************************************************** */
#include "ParticleTypeTask.hpp"
#include "ParticleDb.hpp"
#include "EnvironmentVariables.hpp"
#include "PrintHelpers.hpp"
#include <fstream>

ClassImp(CAP::ParticleTypeTask);

namespace CAP
{

ParticleTypeTask::ParticleTypeTask()
:
EventProcessor()
{
  appendClassName("ParticleTypeTask");
  setMinimumReportLevel(Object::kInfo);
  setName("ParticleTypeTask");
  setTitle("ParticleTypeTask");
}

ParticleTypeTask::ParticleTypeTask(const ParticleTypeTask & source)
:
EventProcessor(source)
{ }

ParticleTypeTask & ParticleTypeTask::operator=(const ParticleTypeTask & rhs)
{
  if (this != &rhs)
    {
    EventProcessor::operator=(rhs);
    }
  return *this;
}

void ParticleTypeTask::execute()
{
  // No per-call work — DB loading happens in initialize().
  _taskExecuted.increment();
}

void ParticleTypeTask::initialize()
{
  // Step 1: let EventProcessor instantiate the (empty) ParticleDb objects
  //         from the .ini's nParticleDbs / ParticleDbName0 keys.
  EventProcessor::initialize();

  // Step 2: populate the DB from disk. Every analyzer that borrows it
  //         (Owner=0) will see the fully-loaded catalog automatically
  //         because it lives in the static ManagedObjects store.
  EnvironmentVariables * env = EnvironmentVariables::environmentVariables();
  String base = env->variable("CAP_DATABASE_PATH");
  if (base.IsNull() || base.Length() == 0)
    base = "DB";

  String particlesFile = base; particlesFile += "/ParticleData/particles.data";
  String decaysFile    = base; decaysFile    += "/ParticleData/decays.data";

  std::ifstream f1(particlesFile.Data());
  std::ifstream f2(decaysFile.Data());
  if (!f1.is_open() || !f2.is_open())
    {
    printCR();
    printString("ParticleTypeTask::initialize() — could NOT open particle DB files:");
    printValue("particles", particlesFile);
    printValue("decays   ", decaysFile);
    return;
    }

  // Load into every owned ParticleDb.
  for (auto * pdb : _managedParticleDbs.getObjects())
    {
    if (!pdb) continue;
    f1.clear(); f1.seekg(0);
    f2.clear(); f2.seekg(0);
    pdb->loadFromAscii2(f1, f2);
    }

  printCR();
  printValue("ParticleTypeTask: loaded particles from", particlesFile);
}

} // namespace CAP
