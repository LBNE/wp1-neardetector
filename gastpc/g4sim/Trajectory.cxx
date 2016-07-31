// -------------------------------------------------------------------
/// \file   Trajectory.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 30 July 2016
// -------------------------------------------------------------------

#include "Trajectory.h"

#include "TrajectoryPoint.h"
#include "TrajectoryMap.h"

#include <G4Track.hh>
#include <G4ParticleDefinition.hh>
#include <G4VProcess.hh>


G4ThreadLocal G4Allocator<Trajectory>* TrajectoryAllocator = 0;


Trajectory::Trajectory(const G4Track* track):
  G4VTrajectory()
{
  pdef_ = track->GetDefinition();
  track_id_ = track->GetTrackID();
  mother_id_ = track->GetParentID();

  if (mother_id_ == 0)
    creator_process_ = "none";
  else
    creator_process_ = track->GetCreatorProcess()->GetProcessName();

  _initial_momentum = track->GetMomentum();
  _initial_position = track->GetVertexPosition();
  _initial_time = track->GetGlobalTime();
  _initial_volume = track->GetLogicalVolumeAtVertex()->GetName();

  _trjpoints = new TrajectoryPointContainer();

  // Add this trajectory in the map, but only if no other
  // trajectory for this track id has been registered yet
  if (!TrajectoryMap::Get(track->GetTrackID()))
    TrajectoryMap::Add(this);

}


Trajectory::Trajectory(const Trajectory& other): G4VTrajectory()
{
  pdef_ = other.pdef_;
}


Trajectory::~Trajectory()
{
  for (unsigned int i=0; i<_trjpoints->size(); ++i) 
    delete (*_trjpoints)[i];
  _trjpoints->clear();
  delete _trjpoints;
}


G4String Trajectory::GetParticleName() const
{
  return pdef_->GetParticleName();
}


G4int Trajectory::GetPDGEncoding() const
{
  return pdef_->GetPDGEncoding();
}



G4double Trajectory::GetCharge() const
{
  return pdef_->GetPDGCharge();
}





void Trajectory::AppendStep(const G4Step* step)
{
  if (!_record_trjpoints) return;

  TrajectoryPoint* point = 
    new TrajectoryPoint(step->GetPostStepPoint()->GetPosition(),
                        step->GetPostStepPoint()->GetGlobalTime());
  _trjpoints->push_back(point);
}


void Trajectory::MergeTrajectory(G4VTrajectory* second)
{
  if (!second) return;

  if (!_record_trjpoints) return;

  Trajectory* tmp = (Trajectory*) second;
  G4int entries = tmp->GetPointEntries();

  // initial point of the second trajectory should not be merged
  for (G4int i=1; i<entries ; ++i) { 
    _trjpoints->push_back((*(tmp->_trjpoints))[i]);
  }

  delete (*tmp->_trjpoints)[0];
  tmp->_trjpoints->clear();
}


void Trajectory::ShowTrajectory(std::ostream& os) const
{
  // Invoke the default implementation
  G4VTrajectory::ShowTrajectory(os);
}


void Trajectory::DrawTrajectory() const
{
  // Invoke the default implementation
  G4VTrajectory::DrawTrajectory();
}