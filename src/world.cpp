// MPND
#include "world.hpp"
#include "materialsmanager.hpp"
// Geant4
#include "G4NistManager.hh"
#include "CLHEP/Units/SystemOfUnits.h"
using namespace CLHEP;
// ROOT
// std

namespace MPND {
  World::World(G4String const& a_name):
    BaseVolume(a_name),
    m_solid(nullptr)
    // m_logical(nullptr),
    // m_physical(nullptr)
  {
    G4cout << "Creating " << m_name << " World volume..." << G4endl;
    setDefaults();
  }

  World::~World() {
    G4cout << "Deleting " << m_name << " World volume..." << G4endl;
  }

  // G4LogicalVolume* World::getLogical() const {
  //   return m_logical;
  // }

  // G4PVPlacement* World::getPhysical() const {
  //   return m_physical;
  // }

  void World::setDefaults() {
    m_position = G4ThreeVector(0.*cm, 0.*cm, 0.*cm);
    m_dimensions = G4ThreeVector(50.*cm, 50.*cm, 50.*cm);
    return;
  }

  void World::build(BaseVolume* ){
  // void World::build(G4LogicalVolume*) {
    G4cout << "Building " << m_name << " World volume...";
    auto sname = m_name + "_solid";
    m_solid = new G4Box(sname,
                        m_dimensions.x() / 2.0,
                        m_dimensions.y() / 2.0,
                        m_dimensions.z() / 2.0);

    auto& mat_man = MaterialsManager::get();
    auto mat = mat_man.get(Material::Vacuum);
    auto lname = m_name + "_log";
    m_logical = new G4LogicalVolume(m_solid, mat, lname);

    auto pname = m_name + "_phys";
    m_physical = new G4PVPlacement(nullptr,
                                   m_position,
                                   m_logical,
                                   pname,
                                   nullptr,
                                   false,
                                   0,
                                   true);
    G4cout << "done" << G4endl;
    return;
  }

}
