// MPND
#include "sdmessenger.hpp"
#include "sensitivedetector.hpp"
// Geant4
#include "G4UnitsTable.hh"
// ROOT
// std

namespace MPND {
  SDMessenger::SDMessenger():
    G4UImessenger(),
    m_sdList(),
    m_currentSD("scintillator_sd")
  {
    G4cout << "Creating SDMessenger..." << G4endl;

    // Directory: /mpnd/sd
    m_directory = new G4UIdirectory("/mpnd/sd/");
    m_directory->SetGuidance("Modify sensitive detector filters.");

    // Select a gun: /mpnd/sd/select_sd
    m_sdSelectCmd = new G4UIcmdWithAString("/mpnd/sd/select_sd",
                                           this);
    m_sdSelectCmd->SetGuidance("Change selected SD.");
    m_sdSelectCmd->SetParameterName("current_sd", true);
    m_sdSelectCmd->SetDefaultValue("/scintillator_sd");
    // This works after init, as long as we aren't running
    m_sdSelectCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    // Particle filter commands
    // Add a particle to the filter: /mpnd/sd/particle/add
    m_particleAddCmd = new G4UIcmdWithAString(
        "/mpnd/sd/particle/add", this);
    m_particleAddCmd->SetGuidance("Add a particle to the filter.");
    m_particleAddCmd->SetParameterName("particle", true);
    m_particleAddCmd->SetDefaultValue("");
    m_particleAddCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    // Remove a particle from the filter: /mpnd/sd/particle/remove
    m_particleRemoveCmd = new G4UIcmdWithAString(
        "/mpnd/sd/particle/remove", this);
    m_particleRemoveCmd->SetGuidance("Remove a particle from the filter.");
    m_particleRemoveCmd->SetParameterName("particle", true);
    m_particleRemoveCmd->SetDefaultValue("");
    m_particleRemoveCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    // Clear the particle filter: /mpnd/sd/particle/clear
    m_particleClearCmd = new G4UIcmdWithoutParameter(
        "/mpnd/sd/particle/clear", this);
    m_particleClearCmd->SetGuidance("Clear the particle filter.");
    m_particleClearCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    // Print the particles in the filter: /mpnd/sd/particle/print
    m_particlePrintCmd = new G4UIcmdWithoutParameter(
        "/mpnd/sd/particle/print", this);
    m_particlePrintCmd->SetGuidance("Print the particle filter.");
    m_particlePrintCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    // Energy filter commands
    // Add an energy range to the filter: /mpnd/sd/energy/add
    m_energyAddCmd = new UIcmdWithTwoDoublesAndUnit(
        "/mpnd/sd/energy/add", this);
    m_energyAddCmd->SetGuidance("Add an energy window to the filter.");
    m_energyAddCmd->SetParameterName("min", "max", true);
    m_energyAddCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    // Remove an energy range from the filter:
    // /mpnd/sd/energy/remove
    m_energyRemoveCmd = new G4UIcmdWithAnInteger(
        "/mpnd/sd/energy/remove", this);
    m_energyRemoveCmd->SetGuidance("Remove an energy window from the "
                                   "filter.");
    m_energyRemoveCmd->SetParameterName("index", true);
    m_energyRemoveCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    // Clear the energy filter: /mpnd/sd/energy/clear
    m_energyClearCmd = new G4UIcmdWithoutParameter(
        "/mpnd/sd/energy/clear", this);
    m_energyClearCmd->SetGuidance("Clear the energy filter.");
    m_energyClearCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    // Print the energys in the filter: /mpnd/sd/energy/print
    m_energyPrintCmd = new G4UIcmdWithoutParameter(
        "/mpnd/sd/energy/print", this);
    m_energyPrintCmd->SetGuidance("Print the energy filter.");
    m_energyPrintCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  }

  /* Destructor. */
  SDMessenger::~SDMessenger() {
    G4cout << "Deleting SDMessenger..." << G4endl;
    delete m_directory;
    delete m_sdSelectCmd;
    delete m_particleAddCmd;
    delete m_particleRemoveCmd;
    delete m_particleClearCmd;
    delete m_particlePrintCmd;
    delete m_energyAddCmd;
    delete m_energyRemoveCmd;
    delete m_energyClearCmd;
    delete m_energyPrintCmd;
  }

  void SDMessenger::addSD(SensitiveDetector* a_sd) {
    if (std::find(m_sdList.begin(), m_sdList.end(), a_sd)
        != m_sdList.end()) {
      G4String msg = "Tried to add " + a_sd->GetName() + " to SDMessenger";
      msg += " SD list, but it is already on it.";
      G4Exception("SDMessenger::addSD()", "MPNDWarning", JustWarning,
          msg);
      return;
    }
    m_sdList.push_back(a_sd);
    return;
  }

  std::vector<G4String> SDMessenger::getSDNames() const {
    // Loop over SD list and add unique names to a new list
    std::vector<G4String> sd_names;
    for (auto& sd : m_sdList) {
      if (std::find(sd_names.begin(), sd_names.end(), sd->GetName())
          == sd_names.end()) {
        sd_names.push_back(sd->GetName());
      }
    }
    return sd_names;
  }

