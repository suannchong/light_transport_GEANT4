// MPND
#include "materialsmanager.hpp"
// Geant4
#include "G4ios.hh"
#include "G4Element.hh"
#include "G4Isotope.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "CLHEP/Units/SystemOfUnits.h"
using namespace CLHEP;
// ROOT
// std

namespace MPND {
  MaterialsManager::MaterialsManager():
    m_materials{nullptr}
  {
    G4cout << "Creating MaterialsManager..." << G4endl;
    buildAll();
  }

  MaterialsManager::~MaterialsManager() {
    G4cout << "Deleting MaterialsManager..." << G4endl;
  }

  G4Material* MaterialsManager::get(Material::Type const& a_type) const {
    return m_materials[static_cast<int>(a_type)];
  }

  /*G4OpticalSurface* MaterialsManager::get(
      G4String const& a_name, ReflectorType const& a_ref_type,
      G4OpticalSurfaceFinish const& a_finish) {
    return nullptr;
  }*/

  void MaterialsManager::buildAll() {
    G4cout << "MaterialsManager: building elements..." << G4endl;
    buildElements();
    G4cout << "MaterialsManager: building materials..." << G4endl;
    buildVacuum();
    buildAir();
    buildNaI();
    buildEJ200();
    buildBSiGlass();
    buildGS20();
    buildSiPM();
    return;
  }

  void MaterialsManager::buildElements() {
    return;
  }

  void MaterialsManager::buildVacuum() {
    auto nist_man = G4NistManager::Instance();
    m_materials[Material::Vacuum] = nist_man->FindOrBuildMaterial(
        "G4_Galactic");
    return;
  }

  void MaterialsManager::buildAir() {
    auto nist_man = G4NistManager::Instance();
    m_materials[Material::Air] = nist_man->FindOrBuildMaterial(
        "G4_AIR");

    // Add optical properties to G4_AIR - note, this is global!
    auto ptair = new G4MaterialPropertiesTable;
    double energies[2] = { 1.*eV, 6.*eV };
    double r_ind[2] = { 1., 1. };
    double abs[2] = { 2000.*m, 2000.*m };
    ptair->AddProperty("RINDEX", energies, r_ind, 2);
    ptair->AddProperty("ABSLENGTH", energies, abs, 2);
    m_materials[Material::Air]->SetMaterialPropertiesTable(ptair);
    return;
  }

  void MaterialsManager::buildSiPM(){
    auto nist_man = G4NistManager::Instance();
    m_materials[Material::SiPM] = nist_man->FindOrBuildMaterial("G4_Si");

    // Add optical properties to G4_SILICON_DIOXIDE
    auto ptsipm = new G4MaterialPropertiesTable;
    double energies[2] = {1. * eV, 6. * eV};
    double r_ind[2] = {1., 1.};
    double abs[2] = {2000. * m, 2000. * m};
    ptsipm->AddProperty("RINDEX", energies, r_ind, 2);
    ptsipm->AddProperty("ABSLENGTH", energies, abs, 2);
    m_materials[Material::Air]->SetMaterialPropertiesTable(ptsipm);

    return;
  }

  void MaterialsManager::buildNaI() {
    auto nist_man = G4NistManager::Instance();
    m_materials[Material::NaI] = nist_man->FindOrBuildMaterial(
        "G4_SODIUM_IODIDE");

    // Optical properties - TODO
    return;
  }

