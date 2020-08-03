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
#include "G4SubtractionSolid.hh"

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
    m_dimensions = G4ThreeVector(2.*cm, 2.*cm, 2.*cm);
    m_material = Material::Type::GS20;
    return;
  }

  void SinglePixelDetector::build(BaseVolume* a_mother)
  // void SinglePixelDetector::build(G4LogicalVolume *a_mother_log)
  {
    G4cout << "Building SinglePixelDetector " << m_name << "..." << G4endl;
    // // Scintillator solid
    G4int multiple = 1;
    auto sname = m_name + "_scint_solid";
    auto scint_solid = new G4Box(sname,
                                 multiple*m_dimensions.x() / 2.,
                                 multiple*m_dimensions.y() / 2.,
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

    // Reflector solid, logical, physical 
    sname = m_name + "_reflector_solid";
    G4double thickness_refl = 0.3*mm;
    G4double gap = 0.00*mm;
    auto outer = new G4Box("outer", 
                           thickness_refl + multiple*m_dimensions.x() / 2.,
                           thickness_refl + multiple*m_dimensions.y() / 2.,
                           thickness_refl/2. + m_dimensions.z() / 2.);
    auto inner = new G4Box("inner",
                           gap + multiple*m_dimensions.x() / 2.,
                           gap + multiple*m_dimensions.y() / 2.,
                           gap +  m_dimensions.z() / 2.);
    auto refl_solid = new G4SubtractionSolid("refl_solid", outer, inner, 0, G4ThreeVector(0., 0., thickness_refl/2));
    lname = m_name + "_reflector_log";
    mat = MaterialsManager::get().get(Material::Air);
    auto refl_log = new G4LogicalVolume(refl_solid, mat, lname);
    auto refl_vis = new G4VisAttributes;
    refl_vis->SetForceWireframe(true);
    refl_vis->SetColor(G4Color::Green());
    refl_log->SetVisAttributes(refl_vis);
    pname = m_name + "_refl_phy";
    auto refl_phy = new G4PVPlacement(nullptr,
                                      G4ThreeVector(0., 0., -thickness_refl / 2),
                                      refl_log,
                                      pname,
                                      a_mother->getLogical(),
                                      false,
                                      0,
                                      true);

    // Optical surface
    const G4int NUM = 2;
    const G4double sigmaalpha = 1.3*degree; // 1.3 degrees = 0.0227 rad (polished), 12 degrees (ground)
    G4OpticalSurfaceModel model = unified;
    G4SurfaceType surface_type = dielectric_dielectric;

    // auto ScintSurface = new G4OpticalSurface("ScintSurface_LUT");
    // ScintSurface->SetType(surface_type);
    // ScintSurface->SetModel(model);
    // ScintSurface->SetFinish(polishedteflonair);

    // // Unified Model
    G4double pp[NUM] = {1 * eV, 7 * eV};
    G4double specularlobe[NUM] = {1.0, 1.0};
    G4double specularspike[NUM] = {0.0, 0.0};
    G4double backscatter[NUM] = {0.0, 0.0};
    G4double lambertian[NUM] = {0.0, 0.0};

    // // Air reflector (polished) use snell's law when reflection occurs 
    auto polishedAir = new G4OpticalSurface("polishedAir");
    polishedAir->SetType(surface_type);
    polishedAir->SetModel(model);
    // polishedAir->SetFinish(polished);
    polishedAir->SetFinish(ground); // ground = rough surface
    polishedAir->SetSigmaAlpha(1.3*degree); //Janecek 2010

    G4MaterialPropertiesTable *polishedAir_MPT = new G4MaterialPropertiesTable();
    G4double air_rindex[NUM] = {1.00, 1.00};
    G4double air_refl[NUM] = {0.0465, 0.0465}; 
    G4double air_eff[NUM] = {0.0, 0.0};
    polishedAir_MPT->AddProperty("RINDEX", pp, air_rindex, NUM);
    polishedAir_MPT->AddProperty("REFLECTIVITY", pp, air_refl, NUM);
    polishedAir_MPT->AddProperty("EFFICIENCY", pp, air_eff, NUM);
    // polishedAir_MPT->AddProperty("SPECULARLOBECONSTANT", pp, specularlobe, NUM);
    // polishedAir_MPT->AddProperty("SPECULARSPIKECONSTANT", pp, specularspike, NUM);
    // polishedAir_MPT->AddProperty("BACKSCATTERCONSTANT", pp, backscatter, NUM);
    // polishedAir_MPT->AddProperty("DIFFUSELOBECONSTANT", pp, lambertian, NUM); // assume purely lambertian
    polishedAir->SetMaterialPropertiesTable(polishedAir_MPT);

    // // Air reflector (ground), assumed lambertian (diffuse)
    auto groundAir = new G4OpticalSurface("groundAir");
    groundAir->SetType(surface_type);
    groundAir->SetModel(model);
    groundAir->SetFinish(ground); // groundfrontpainted = only lambertian reflection
    groundAir->SetSigmaAlpha(12* degree);

    G4MaterialPropertiesTable *groundAir_MPT = new G4MaterialPropertiesTable();
    groundAir_MPT->AddProperty("RINDEX", pp, air_rindex, NUM);
    groundAir_MPT->AddProperty("REFLECTIVITY", pp, air_refl, NUM);
    groundAir_MPT->AddProperty("EFFICIENCY", pp, air_eff, NUM);
    groundAir_MPT->AddProperty("SPECULARLOBECONSTANT", pp, specularlobe, NUM);
    groundAir_MPT->AddProperty("SPECULARSPIKECONSTANT", pp, specularspike, NUM);
    groundAir_MPT->AddProperty("BACKSCATTERCONSTANT", pp, backscatter, NUM);
    groundAir_MPT->AddProperty("DIFFUSELOBECONSTANT", pp,lambertian, NUM); // assume purely lambertian
    groundAir->SetMaterialPropertiesTable(groundAir_MPT);

    // TiO2 reflector (polished tio air )
    auto polishedTiO2Air = new G4OpticalSurface("polishedTiO2Air");
    polishedTiO2Air->SetType(surface_type);
    polishedTiO2Air->SetModel(model);
    polishedTiO2Air->SetFinish(groundbackpainted); //groundbackpainted (lambertian)
    polishedTiO2Air->SetSigmaAlpha(1.3*degree); // Janecek 2010

    G4MaterialPropertiesTable *polishedTiO2Air_MPT = new G4MaterialPropertiesTable();
    G4double tio2_rindex[NUM] = {1.35, 1.35};
    G4double tio2_refl[NUM] = {0.955, 0.955}; // Janecek 2010 (actually TiO2 cut off at 420 nm)
    G4double tio2_eff[NUM] = {0.0, 0.0};
    polishedTiO2Air_MPT->AddProperty("RINDEX", pp, tio2_rindex, NUM);
    polishedTiO2Air_MPT->AddProperty("REFLECTIVITY", pp, tio2_refl, NUM);
    polishedTiO2Air_MPT->AddProperty("EFFICIENCY", pp, tio2_eff, NUM);
    polishedTiO2Air_MPT->AddProperty("SPECULARLOBECONSTANT", pp, specularlobe, NUM);
    polishedTiO2Air_MPT->AddProperty("SPECULARSPIKECONSTANT", pp, specularspike, NUM);
    polishedTiO2Air_MPT->AddProperty("BACKSCATTERCONSTANT", pp, backscatter, NUM);
    polishedTiO2Air_MPT->AddProperty("DIFFUSELOBECONSTANT", pp, lambertian, NUM);
    polishedTiO2Air->SetMaterialPropertiesTable(polishedTiO2Air_MPT);

    // TiO2 reflector (ground tio air)
    auto groundTiO2Air = new G4OpticalSurface("groundTiO2Air");
    groundTiO2Air->SetType(surface_type);
    groundTiO2Air->SetModel(model);
    groundTiO2Air->SetFinish(groundbackpainted); // (lambertian ground mirror )
    groundTiO2Air->SetSigmaAlpha(12*degree); // Janecek 2010

    G4MaterialPropertiesTable *groundTiO2Air_MPT = new G4MaterialPropertiesTable();
    groundTiO2Air_MPT->AddProperty("RINDEX", pp, tio2_rindex, NUM);
    groundTiO2Air_MPT->AddProperty("REFLECTIVITY", pp, tio2_refl, NUM);
    groundTiO2Air_MPT->AddProperty("EFFICIENCY", pp, tio2_eff, NUM);
    groundTiO2Air_MPT->AddProperty("SPECULARLOBECONSTANT", pp, specularlobe, NUM);
    groundTiO2Air_MPT->AddProperty("SPECULARSPIKECONSTANT", pp, specularspike, NUM);
    groundTiO2Air_MPT->AddProperty("BACKSCATTERCONSTANT", pp, backscatter, NUM);
    groundTiO2Air_MPT->AddProperty("DIFFUSELOBECONSTANT", pp, lambertian, NUM);
    groundTiO2Air->SetMaterialPropertiesTable(groundTiO2Air_MPT);

    // new G4LogicalBorderSurface("scintillator_reflector_border", scint_phy, a_mother->getPhysical(), groundAir);
    new G4LogicalBorderSurface("scintillator_reflector_border", scint_phy, refl_phy,polishedTiO2Air);

    // // Teflon Air reflector
    // auto polishedTeflonAir = new G4OpticalSurface("polishedTeflonAir");
    // polishedTeflonAir->SetType(surface_type);
    // polishedTeflonAir->SetModel(model);
    // polishedTeflonAir->SetFinish(groundbackpainted);
    // polishedTeflonAir->SetSigmaAlpha(sigmaalpha); // Janecek 2010

    // G4MaterialPropertiesTable *polishedTeflonAir_MPT = new G4MaterialPropertiesTable();
    // G4double teflon_rindex[NUM] = {1.35, 1.35};
    // G4double teflon_refl[NUM] = {0.99, 0.99}; // Janecek 2010
    // polishedTeflonAir_MPT->AddProperty("RINDEX", pp, teflon_rindex, NUM);
    // polishedTeflonAir_MPT->AddProperty("REFLECTIVITY", pp, teflon_refl, NUM);
    // polishedTeflonAir_MPT->AddProperty("SPECULARLOBECONSTANT", pp, specularlobe, NUM);
    // polishedTeflonAir_MPT->AddProperty("SPECULARSPIKECONSTANT", pp, specularspike, NUM);
    // polishedTeflonAir_MPT->AddProperty("BACKSCATTERCONSTANT", pp, backscatter, NUM);
    // polishedTeflonAir->SetMaterialPropertiesTable(polishedTeflonAir_MPT);

    // // VM2000 Air reflector
    // auto polishedVM2000Air = new G4OpticalSurface("polishedVM2000Air");
    // polishedVM2000Air->SetType(surface_type);
    // polishedVM2000Air->SetModel(model);
    // polishedVM2000Air->SetFinish(groundbackpainted);
    // polishedVM2000Air->SetSigmaAlpha(sigmaalpha); // Janecek 2010

    // G4MaterialPropertiesTable *polishedVM2000Air_MPT = new G4MaterialPropertiesTable();
    // G4double vm2000_rindex[NUM] = {0.92, 0.92};
    // G4double vm2000_refl[NUM] = {0.985, 0.985}; // Janecek 2010
    // polishedVM2000Air_MPT->AddProperty("RINDEX", pp, vm2000_rindex, NUM);
    // polishedVM2000Air_MPT->AddProperty("REFLECTIVITY", pp, vm2000_refl, NUM);
    // polishedVM2000Air_MPT->AddProperty("SPECULARLOBECONSTANT", pp, specularlobe, NUM);
    // polishedVM2000Air_MPT->AddProperty("SPECULARSPIKECONSTANT", pp, specularspike, NUM);
    // polishedVM2000Air_MPT->AddProperty("BACKSCATTERCONSTANT", pp, backscatter, NUM);
    // polishedVM2000Air->SetMaterialPropertiesTable(polishedVM2000Air_MPT);

    // // G4LogicalSkinSurface* scintSurface = new G4LogicalSkinSurface("ScintillatorSurface", scint_log, opScintSurface);

    // // Inactive Detector Area
    G4double pmt_thickness = 1 * cm;
    G4double air_gap = 0.0 * mm;
    G4double fraction = 1.0;

    // PMT (single pixel)
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
    dc = GeneralManager::get().getDetectorConstruction();
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

    auto SiPM_surface = new G4OpticalSurface("SiPM_surface");
    SiPM_surface->SetType(dielectric_metal);
    SiPM_surface->SetModel(unified);
    SiPM_surface->SetFinish(polished); // ground = rough surface

    G4MaterialPropertiesTable *SiPM_surface_MPT = new G4MaterialPropertiesTable();
    G4double rindex[NUM] = {1.482, 1.533};
    G4double ref[NUM] = {0.0, 0.0};
    G4double eff[NUM] = {1.0, 1.0};  // detect optical photons
    G4double abs[NUM] = {1.*cm, 1.*cm};
    SiPM_surface_MPT->AddProperty("RINDEX", pp, rindex, NUM);
    SiPM_surface_MPT->AddProperty("REFLECTIVITY", pp, ref, NUM);
    SiPM_surface_MPT->AddProperty("EFFICIENCY", pp, eff, NUM);
    SiPM_surface_MPT->AddProperty("ABSLENGTH", pp, abs, NUM);
    SiPM_surface->SetMaterialPropertiesTable(SiPM_surface_MPT);

    new G4LogicalBorderSurface("scintillator_pmt_border", scint_phy, pmt_phy, SiPM_surface);


    // PMT (pixel array) monolithic scintillator 
    // // G4double pmt_thickness = 1 * cm;
    // sname = m_name + "pmt_solid";
    // auto pmt_solid = new G4Box(sname,
    //                            m_dimensions.x() / 2.,
    //                            m_dimensions.y() / 2.,
    //                            pmt_thickness / 2.);
    // lname = m_name + "_pmt_log";
    // mat = MaterialsManager::get().get(Material::BSiGlass);
    // auto pmt_log = new G4LogicalVolume(pmt_solid, mat, lname);
    // auto pmt_vis = new G4VisAttributes;
    // pmt_vis->SetForceWireframe(true);
    // pmt_vis->SetColor(G4Color::Magenta());
    // pmt_log->SetVisAttributes(pmt_vis);

    // dc->linkLVSD(lname, "pmt_sd");
    // G4int m_nX = 8;
    // G4int m_nY = 8;
    // double xmin = -(m_nX * m_dimensions.x() / 2.) + m_dimensions.x() / 2.;
    // double ymin = -(m_nY * m_dimensions.y() / 2.) + m_dimensions.y() / 2.;

    // for (int ix = 0; ix < m_nX; ++ix){
    //   for (int iy = 0; iy < m_nY; ++iy){
    //     pname = m_name + "_pmt_phys_" + std::to_string(ix) + "_" + std::to_string(iy);
    //     double xpos = (xmin + ((double)ix * m_dimensions.x()));
    //     double ypos = (ymin + ((double)iy * m_dimensions.y()));
    //     auto pmt_phy = new G4PVPlacement(nullptr,
    //                       G4ThreeVector(xpos, ypos, (m_dimensions.z() + pmt_thickness) / 2.),
    //                       pmt_log,
    //                       pname,
    //                       a_mother->getLogical(),
    //                       false,
    //                       0,
    //                       true);
    //     auto SiPM_surface = new G4OpticalSurface("SiPM_surface");
    //     SiPM_surface->SetType(dielectric_metal);
    //     SiPM_surface->SetModel(unified);
    //     SiPM_surface->SetFinish(polished); // ground = rough surface
    //     SiPM_surface->SetSigmaAlpha(0.);

    //     G4MaterialPropertiesTable *SiPM_surface_MPT = new G4MaterialPropertiesTable();
    //     G4double ref[NUM] = {0.0, 0.0};
    //     G4double eff[NUM] = {1.0, 1.0};
    //     SiPM_surface_MPT->AddProperty("REFLECTIVITY", pp, ref, NUM);
    //     SiPM_surface_MPT->AddProperty("EFFICIENCY", pp, eff, NUM);
    //     SiPM_surface->SetMaterialPropertiesTable(SiPM_surface_MPT);
    //     new G4LogicalBorderSurface("scintillator_pmt_border", scint_phy, pmt_phy, SiPM_surface);
    //   }
    // }



    return;
    }

  void SinglePixelDetector::setMaterial(Material::Type const &a_mat){
    m_material = a_mat;
    return;
  }
}
