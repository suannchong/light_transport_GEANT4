// MPND
#include "singleton.hpp"
#include "generalmanager.hpp"
#include "messengermanager.hpp"
#include "filemanager.hpp"
#include "randommanager.hpp"
#include "materialsmanager.hpp"
#include "sdmessenger.hpp"
// Geant4
#include "G4ios.hh"
// ROOT
// std

namespace MPND {
  namespace Singleton {
    void initialize() {
      G4cout << "Instantiating singletons..." << G4endl;
      GeneralManager::get();
      MessengerManager::get();
      FileManager::get();
      RandomManager::get();
      MaterialsManager::get();
      SDMessenger::get();
      return;
    }
  }
}
