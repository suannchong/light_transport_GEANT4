// MPND
#include "singlepixeldetector.hpp"
#include "materialsmanager.hpp"
#include "generalmanager.hpp"
#include "world.hpp"

// Geant4
#include "G4Box.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include "G4LogicalBorderSurface.hh"

using namespace CLHEP;
// ROOT
// std

namespace MPND {
  SinglePixelDetector::SinglePixelDetector(G4String const& a_name):
    BaseVolume(a_name)
  {
    G4cout << "Creating SinglePixelDetector " << m_name << G4endl;
    setDefaults();
    m_vis = new G4VisAttributes;
    m_vis->SetForceWireframe(true);
    m_vis->SetColor(G4Color::White());
  }

  SinglePixelDetector::~SinglePixelDetector() {
    G4cout << "Deleting SinglePixelDetector " << m_name << G4endl;
    delete m_vis;
  }

  void SinglePixelDetector::setDefaults() {
    m_position = G4ThreeVector(0., 0., 0.);
    //m_dimensions = G4ThreeVector(6.*mm, 6.*mm, 10.*mm);
    m_dimensions = G4ThreeVector(2.*mm, 2.*mm, 2.*mm);
    m_material = Material::Type::EJ200;
    return;
  }

  void SinglePixelDetector::build(BaseVolume* a_mother)
  // void SinglePixelDetector::build(G4LogicalVolume *a_mother_log)
  {
    G4cout << "Building SinglePixelDetector " << m_name << "..." << G4endl;
    // Scintillator solid
    auto sname = m_name + "_scint_solid";
    auto scint_solid = new G4Box(sname,
                                 m_dimensions.x() / 2.,
                                 m_dimensions.y() / 2.,
                                 m_dimensions.z() / 2.);
    // Scintillator logical
    auto lname = m_name + "_scint_log";
    // auto& mat_man = MaterialsManager::get();
    // auto mat = mat_man.get(Material::GS20);
    auto mat = MaterialsManager::get().get(m_material);
    auto scint_log = new G4LogicalVolume(scint_solid, mat, lname);
    scint_log->SetVisAttributes(m_vis);
    // Connect this logical volume to the scintillator SD
    auto dc = GeneralManager::get().getDetectorConstruction();
    dc->linkLVSD(lname, "scintillator_sd");
    // Scintillator physical
    auto pname = m_name + "_scint_phys";
    auto scint_phy = new G4PVPlacement(nullptr,
                      m_position,
                      scint_log,
                      pname,
                      a_mother->getLogical(),
                      false,
                      0,
                      true);

    // Optical surface
    // Surface finishes and properties
    auto opScintSurface = new G4OpticalSurface("ScintillatorSurface");
    opScintSurface->SetType(dielectric_LUT);
    opScintSurface->SetModel(LUT);
    opScintSurface->SetFinish(polishedtioair); // ground = rough surface
    
    // opScintSurface->SetType(dielectric_LUT);
    // opScintSurface->SetModel(unified);
    // opScintSurface->SetFinish(ground); // ground = rough surface

    // opScintSurface->SetSigmaAlpha(0.15);

    // G4LogicalSkinSurface* scintSurface = new G4LogicalSkinSurface("ScintillatorSurface", scint_log, opScintSurface);
    new G4LogicalBorderSurface("Scintillator", scint_phy, a_mother->getPhysical(), opScintSurface);

    // G4MaterialPropertiesTable* OpSurfaceProperty = new G4MaterialPropertiesTable();

    // const G4int n = 2;
    // G4double energies[n] = {1.771147508*eV, 6.149039422*eV};
    // G4double r_ind[n] = {1.55, 1.55};
    // G4double ref[n] = {1., 1.};
    // G4double eff[n] = {0.8, 1.};

    // OpSurfaceProperty->AddProperty("RINDEX", energies, r_ind, n);
    // OpSurfaceProperty->AddProperty("REFLECTIVITY", energies, ref,n);
    // OpSurfaceProperty->AddProperty("EFFICIENCY", energies, eff,n);

    // opScintSurface->SetMaterialPropertiesTable(OpSurfaceProperty);

    // PMT
    G4double pmt_thickness = 1*cm;
    G4double air_gap = 0.0*mm;
    G4double fraction = 0.5;
    sname = m_name + "pmt_solid";
    auto pmt_solid = new G4Box(sname,
                               fraction * m_dimensions.x() / 2.,
                               fraction * m_dimensions.y() / 2.,
                               pmt_thickness / 2.);
    lname = m_name + "_pmt_log";
    // mat = mat_man.get(Material::BSiGlass);
    mat = MaterialsManager::get().get(Material::BSiGlass);
    auto pmt_log = new G4LogicalVolume(pmt_solid, mat, lname);
    auto pmt_vis = new G4VisAttributes;
    pmt_vis->SetForceWireframe(true);
    pmt_vis->SetColor(G4Color::Magenta());
    pmt_log->SetVisAttributes(pmt_vis);
    dc->linkLVSD(lname, "pmt_sd");
    pname = m_name + "_pmt_phys";
    auto pmt_phy = new G4PVPlacement(nullptr,
                      G4ThreeVector(0., 0.,
                                    (m_dimensions.z() + pmt_thickness + air_gap) / 2.),
                      pmt_log,
                      pname,
                      a_mother->getLogical(),
                      false,
                      0,
                      true);
    return;
  }

  void SinglePixelDetector::setMaterial(Material::Type const &a_mat){
    m_material = a_mat;
    return;
  }
}
