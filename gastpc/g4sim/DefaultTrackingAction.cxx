// -------------------------------------------------------------------
/// \file   DefaultTrackingAction.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 30 July 2016
// -------------------------------------------------------------------

#include "DefaultTrackingAction.h"

#include "Trajectory.h"
#include "TrajectoryRegister.h"
#include "PersistencyManager.h"

#include <G4Track.hh>
#include <G4TrackingManager.hh>
#include <G4VPersistencyManager.hh>



void DefaultTrackingAction::PreUserTrackingAction(const G4Track* track)
{
  PersistencyManager* pm = dynamic_cast<PersistencyManager*>
    (G4VPersistencyManager::GetPersistencyManager());

  if (pm) {
    pm->StoreCurrentEvent(true);
    Trajectory* trj = new Trajectory(track);
    trj->RecordTrajectoryPoints(true);
    this->fpTrackingManager->SetStoreTrajectory(true);
    this->fpTrackingManager->SetTrajectory(trj);
  }
}


void DefaultTrackingAction::PostUserTrackingAction(const G4Track* track)
{
  Trajectory* trj = 
    dynamic_cast<Trajectory*>(TrajectoryRegister::Get(track->GetTrackID()));

  if (trj) {
    // Set some end-of-tracking properties of the trajectory
    trj->SetFinalPosition(track->GetPosition());
    trj->SetFinalTime(track->GetGlobalTime());
  }
}
