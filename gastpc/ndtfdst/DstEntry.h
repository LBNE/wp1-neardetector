// -------------------------------------------------------------------
/// \file   DstEntry.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 5 Dec 2016
// -------------------------------------------------------------------

#ifndef DST_ENTRY_H
#define DST_ENTRY_H

namespace genie { class NtpMCEventRecord;}


/// TODO. Class description

class DstEntry
{
public:
  genie::NtpMCEventRecord* gmcrec; ///< Pointer to Genie's event record

  int RunID;
  int EventID;
  int Sample;

  double Ev;      ///< Neutrino energy
  double Ev_reco; ///< Reconstructed neutrino energy
  double Y;       ///< Inelasticity
  double Y_reco;  ///< Reconstructed inelasticity

  int NTracks;                    ///< Number of tracks in neutrino interaction
  int TrackID[500];               ///< MC track ID number
  int FamilyTreeLevel[500];       ///< Primary: 1; Secondary: 2 ...
  int Pdg[500];                   ///< PDG code of each track
  int Pdg_reco[500];              ///< Reconstructed PDG code
  double InitialMomentum[500][3]; ///< Initial momentum
  double Momentum_reco[500];      ///< Reconstructed momentum
  double VertexPosition[4];      ///< Initial vertex (position and time)

public:
  DstEntry();
  ~DstEntry();
};

inline DstEntry::DstEntry(): gmcrec(0) {}
inline DstEntry::~DstEntry() {}

#endif