  void SDMessenger::SetNewValue(G4UIcommand* a_cmd, G4String a_val) {
    // /mpnd/sd/select_sd
    if (a_cmd == m_sdSelectCmd) {
      bool found = false;
      for (auto& sd : m_sdList) {
        auto sd_name = sd->GetName();
        if (sd_name == a_val) {
          found = true;
          break;
        }
      }
      if (!found) {
        G4String msg = "There is no SD with the name " + a_val;
        msg += " registered with the SDMessenger.";
        G4Exception("/mpnd/sd/select_sd", "SD Not Found", JustWarning,
            msg);
        return;
      }
      m_currentSD = a_val;
      G4cout << "Current SD is " << a_val << " selected" << G4endl;
    }

    // Loop over SDs and apply command (if applicable to that SD)
    bool first_loop = true;
    for (auto& sd : m_sdList) {
      auto sd_name = sd->GetName();
      if (sd_name == m_currentSD) {
        // /mpnd/sd/particle/add
        if (a_cmd == m_particleAddCmd) {
          auto pf = static_cast<ParticleFilter*>(sd->getFilter(
                Filters::Type::particle));
          if (!pf) {
            G4Exception("Filter Does Not Exist", "/mpnd/sd/particle/add",
                        JustWarning, "That SD does not have that type of "
                        "filter. You must allocate it in "
                        "DetectorConstruction::ConstructSDandField().");
            return;
          }
          pf->add(a_val);
        // /mpnd/sd/particle/remove
        } else if (a_cmd == m_particleRemoveCmd) {
          auto pf = static_cast<ParticleFilter*>(sd->getFilter(
                Filters::Type::particle));
          if (!pf) {
            G4Exception("Filter Does Not Exist", "/mpnd/sd/particle/remove",
                        JustWarning, "That SD does not have that type of "
                        "filter. You must allocate it in "
                        "DetectorConstruction::ConstructSDandField().");
            return;
          }
          pf->remove(a_val);
        // /mpnd/sd/particle/clear
        } else if (a_cmd == m_particleClearCmd) {
          auto pf = static_cast<ParticleFilter*>(sd->getFilter(
                Filters::Type::particle));
          if (!pf) {
            G4Exception("Filter Does Not Exist", "/mpnd/sd/particle/clear",
                        JustWarning, "That SD does not have that type of "
                        "filter. You must allocate it in "
                        "DetectorConstruction::ConstructSDandField().");
            return;
          }
          pf->clear();
        // /mpnd/sd/particle/print
        } else if (a_cmd == m_particlePrintCmd) {
          auto pf = static_cast<ParticleFilter*>(sd->getFilter(
                Filters::Type::particle));
          if (!pf) {
            G4Exception("Filter Does Not Exist", "/mpnd/sd/particle/print",
                        JustWarning, "That SD does not have that type of "
                        "filter. You must allocate it in "
                        "DetectorConstruction::ConstructSDandField().");
            return;
          }
          // Only print the first one, to avoid spamming the user when
          // there are multiple threads
          if (first_loop) {
            G4cout << sd->GetName() << " allowed particles: "
              << pf->print() << G4endl;
            first_loop = false;
          }
        // /mpnd/sd/energy/add
        } else if (a_cmd == m_energyAddCmd) {
          auto ef = static_cast<EnergyFilter*>(sd->getFilter(
                Filters::Type::energy));
          if (!ef) {
            G4Exception("Filter Does Not Exist", "/mpnd/sd/energy/add",
                        JustWarning, "That SD does not have that type of "
                        "filter. You must allocate it in "
                        "DetectorConstruction::ConstructSDandField().");
            return;
          } else {
            ef->add(m_energyAddCmd->GetNewDoublePair(a_val));
          }
        // /mpnd/sd/energy/remove
        } else if (a_cmd == m_energyRemoveCmd) {
          auto ef = static_cast<EnergyFilter*>(sd->getFilter(
                Filters::Type::energy));
          if (!ef) {
            G4Exception("Filter Does Not Exist", "/mpnd/sd/energy/remove",
                        JustWarning, "That SD does not have that type of "
                        "filter. You must allocate it in "
                        "DetectorConstruction::ConstructSDandField().");
            return;
          } else {
            ef->remove(m_energyRemoveCmd->GetNewIntValue(a_val));
          }
        // /mpnd/sd/energy/clear
        } else if (a_cmd == m_energyClearCmd) {
          auto ef = static_cast<EnergyFilter*>(sd->getFilter(
                Filters::Type::energy));
          if (!ef) {
            G4Exception("Filter Does Not Exist", "/mpnd/sd/energy/clear",
                        JustWarning, "That SD does not have that type of "
                        "filter. You must allocate it in "
                        "DetectorConstruction::ConstructSDandField().");
            return;
          } else {
            ef->clear();
          }
        // /mpnd/sd/energy/print
        } else if (a_cmd == m_energyPrintCmd) {
          auto ef = static_cast<EnergyFilter*>(sd->getFilter(
              Filters::Type::energy));
          if (!ef) {
            G4Exception("Filter Does Not Exist", "/mpnd/sd/energy/print",
                        JustWarning, "That SD does not have that type of "
                        "filter. You must allocate it in "
                        "DetectorConstruction::ConstructSDandField().");
            return;
          } else if (ef->size() == 0) {
            if (first_loop) {
              G4cout << "No energy ranges allowed - nothing is being "
                << "tracked in this SD." << G4endl;
              first_loop = false;
            }
          } else {
            if (first_loop) {
              G4cout << "Energy ranges of hits being tracked in "
                << m_currentSD << ":\n";
              for (std::size_t i=0;i < ef->size();++i) {
                G4cout << i << ": " << ef->printRange(i) << G4endl;
              }
              first_loop = false;
            }
          }
        }
      }
    }
    return;
  }
}
