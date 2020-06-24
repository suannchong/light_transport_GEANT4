// MPND
#include "runaction.hpp"
#include "run.hpp"
#include "randommanager.hpp"
#include "filemanager.hpp"
// Geant4
#include "G4RunManager.hh"
#include "G4Threading.hh"
// ROOT
// std

namespace MPND {
  RunAction::RunAction():
    G4UserRunAction(),
    m_timer()
  {
    G4cout << "Creating RunAction in thread "
      << G4Threading::G4GetThreadId() << "..." << G4endl;
  }

  RunAction::~RunAction() {
    G4cout << "Deleting RunAction in thread "
      << G4Threading::G4GetThreadId() << "..." << G4endl;
  }

  G4Run* RunAction::GenerateRun() {
    return new Run;
  }

  void RunAction::BeginOfRunAction(G4Run const* a_run) {
    auto nevents = a_run->GetNumberOfEventToBeProcessed();
    if (nevents == 0) {
      return;
    }

    // The master RunAction is always created and started first
    if (IsMaster()) {
      G4cout << "******************************" << G4endl;
      G4cout << "********* RUN BEGIN **********" << G4endl;
      G4cout << "******************************" << G4endl;
      G4cout << "Number of events for run: " << nevents << G4endl;
      // Get a new random seed - only need to do this once in the master
      // thread
      FileManager::get().initialize();
      RandomManager::get().reseed();
    }
    m_timer.Start();
    return;
  }

  void RunAction::EndOfRunAction(G4Run const* a_run) {
    m_timer.Stop();
    auto nevents = a_run->GetNumberOfEvent();
    if (nevents == 0) {
      return;
    }

    // The master RunAction is always stopped and deleted last
    if (IsMaster()) {
      G4cout << "******************************" << G4endl;
      G4cout << "********** RUN END ***********" << G4endl;
      G4cout << "******************************" << G4endl;
      G4cout << "Number of events processed: " << nevents << G4endl;
      G4cout << "Simulation took " << m_timer.RealTime() << " seconds "
        << "real time, " << m_timer.CpuTime() << " seconds CPU time"
        << G4endl;
      if (FileManager::get().getWriteData()) {
        G4cout << "Simulation data was written to "
          << FileManager::get().getPath() << G4endl;
      }
      FileManager::get().close();
    }
    return;
  }
}
