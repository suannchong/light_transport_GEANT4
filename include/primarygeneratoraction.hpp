#ifndef _PRIMARYGENERATORACTION_H_
#define _PRIMARYGENERATORACTION_H_
// MPND
// Geant4
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
// ROOT
// std
#include <vector>

namespace MPND {
  /*! \class PrimaryGeneratorAction 
   * Top level user action class responsible for generating source
   * particles. One PGA is generated per-thread and has its own copies of
   * the particle guns. We use the built-in General Particle Source (GPS),
   * which has commands in the /gps/ directory. */
  class PrimaryGeneratorAction: public G4VUserPrimaryGeneratorAction {
    public:
      /*! Constructor. */
      PrimaryGeneratorAction();
      /*! Destructor. */
      ~PrimaryGeneratorAction();
      
      /*! Main particle generation function - called by Geant4 at the
       * start of every G4Event. */
      void GeneratePrimaries(G4Event* a_event);

    private:
      /*! Pointer to the GPS. */
      G4GeneralParticleSource* m_gun;
  };
}

#endif
