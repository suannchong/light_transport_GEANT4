// MPND
#include "hit.hpp"
// Geant4
#include "CLHEP/Units/SystemOfUnits.h"
using namespace CLHEP;
// ROOT
// std

namespace MPND {
  // Initialize the globally-visible thread-local pointer to the hit
  // allocator
  G4ThreadLocal G4Allocator<Hit>* HitAllocator = nullptr;

  Hit::Hit():
    m_trackID(0),
    m_parentTrackID(0),
    m_volumeName(""),
    m_particleName(""),
    m_creatorProcess(""),
    m_position(0.*m, 0.*m, 0.*m),
    m_energy(0.*eV),
    m_time(0.*s)
  {
  }

  Hit::Hit(int const& a_trackid, int const& a_parent_trackid,
           G4String const& a_volname, G4String const& a_pname,
           G4String const& a_cprocess, G4ThreeVector const& a_pos,
           double const& a_energy, double const& a_time):
    m_trackID(a_trackid),
    m_parentTrackID(a_parent_trackid),
    m_volumeName(a_volname),
    m_particleName(a_pname),
    m_creatorProcess(a_cprocess),
    m_position(a_pos),
    m_energy(a_energy),
    m_time(a_time)
  {
  }

  int const& Hit::getTrackID() const {
    return m_trackID;
  }

  int const& Hit::getParentTrackID() const {
    return m_parentTrackID;
  }

  G4String const& Hit::getVolumeName() const {
    return m_volumeName;
  }

  G4String const& Hit::getParticleName() const {
    return m_particleName;
  }

  G4String const& Hit::getCreatorProcess() const {
    return m_creatorProcess;
  }

  G4ThreeVector const& Hit::getPosition() const {
    return m_position;
  }

  double const& Hit::getEnergy() const {
    return m_energy;
  }

  double const& Hit::getTime() const {
    return m_time;
  }

  SimHit Hit::toSimHit() const {
    SimHit hit(TVector3(m_position.x()/cm,
                        m_position.y()/cm,
                        m_position.z()/cm),
               m_energy/eV,
               m_time/ns,
               TString(m_particleName.data()));
    return hit;
  }
}
