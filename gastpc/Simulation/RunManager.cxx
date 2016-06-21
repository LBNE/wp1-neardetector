// -------------------------------------------------------------------
/// \file   RunManager.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 1 June 2016
// -------------------------------------------------------------------

#include "RunManager.h"

#include "DuneGArNDGeometry.h"
#include "BeamSpillGenerator.h"

#include <G4UImanager.hh>
#include <G4VModularPhysicsList.hh>
#include <QGSP_BERT.hh>
#include <G4StepLimiterPhysics.hh>



RunManager::RunManager(): G4RunManager()
{
  G4VModularPhysicsList* physlist = new QGSP_BERT();
  physlist->RegisterPhysics(new G4StepLimiterPhysics());
  this->SetUserInitialization(physlist);

  this->SetUserInitialization(new DuneGArNDGeometry);

  this->SetUserAction(new BeamSpillGenerator());
}


RunManager::~RunManager()
{}


void RunManager::Initialize()
{
  G4RunManager::Initialize();
}


void RunManager::ExecuteMacroFile(const G4String& filename)
{
  G4UImanager* UI = G4UImanager::GetUIpointer();
  UI->ExecuteMacroFile(filename);
}


void RunManager::SetRandomSeed(G4int seed) const
{
  if (seed < 0) CLHEP::HepRandom::setTheSeed(time(0));
  else CLHEP::HepRandom::setTheSeed(seed);
}