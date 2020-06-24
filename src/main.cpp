// MPND
#include "singleton.hpp"
#include "generalmanager.hpp"
#include "detectorconstruction.hpp"
#include "actioninitialization.hpp"
#include "physicslist.hpp"
// Geant4
#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#ifdef MPNDG4VIS
  #include "G4VisExecutive.hh"
#endif
// ROOT
#include <TROOT.h>
// std

int main(int argc, char* argv[]) {
  // Force ROOT to initialize before anything else! MUST BE FIRST! This
  // addresses the following error that is encountered when using ROOT and
  // Geant4 together:
  // LLVM SYMBOLS ARE EXPOSED TO CLING! This will cause problems; please
  // hide them or dlopen() them after the call to TROOT::InitInterpreter()!
  // The fix works by loading the contentious libmesa symbols *before*
  // Geant4 tries to. Geant4 will see they exist and then just use those
  // instead of causing a collision. See this issue for more info:
  // https://root-forum.cern.ch/t/error-llvm-symbols-exposed-to-cling/23597
  gROOT->GetInterpreter();

  // Use ALL of the threads!!! by default. The user can change it with
  // /run/numberOfThreads <N>
  #ifdef G4MULTITHREADED
    #ifdef MPNDG4MT
      // False by default
      MPND::GeneralManager::get().setMT(true);
    #endif
  #endif
  G4RunManager* run_man = nullptr;
  if (MPND::GeneralManager::get().getMT()) {
    run_man = new G4MTRunManager;
    ((G4MTRunManager*)run_man)->SetNumberOfThreads(
      G4Threading::G4GetNumberOfCores());
    G4cout << "MPNDG4 is running with "
      << G4Threading::G4GetNumberOfCores() << " threads." << G4endl;
    // Set custom initialization classes, detector construction and physics
    // list - shared objects
    ((G4MTRunManager*)run_man)->SetUserInitialization(
      new MPND::DetectorConstruction);
    ((G4MTRunManager*)run_man)->SetUserInitialization(
      new MPND::PhysicsList);
    // Set custom action classes - for threaded objects
    ((G4MTRunManager*)run_man)->SetUserInitialization(
      new MPND::ActionInitialization);
  } else {
    run_man = new G4RunManager;
    G4cout << "MPND is running in sequential mode." << G4endl;
    // Set custom initialization classes, detector construction and physics
    // list - shared objects
    run_man->SetUserInitialization(new MPND::DetectorConstruction);
    run_man->SetUserInitialization(new MPND::PhysicsList);
    run_man->SetUserInitialization(new MPND::ActionInitialization);
  }

  // Force all singletons to instantiate, guaranteeing the messengers, etc
  MPND::Singleton::initialize();

  // Do NOT initialize the RunManager! The user must do it before running

  // Initialize visualization, only if the user did not specify a macro
  // argument
  G4VisManager* vis_man = nullptr;
  #ifdef MPNDG4VIS
    if (argc == 1) {
      vis_man = new G4VisExecutive;
      vis_man->Initialize();
    }
  #endif

  // Initialize the UI
  auto ui_man = G4UImanager::GetUIpointer();
  // Change the default macro path
  ui_man->ApplyCommand("/control/macroPath init_mac/");

  // Handle input file if there is one. If not, open the visualization
  // if it's enabled, otherwise open the command line UI
  if (argc == 2) {
    G4String cmd = "/control/execute " + (G4String)argv[1];
    ui_man->ApplyCommand(cmd);
  } else {
    auto ui = new G4UIExecutive(argc, argv);
    if (vis_man) {
      ui_man->ApplyCommand("/control/execute init_vis.mac");
    } else {
      ui_man->ApplyCommand("/control/execute init.mac");
    }
    ui->SessionStart();
    delete ui;
  }

  // Clean up
  delete vis_man;
  delete run_man;
  return 0;
}
