// MPND
#include "pixelarraydetector.hpp"
#include "materialsmanager.hpp"
#include "generalmanager.hpp"
// Geant4
#include "G4Box.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "CLHEP/Units/SystemOfUnits.h"
using namespace CLHEP;
// ROOT
// std

namespace MPND {
  PixelArrayDetector::PixelArrayDetector(G4String const& a_name):
    BaseVolume(a_name)
  {
    G4cout << "Creating PixelArrayDetector " << m_name << G4endl;
    setDefaults();
    m_vis = new G4VisAttributes;
    m_vis->SetForceWireframe(true);
    m_vis->SetColor(G4Color::White());
  }

  PixelArrayDetector::~PixelArrayDetector() {
    G4cout << "Deleting PixelArrayDetector " << m_name << G4endl;
    delete m_vis;
  }

  int const& PixelArrayDetector::getNX() const {
    return m_nX;
  }

  void PixelArrayDetector::setNX(int const& a_nx) {
    m_nX = a_nx;
    return;
  }

  int const& PixelArrayDetector::getNY() const {
    return m_nY;
  }

  void PixelArrayDetector::setNY(int const& a_ny) {
    m_nY = a_ny;
    return;
  }

  void PixelArrayDetector::setDefaults() {
    m_position = G4ThreeVector(0., 0., 0.);
    m_dimensions = G4ThreeVector(2.*mm, 2.*mm, 2.*mm);
    m_material = Material::Type::GS20; 
    m_nX = 8;
    m_nY = 8;
    return;
  }

