// MPND
#include "generalmanager.hpp"
// Geant4
#include "G4ios.hh"
#include "G4SDManager.hh"
// ROOT
// std

namespace MPND {
  GeneralManager::GeneralManager():
    m_fMT(false)
  {
    G4cout << "Creating GeneralManager..." << G4endl;
    G4SDManager::GetSDMpointer()->SetVerboseLevel(2);
  }

  GeneralManager::~GeneralManager() {
    G4cout << "Deleting GeneralManager..." << G4endl;
  }

  bool const& GeneralManager::getMT() const {
    return m_fMT;
  }

  void GeneralManager::setMT(bool const& a_toggle) {
    m_fMT = a_toggle;
    return;
  }

  DetectorConstruction* GeneralManager::getDetectorConstruction() {
    return m_dc;
  }

  void GeneralManager::setDetectorConstruction(
      DetectorConstruction* a_dc) {
    m_dc = a_dc;
    return;
  }
}