  void MaterialsManager::buildEJ200() {
    // Basic stuff
    auto nist_man = G4NistManager::Instance();
    G4String mat_name = "MPND_" + Material::TypeString[Material::EJ200];
    // From the datasheet
    auto ej200 = new G4Material(mat_name, 1.023*g/cm3, 2);
    ej200->AddElement(nist_man->FindOrBuildElement(1), 517); // H
    ej200->AddElement(nist_man->FindOrBuildElement(6), 469); // C

    // Optical properties
    auto pt200 = new G4MaterialPropertiesTable;
    pt200->AddConstProperty("SCINTILLATIONYIELD", 10000./MeV);
    // No special Fano factor
    pt200->AddConstProperty("RESOLUTIONSCALE", 1.);
    // Decay time
    pt200->AddConstProperty("FASTTIMECONSTANT", 2.1*ns);
    // Rise time
    pt200->AddConstProperty("FASTSCINTILLATIONRISETIME", 0.9*ns);
    // All fast component, no slow component
    pt200->AddConstProperty("YIELDRATIO", 1.);

    double energies[2] = { 1.*eV, 6.*eV };
    double r_ind[2] = { 1.58, 1.58 };
    double abs[2] = { 380.*cm, 380.*cm };
    double fast_comp[2] = { 3.*eV, 3.*eV };

    pt200->AddProperty("RINDEX", energies, r_ind, 2);
    pt200->AddProperty("ABSLENGTH", energies, abs, 2);
    // Emission wavelength spectrum - TODO
    pt200->AddProperty("FASTCOMPONENT", energies, fast_comp, 2);

    ej200->SetMaterialPropertiesTable(pt200);
    m_materials[Material::EJ200] = ej200;
    return;
  }
  
  void MaterialsManager::buildBSiGlass() {
    G4String mat_name = "MPND_" + Material::TypeString[Material::BSiGlass];
    auto nist_man = G4NistManager::Instance();
    auto bsiglass = new G4Material(mat_name, 2.23*g/cm3, 2);
    // Ignore the boron - should be added for slow neutrons!
    bsiglass->AddElement(nist_man->FindOrBuildElement(14), 1);
    bsiglass->AddElement(nist_man->FindOrBuildElement(5), 2);
    double energies[3] = { 2.*eV, 3.5*eV, 5*eV };
    double index[3] = { 1.482, 1.502, 1.533 };
    double abs[3] = { 1.*mm, 1.*mm, 1.*mm };
    auto pt = new G4MaterialPropertiesTable;
    pt->AddProperty("RINDEX", energies, index, 3);
    pt->AddProperty("ABSLENGTH", energies, abs, 3);
    bsiglass->SetMaterialPropertiesTable(pt);
    
    m_materials[Material::BSiGlass] = bsiglass;
    return;
  }

