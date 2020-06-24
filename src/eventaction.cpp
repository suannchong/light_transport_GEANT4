// MPND
#include "eventaction.hpp"
#include "run.hpp"
// Geant4
#include "G4RunManager.hh"
#include "G4Threading.hh"
// ROOT
// std

namespace MPND {
  EventAction::EventAction():
    G4UserEventAction()
  {
    G4cout << "Creating EventAction in thread "
      << G4Threading::G4GetThreadId() << "..." << G4endl;
  }

  EventAction::~EventAction() {
    G4cout << "Deleting EventAction in thread "
      << G4Threading::G4GetThreadId() << "..." << G4endl;
  }

  void EventAction::BeginOfEventAction(G4Event const*) {
    return;
  }

  void EventAction::EndOfEventAction(G4Event const* a_event) {
    auto run_man = G4RunManager::GetRunManager();
    auto run = dynamic_cast<Run*>(run_man->GetNonConstCurrentRun());
    auto ntotal = run->GetNumberOfEventToBeProcessed();
    auto ievent = a_event->GetEventID();
    // Report status every 5% or so
    int rep_freq = (int)(ntotal / 20);
    if (rep_freq == 0) {
      rep_freq = 1;
    }
    if (ievent % rep_freq == 0) {
      G4cout << "Processing event " << ievent << " ("
        << 100.0 * (double)ievent / (double)ntotal << " %)" << G4endl;
    }
    return;
  }
}

