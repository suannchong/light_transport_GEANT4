#ifndef _HIT_H_
#define _HIT_H_
// MPND
#include "simhit.hpp"
// Geant4
#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4Types.hh"
#include "G4ThreeVector.hh"
// ROOT
// std

namespace MPND {
  /*! \class Hit
   * Hit carries information about the energy depositions in a
   * SensitiveDetector-associated volume. It has a specific form and set
   * of defined functions, as required by Geant4. For items with units,
   * the units are baked into the value (using CLHEP). */
  class Hit: public G4VHit {
    public:
      /*! Constructor. All parameters default to 0 or "". */
      Hit();
      /*! Constructor, providing all parameters. */
      Hit(int const& a_trackid, int const& a_parent_trackid,
          G4String const& a_volname, G4String const& a_pname,
          G4String const& a_cprocess, G4ThreeVector const& a_pos,
          double const& a_energy, double const& a_time);
      /*! Get the track ID. */
      int const& getTrackID() const;
      /*! Get the parent track ID. */
      int const& getParentTrackID() const;
      /*! Get the G4PVPlacement (physical) volume name. */
      G4String const& getVolumeName() const;
      /*! Get the name of the particle that did the hit. */
      G4String const& getParticleName() const;
      /*! Get the name of the process that created this particle, if any
       * (generator = ParticleGun). */
      G4String const& getCreatorProcess() const;
      /*! Get the position of the hit. */
      G4ThreeVector const& getPosition() const;
      /*! Get the energy deposited during the hit. */
      double const& getEnergy() const;
      /*! Get the global time of the hit. */
      double const& getTime() const;
      /*! Required by Geant4 - override the new operator. */
      inline void* operator new(std::size_t);
      /*! Required by Geant4 - override the delete operator. */
      inline void operator delete(void* a_hit);
      /*! Convert to SimHit for convenience. */
      SimHit toSimHit() const;

    private:
      /*! Particle track ID. */
      int m_trackID;
      /*! Parent's track ID, =0 if it's the primary particle. */
      int m_parentTrackID;
      /*! Physical volume name. */
      G4String m_volumeName;
      /*! Name of the particle. */
      G4String m_particleName;
      /*! Process that created this particle. */
      G4String m_creatorProcess;
      /*! Position of the hit. */
      G4ThreeVector m_position;
      /*! Energy deposited during the hit. */
      double m_energy;
      /*! Global time of the hit. */
      double m_time;
  };
  /*! Alias HitsCollection as G4THitsCollection<Hit>. */
  typedef G4THitsCollection<Hit> HitsCollection;
  /*! Thread-local pointer for the Hit allocator. */
  extern G4ThreadLocal G4Allocator<Hit>* HitAllocator;

  // The two functions below must be defined *after* HitAllocator. */
  inline void* Hit::operator new(std::size_t) {
    // If there's no allocator yet, make one, then return memory for
    // a new Hit
    if (!HitAllocator) {
      HitAllocator = new G4Allocator<Hit>;
    }
    return (void*)HitAllocator->MallocSingle();
  }
  inline void Hit::operator delete(void* a_hit) {
    HitAllocator->FreeSingle((Hit*)a_hit);
    return;
  }
}

#endif