  void MaterialsManager::buildGS20(){
    // Basic stuff
    G4String mat_name = "MPND_" + Material::TypeString[Material::GS20];
    auto nist_man = G4NistManager::Instance();
    auto gs20 = new G4Material(mat_name, 2.5*g/cm3, 5);
    // From datasheet
    // Create enriched Li (95% 6Li, 5% 6Li)
    G4int nbIsotopes;
    auto enriched_li = new G4Element("Enriched Lithium", "en_Li", nbIsotopes=2);
    auto li6 = new G4Isotope("6Li", 3, 6, 6.015*g/mole);
    auto li7 = new G4Isotope("7Li", 3, 7, 7.016003*g/mole);
    enriched_li->AddIsotope(li6, 95*perCent);
    enriched_li->AddIsotope(li7, 5*perCent);

    auto enriched_li2o = new G4Material("enriched_LITHIUM_OXIDE", 2.01*g/cm3,  2);
    enriched_li2o->AddElement(enriched_li, 2);
    enriched_li2o->AddElement(nist_man->FindOrBuildElement(8),1);

    auto ce2o3 = new G4Material("CERIUM(III)_OXIDE", 6.2*g/cm3,2);
    ce2o3->AddElement(nist_man->FindOrBuildElement(58),2);
    ce2o3->AddElement(nist_man->FindOrBuildElement(8),3);

    gs20->AddMaterial(nist_man->FindOrBuildMaterial("G4_SILICON_DIOXIDE"),57*perCent);
    gs20->AddMaterial(nist_man->FindOrBuildMaterial("G4_ALUMINUM_OXIDE"), 18*perCent);
    gs20->AddMaterial(nist_man->FindOrBuildMaterial("G4_MAGNESIUM_OXIDE"), 4*perCent);
    gs20->AddMaterial(nist_man->FindOrBuildMaterial("enriched_LITHIUM_OXIDE"),     17*perCent);
    gs20->AddMaterial(nist_man->FindOrBuildMaterial("CERIUM(III)_OXIDE"),  4*perCent);

    // // Surface finishes and properties
    // auto opScintSurface = new G4OpticalSurface("ScintillatorSurface");
    // opScintSurface->SetType(dielectric_dielectric);
    // opScintSurface->SetFinish(polished);  // ground = rough surface
    // opScintSurface->SetModel(unified);

    // auto ScintSurface = new G4LogicalSkinSurface("ScintillatorSurface",  volume_log ,opScintSurface);


    // Optical properties
    auto pt20 = new G4MaterialPropertiesTable;
    pt20->AddConstProperty("SCINTILLATIONYIELD", 1255.23 / MeV);
    // No special Fano factor
    pt20->AddConstProperty("RESOLUTIONSCALE", 1.);
    // Decay time
    pt20->AddConstProperty("FASTTIMECONSTANT", 57. * ns);
    pt20->AddConstProperty("SLOWTIMECONSTANT", 98. * ns);
    // All fast component, no slow component
    pt20->AddConstProperty("YIELDRATIO", 1.);

    G4double energies[2] = {1.771147508*eV, 6.149039422*eV};
    G4double r_ind[2] = {1.55, 1.55};
    G4double abs[2] = {1000. * m, 1000. * m};
    G4double ref[2] = {1.0, 1.0};
    G4double efficiency[2] = {1., 1.};
    
    pt20->AddProperty("RINDEX", energies, r_ind, 2);
    pt20->AddProperty("ABSLENGTH", energies, abs, 2);
    pt20->AddProperty("REFLECTIVITY", energies, abs, 2);
    pt20->AddProperty("EFFICIENCY", energies, abs, 2);
    // Emission wavelength spectrum - TODO
    G4double ScintEnergy[] = {
        1.771147508*eV, 1.785693715*eV, 1.806886491*eV, 1.835205916*eV, 1.859861711*eV, 1.887526235 *eV,
        1.911215667*eV, 1.937982147*eV, 1.968038263*eV, 1.993817314*eV, 2.025656659*eV, 2.052965257 *eV,
        2.086731237*eV, 2.11868072 *eV, 2.14858044 *eV, 2.176213997*eV, 2.214200693*eV, 2.243574408 *eV,
        2.270347301*eV, 2.294304242*eV, 2.322309111*eV, 2.354650359*eV, 2.376721902*eV, 2.406788116 *eV,
        2.433745705*eV, 2.469279938*eV, 2.493591512*eV, 2.522559239*eV, 2.556474115*eV, 2.586920261 *eV,
        2.609157128*eV, 2.627217637*eV, 2.650155755*eV, 2.673476749*eV, 2.692441907*eV, 2.711710765 *eV,
        2.726355401*eV, 2.746080968*eV, 2.761100218*eV, 2.77121984 *eV, 2.796776839*eV, 2.817562006 *eV,
        2.828136098*eV, 2.8547708  *eV, 2.871018238*eV, 2.892938281*eV, 2.898532973*eV, 2.915258618 *eV,
        2.926567493*eV, 2.937990045*eV, 2.955188453*eV, 2.966796809*eV, 2.978496723*eV, 2.990276024 *eV,
        2.996253925*eV, 3.008174378*eV, 3.020203583*eV, 3.038367271*eV, 3.050653453*eV, 3.069200332 *eV,
        3.081737652*eV, 3.100651336*eV, 3.139096176*eV, 3.191743043*eV, 3.225507311*eV, 3.239154774 *eV,
        3.245982849*eV, 3.259772991*eV, 3.273696694*eV, 3.2806075  *eV, 3.294726235*eV, 3.301758535 *eV,
        3.308837152*eV, 3.323134953*eV, 3.330272736*eV, 3.337474277*eV, 3.344707031*eV, 3.351971202 *eV,
        3.359216802*eV, 3.366443383*eV, 3.366544205*eV, 3.381295329*eV, 3.396090789*eV, 3.396227597 *eV,
        3.411206068*eV, 3.418692972*eV, 3.426230217*eV, 3.441598022*eV, 3.449219077*eV, 3.464740871 *eV,
        3.480313198*eV, 3.495989894*eV, 3.496098625*eV, 3.520093022*eV, 3.528102993*eV, 3.544381799 *eV,
        3.560849122*eV, 3.569083637*eV, 3.60277172 *eV, 3.654582691*eV, 3.735258252*eV, 3.829204446 *eV,
        3.927975404*eV, 4.010757254*eV, 4.130449603*eV, 4.245659698*eV, 4.367453005*eV, 4.470036937 *eV,
        4.577524753*eV, 4.763720755*eV, 4.886025126*eV, 5.06482313 *eV, 5.293804937*eV, 5.464820789 *eV,
        5.64730209 *eV, 5.864795368*eV, 6.149039422*eV
    };

    const G4int nPoints = sizeof(ScintEnergy)/sizeof(G4double);

    G4double ScintFast[nPoints] = {
        0.004514673, 0.006772009, 0.006772009, 0.009029345, 0.006772009, 0.004514673, 
        0.002257336, 0.004514673, 0.002257336, 0.004514673, 0.006772009, 0.004514673, 
        0.004514673, 0.006772009, 0.006772009, 0.004514673, 0.006772009, 0.009029345, 
        0.011286682, 0.013544018, 0.015801354, 0.020316027, 0.0248307  , 0.027088036, 
        0.033860045, 0.036117381, 0.047404063, 0.058690745, 0.065462754, 0.074492099, 
        0.090293454, 0.101580135, 0.11738149 , 0.128668172, 0.139954853, 0.158013544, 
        0.173814898, 0.18510158 , 0.200902935, 0.214446953, 0.23476298 , 0.250564334, 
        0.270880361, 0.293453725, 0.311512415, 0.329571106, 0.34537246 , 0.358916479, 
        0.376975169, 0.399548533, 0.415349887, 0.431151242, 0.446952596, 0.460496614, 
        0.476297968, 0.489841986, 0.505643341, 0.519187359, 0.53724605 , 0.553047404, 
        0.571106095, 0.584650113, 0.598194131, 0.598194131, 0.591422122, 0.58013544, 
        0.568848758, 0.553047404, 0.539503386, 0.519187359, 0.507900677, 0.492099323, 
        0.478555305, 0.458239278, 0.440180587, 0.426636569, 0.413092551, 0.399548533, 
        0.379232506, 0.35214447 , 0.365688488, 0.338600451, 0.300225734, 0.318284424, 
        0.286681716, 0.264108352, 0.243792325, 0.227990971, 0.205417607, 0.182844244, 
        0.148984199, 0.110609481, 0.124153499, 0.09255079 , 0.074492099, 0.056433409, 
        0.042889391, 0.029345372, 0.018058691, 0.009029345, 0.006772009, 0.006772009, 
        0.004514673, 0.004514673, 0.004514673, 0.006772009, 0.006772009, 0.006772009, 
        0.004514673, 0.004514673, 0.004514673, 0.004514673, 0.006772009, 0.006772009, 
        0.009029345, 0.006772009, 0.006772009,
    };
    pt20->AddProperty("FASTCOMPONENT", ScintEnergy, ScintFast, nPoints);

    gs20->SetMaterialPropertiesTable(pt20);
    m_materials[Material::GS20] = gs20;
    return;
  }


}
