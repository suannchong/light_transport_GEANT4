// MPND
#include "primarygeneratoraction.hpp"
// Geant4
#include "G4ios.hh"
#include "G4Threading.hh"
// ROOT
// std

namespace MPND {
  PrimaryGeneratorAction::PrimaryGeneratorAction():
    G4VUserPrimaryGeneratorAction(),
    m_gun(new G4GeneralParticleSource)
  {
    G4cout << "Creating PrimaryGeneratorAction in thread "
      << G4Threading::G4GetThreadId() << "..." << G4endl;
  }

  PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    G4cout << "Deleting PrimaryGeneratorAction in thread "
      << G4Threading::G4GetThreadId() << "..." << G4endl;
    delete m_gun;
  }

  void PrimaryGeneratorAction::GeneratePrimaries(G4Event* a_event) {
    m_gun->GeneratePrimaryVertex(a_event);
    return;
  }
}