    void PixelArrayDetector::build(BaseVolume* a_mother){
  // void PixelArrayDetector::build(G4LogicalVolume* a_mother_log) {
    G4cout << "Building PixelArrayDetector " << m_name << "..." << G4endl;
    // Scintillator solid
    auto sname = m_name + "_scint_solid";
    auto scint_solid = new G4Box(sname,
                                 m_dimensions.x() / 2.,
                                 m_dimensions.y() / 2.,
                                 m_dimensions.z() / 2.);
    // Scintillator logical
    auto lname = m_name + "_scint_log";
    // auto &mat_man = MaterialsManager::get();
    // auto mat = mat_man.get(Material::EJ200);
    auto mat = MaterialsManager::get().get(m_material);
    // auto nist = G4NistManager::Instance();
    // auto mat = nist->FindOrBuildMaterial("G4_WATER");
    auto scint_log = new G4LogicalVolume(scint_solid, mat, lname);
    scint_log->SetVisAttributes(m_vis);

    // Optical surface
    // Surface finishes and properties
    auto opScintSurface = new G4OpticalSurface("ScintillatorSurface");
    opScintSurface->SetType(dielectric_LUT);
    opScintSurface->SetModel(LUT);
    opScintSurface->SetFinish(groundteflonair); // ground = rough surface

    // opScintSurface->SetSigmaAlpha(0.1);

    G4LogicalSkinSurface *scintSurface = new G4LogicalSkinSurface("ScintillatorSurface", scint_log, opScintSurface);
    G4MaterialPropertiesTable *OpSurfaceProperty = new G4MaterialPropertiesTable();

    const G4int pmt_n = 2;
    G4double pmt_energies[pmt_n] = {.1 * eV, 7. * eV};
    G4double pmt_r_ind[pmt_n] = {1.55, 1.55};
    G4double pmt_ref[pmt_n] = {1.0, 1.0};
    G4double pmt_efficiency[pmt_n] = {1., 1.};

    OpSurfaceProperty->AddProperty("RINDEX", pmt_energies, pmt_r_ind, pmt_n);
    OpSurfaceProperty->AddProperty("REFLECTIVITY", pmt_energies, pmt_ref, pmt_n);
    OpSurfaceProperty->AddProperty("EFFICIENCY", pmt_energies, pmt_efficiency, pmt_n);

    opScintSurface->SetMaterialPropertiesTable(OpSurfaceProperty);

    // Connect this logical volume to the scintillator SD
    auto dc = GeneralManager::get().getDetectorConstruction();
    dc->linkLVSD(lname, "scintillator_sd");
    // Scintillator physical
    double xmin = -(m_nX * m_dimensions.x() / 2.) + m_dimensions.x() / 2.;
    double ymin = -(m_nY * m_dimensions.y() / 2.) + m_dimensions.y() / 2.;
    for (int ix=0;ix < m_nX;++ix) {
      for (int iy=0;iy < m_nY;++iy) {
        auto pname = m_name + "_scint_phys_" + std::to_string(ix)
          + "_" + std::to_string(iy);
        double xpos = (xmin + ((double)ix * m_dimensions.x()));
        double ypos = (ymin + ((double)iy * m_dimensions.y()));
        new G4PVPlacement(nullptr,
                          G4ThreeVector(xpos, ypos, 0.),
                          scint_log,
                          pname,
                          a_mother->getLogical(),
                          false,
                          0,
                          true);
      }
    }

    // PMT
    G4double pmt_thickness = 1*cm;
    sname = m_name + "pmt_solid";
    auto pmt_solid = new G4Box(sname,
                               m_dimensions.x() / 2.,
                               m_dimensions.y() / 2.,
                               pmt_thickness / 2.);
    lname = m_name + "_pmt_log";
    mat = MaterialsManager::get().get(Material::BSiGlass);
    auto pmt_log = new G4LogicalVolume(pmt_solid, mat, lname);
    auto pmt_vis = new G4VisAttributes;
    pmt_vis->SetForceWireframe(true);
    pmt_vis->SetColor(G4Color::Magenta());
    pmt_log->SetVisAttributes(pmt_vis);

    // // Optical surface
    // // Surface finishes and properties
    // auto opPMTSurface = new G4OpticalSurface("PMTSurface");
    // opPMTSurface->SetType(dielectric_dielectric);
    // opPMTSurface->SetModel(LUT);
    // opPMTSurface->SetFinish(PolishedTeflon_LUT); // ground = rough surface

    // // opScintSurface->SetSigmaAlpha(0.1);

    // G4LogicalSkinSurface *PMTSurface = new G4LogicalSkinSurface("PMTSurface", scint_log, opScintSurface);
    // G4MaterialPropertiesTable *OpPMTSurfaceProperty = new G4MaterialPropertiesTable();

    // const G4int n = 2;
    // G4double energies[n] = {.1 * eV, 7. * eV};
    // G4double r_ind[n] = {1.55, 1.55};
    // G4double ref[n] = {1.0, 1.0};
    // G4double efficiency[n] = {1., 1.};

    // OpPMTSurfaceProperty->AddProperty("RINDEX", energies, r_ind, n);
    // OpPMTSurfaceProperty->AddProperty("REFLECTIVITY", energies, ref, n);
    // OpPMTSurfaceProperty->AddProperty("EFFICIENCY", energies, efficiency, n);

    // opPMTSurface->SetMaterialPropertiesTable(OpPMTSurfaceProperty);

    dc->linkLVSD(lname, "pmt_sd");

    for (int ix=0;ix < m_nX;++ix) {
      for (int iy=0;iy < m_nY;++iy) {
        auto pname = m_name + "_pmt_phys_" + std::to_string(ix)
          + "_" + std::to_string(iy);
        double xpos = (xmin + ((double)ix * m_dimensions.x()));
        double ypos = (ymin + ((double)iy * m_dimensions.y()));
        new G4PVPlacement(nullptr,
                          G4ThreeVector(xpos, ypos, (m_dimensions.z() + pmt_thickness) / 2.),
                          pmt_log,
                          pname,
                          a_mother->getLogical(),
                          false,
                          0,
                          true);
      }
    }
    return;
  }

  void PixelArrayDetector::setMaterial(Material::Type const& a_mat){
    m_material = a_mat;
    return;
  }
}
