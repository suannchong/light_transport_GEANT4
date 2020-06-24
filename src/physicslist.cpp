// MPND
#include "physicslist.hpp"
// Geant4
#include "G4EmStandardPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronPhysicsQGSP_BERT_HP.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "CLHEP/Units/SystemOfUnits.h"
using namespace CLHEP;
// ROOT
// std

namespace MPND {
  PhysicsList::PhysicsList(int const& a_verbose):
    G4VModularPhysicsList()
  {
    defaultCutValue = 0.7*mm;
    SetVerboseLevel(a_verbose);
    // Same as QGSP_BERT_HP constructor. Inheriting from it is messy, and
    // passing the individual G4VPhysicsConstructors into RegisterPhysics()
    // causes a crash since both objects will try to free them when they're
    // destructed. Original comments were kept.
    // EM Physics
    RegisterPhysics(new G4EmStandardPhysics(verboseLevel));
    // Synchroton Radiation & GN Physics
    RegisterPhysics(new G4EmExtraPhysics(verboseLevel));
    // Decays
    RegisterPhysics(new G4DecayPhysics(verboseLevel));
    RegisterPhysics(new G4RadioactiveDecayPhysics(verboseLevel));
    // Hadron Elastic scattering
    RegisterPhysics(new G4HadronElasticPhysicsHP(verboseLevel));
    // Hadron Physics
    RegisterPhysics(new G4HadronPhysicsQGSP_BERT_HP(verboseLevel));
    // Stopping Physics
    RegisterPhysics(new G4StoppingPhysics(verboseLevel));
    // Ion Physics
    RegisterPhysics(new G4IonPhysics(verboseLevel));

    // Optical physics - just use the stuff Geant4 provides
    auto opt_phys = new G4OpticalPhysics(verboseLevel);
    opt_phys->SetScintillationYieldFactor(1.);
    opt_phys->SetScintillationExcitationRatio(1.);
    opt_phys->SetTrackSecondariesFirst(kCerenkov, false);
    opt_phys->SetTrackSecondariesFirst(kScintillation, true);
    // opt_phys->SetFiniteRiseTime(true);
    RegisterPhysics(opt_phys);
  }

  PhysicsList::~PhysicsList() {
    // G4VModularPhysicsList will clean up the stuff we allocated
  }
}
