// MPND
#include "run.hpp"
#include "filemanager.hpp"
#include "sdmessenger.hpp"
// Geant4
#include "G4ios.hh"
#include "G4Threading.hh"
#include "G4SDManager.hh"
#include "G4Event.hh"
// ROOT
// std

namespace MPND {
  Run::Run():
    G4Run()
  {
    G4cout << "Creating Run in thread " << G4Threading::G4GetThreadId()
      << "..." << G4endl;
  }

  Run::~Run() {
    G4cout << "Deleting Run in thread " << G4Threading::G4GetThreadId()
      << "..." << G4endl;
  }

  void Run::RecordEvent(G4Event const* a_event) {
    // Loop over all of the SDs
    auto sd_man = G4SDManager::GetSDMpointer();
    auto sd_names = SDMessenger::get().getSDNames();
    auto& fman = FileManager::get();
    for (auto& sd : sd_names) {
      // Get the hit collection
      sd += "HC";
      auto hc_id = sd_man->GetCollectionID(sd);
      if (hc_id == -1) {
        G4cerr << "Could not get collection ID for " << sd << G4endl;
        return;
      }
      auto hc_ev = a_event->GetHCofThisEvent();
      if (!hc_ev) {
        G4cerr << "Could not get the hits collection for this event."
          << "hc_id = " << hc_id << G4endl;
        return;
      }
      auto hc = static_cast<G4THitsCollection<Hit>*>(hc_ev->GetHC(hc_id));
      if (!hc) {
        G4cerr << "Could not get the hits collection from the HCoTE; "
          << "hc_id = " << hc_id << G4endl;
        return;
      }

      // Loop over hits and send them to be written
      for (int ihit=0;ihit < hc->entries();++ihit) {
        MPND::Hit hit(*(*hc)[ihit]);
        fman.addHit(hit);
      }
    }
    G4Run::RecordEvent(a_event);
    return;
  }

  void Run::Merge(G4Run const* a_run) {
    G4Run::Merge(a_run);
    return;
  }
}

