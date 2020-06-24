// MPND
#include "detectorconstruction.hpp"
#include "geometrymessenger.hpp"
#include "generalmanager.hpp"
#include "singlepixeldetector.hpp"
#include "pixelarraydetector.hpp"
#include "sensitivedetector.hpp"
// Geant4
#include "G4MTRunManager.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4VVisManager.hh"
#include "G4UImanager.hh"
#include "G4SDManager.hh"
#include "G4Threading.hh"
#include "G4MTRunManager.hh"
#include "CLHEP/Units/SystemOfUnits.h"
using namespace CLHEP;
// ROOT
// std

namespace MPND {
  DetectorConstruction::DetectorConstruction():
    G4VUserDetectorConstruction(),
    m_world(new World("world")),
    m_config(Detectors::Config::singlepixel),
    m_detectors({nullptr}),
    m_messenger(new GeometryMessenger(this))
  {
    G4cout << "Creating DetectorConstruction..." << G4endl;
    // Set the global pointer to this object
    GeneralManager::get().setDetectorConstruction(this);

    // Set SD allocation flags
    int NTHREADS = 1;
    if (GeneralManager::get().getMT()) {
      NTHREADS
        = G4MTRunManager::GetMasterRunManager()->GetNumberOfThreads() + 1;
    }
    m_fSDAllocated = std::vector<bool>(NTHREADS, false);

    // Allocate the different Detectors
    int index = static_cast<int>(Detectors::Config::singlepixel);
    m_detectors[index] = new SinglePixelDetector(
      Detectors::ConfigString[index]);
    index = static_cast<int>(Detectors::Config::pixelarray);
    m_detectors[index] = new PixelArrayDetector(
      Detectors::ConfigString[index]);
  }

  DetectorConstruction::~DetectorConstruction() {
    G4cout << "Deleting DetectorConstruction..." << G4endl;
    delete m_world;
    for (auto& d : m_detectors) {
      delete d;
    }
    delete m_messenger;
  }

  G4VPhysicalVolume* DetectorConstruction::Construct() {
    m_world->build();
    int index = static_cast<int>(m_config);
    m_detectors[index]->build(m_world);
    // m_detectors[index]->build(m_world->getLogical());
    return m_world->getPhysical();
  }

  void DetectorConstruction::ConstructSDandField() {
    int ithread = 0;
    if (GeneralManager::get().getMT()) {
      ithread = G4Threading::G4GetThreadId() + 1;
    }
    auto sd_man = G4SDManager::GetSDMpointer();
    if (!m_fSDAllocated[ithread]) {
      // Allocate all of the SDs that will be available. This must be
      // done once per thread. They do not get deleted when the geometry
      // gets reloaded
      auto sd = new SensitiveDetector("scintillator_sd");
      auto pfilter = new ParticleFilter("scint_particle_filter");
      pfilter->add("e-");
      sd->setFilter(Filters::Type::particle, pfilter);
      auto efilter = new EnergyFilter("scint_energy_filter");
      efilter->add(100.*eV, 10.*GeV);
      sd->setFilter(Filters::Type::energy, efilter);
      sd_man->AddNewDetector(sd);

      sd = new SensitiveDetector("pmt_sd");
      pfilter = new ParticleFilter("pmt_particle_filter");
      pfilter->add("opticalphoton");
      sd->setFilter(Filters::Type::particle, pfilter);
      efilter = new EnergyFilter("pmt_energy_filter");
      efilter->add(.5*eV, 7.*eV);
      sd->setFilter(Filters::Type::energy, efilter);
      sd_man->AddNewDetector(sd);

      sd = new SensitiveDetector("other_sd");
      sd_man->AddNewDetector(sd);
      m_fSDAllocated[ithread] = true;
    }
    // Connect LVs to SDs, which needs to be done the first time we run
    // or when we reload the geometry
    for (auto& link : m_linkLVSD) {
      auto sd = sd_man->FindSensitiveDetector(link.second);
      if (!sd) {
        G4String msg = "Tried to connect LV " + link.first + " to an ";
        msg += "SD (" + link.second + ") that does not exist!";
        G4Exception("DetectorConstruction::ConstructSDandField()",
            "MPNDException", FatalException, msg);
      }
      G4cout << "Connecting LV " << link.first << " to SD " << link.second
        << G4endl;
      SetSensitiveDetector(link.first, sd);
    }
    return;
  }

