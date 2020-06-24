// MPND
#include "actioninitialization.hpp"
#include "primarygeneratoraction.hpp"
#include "runaction.hpp"
#include "eventaction.hpp"
// Geant4
#include "G4ios.hh"
// ROOT
// std

namespace MPND {
  ActionInitialization::ActionInitialization():
    G4VUserActionInitialization()
  {
    G4cout << "Creating ActionInitialization..." << G4endl;
  }

  ActionInitialization::~ActionInitialization() {
    G4cout << "Deleting ActionInitialization..." << G4endl;
  }

  void ActionInitialization::Build() const {
    // Build 1 version of all actions
    SetUserAction(new MPND::PrimaryGeneratorAction);
    SetUserAction(new MPND::RunAction);
    SetUserAction(new MPND::EventAction);
    return;
  }

  void ActionInitialization::BuildForMaster() const {
    // Only build the global actions - per-thread actions are allocated
    // by the G4MTRunManager
    SetUserAction(new MPND::RunAction);
    return;
  }
}