  Detectors::Config const& DetectorConstruction::getConfig() const {
    return m_config;
  }

  void DetectorConstruction::setConfig(Detectors::Config const& a_config) {
    m_config = a_config;
    return;
  }

  BaseVolume* DetectorConstruction::getDetector(
      Detectors::Config const& a_config) {
    uint8_t index = static_cast<uint8_t>(a_config);
    return m_detectors[index];
  }

  void DetectorConstruction::reloadGeometry() {
    G4cout << "Completely reloading geometry. This may take a moment...";
    // Get the right run manager
    if (GeneralManager::get().getMT()) {
      auto rman = G4MTRunManager::GetMasterRunManager();
      rman->ReinitializeGeometry(true);
    } else {
      auto rman = G4RunManager::GetRunManager();
      rman->ReinitializeGeometry(true);
    }
    // Clear out Solids, LVs, and PVs. Also calls GeometryHasBeenModified()
    // Note that ReinitializeGeometry() does not catch the optical borders
    G4LogicalSkinSurface::CleanSurfaceTable();
    G4LogicalBorderSurface::CleanSurfaceTable();

    // Unlink all the LVs and SDs - anything that needs to be relinked
    // should do so in its build() function
    m_linkLVSD.clear();

    // If we are in vis mode, we need to re-run /run/initialize and run
    // some /vis commands to get back to a clean state where we are showing
    // the geometry. In batch mode, the RunManager will be re-initialized
    // when /run/beamOn is called, so it's not necessary to call explicitly
    if (G4VVisManager::GetConcreteInstance()) {
      G4UImanager::GetUIpointer()->
        ApplyCommand("/run/initialize");
      G4UImanager::GetUIpointer()->
        ApplyCommand("/vis/drawVolume");
      G4UImanager::GetUIpointer()->
        ApplyCommand("/vis/scene/add/trajectories");
      G4UImanager::GetUIpointer()->
        ApplyCommand("/vis/scene/add/hits");
      // G4UImanager::GetUIpointer()->
      //   ApplyCommand("/vis/scene/add/axes 0 0 0 10 mm");
      G4UImanager::GetUIpointer()->
        ApplyCommand("/vis/scene/endOfEventAction/accumulate 100");
    }
    G4cout << "...geometry reload complete." << G4endl;
    return;
  }

  void DetectorConstruction::linkLVSD(G4String const& a_lvname,
                                      G4String const& a_sdname) {
    // Check if it's already there
    auto link = std::make_pair(a_lvname, a_sdname);
    if (std::find(m_linkLVSD.begin(), m_linkLVSD.end(), link)
        != m_linkLVSD.end()) {
      G4String msg = "The LV " + link.first + " and the SD " + link.second;
      msg += " are already linked.";
      G4Exception("DetectorConstruction::linkLVSD()", "MPNDWarning",
                  JustWarning, msg);
      return;
    }
    G4cout << "LV " << link.first << " linked to SD " << link.second
      << G4endl;
    m_linkLVSD.push_back(link);
    return;
  }

  void DetectorConstruction::unlinkLVSD(G4String const& a_lvname,
                                        G4String const& a_sdname) {
    auto link = std::make_pair(a_lvname, a_sdname);
    auto itr = std::find(m_linkLVSD.begin(), m_linkLVSD.end(), link);
    if (itr == m_linkLVSD.end()) {
      G4String msg = "The link between the LV " + link.first + " and the ";
      msg += "SD " + link.second + " is not in the list.";
      G4Exception("DetectorConstruction::unlinkLVSD()", "MPNDWarning",
                  JustWarning, msg);
      return;
    }
    G4cout << "LV " << (*itr).first << " unlinked from SD "
      << (*itr).second << G4endl;
    m_linkLVSD.erase(itr);
    return;
  }
}
