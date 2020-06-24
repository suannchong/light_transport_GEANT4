#ifndef _SDMESSENGER_H_
#define _SDMESSENGER_H_
// MPND
#include "uicmdwithtwodoublesandunit.hpp"
// Geant4
#include "G4UImessenger.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAString.hh"
// ROOT
// std

namespace MPND {
  class SensitiveDetector;
  /*! \class SDMessenger
   * Manages communication between the UI and the sensitive detectors.
   * This class should in principle be a member of the SensitiveDetectors
   * and therefore exist in each thread, but I could never get Geant4
   * to propagate it properly - commands only went to the first SD
   * registered via SDManager::AddNewDetector(). */
  class SDMessenger: public G4UImessenger {
    public:
      /*! Globally access the instance of this singleton. */
      static SDMessenger& get() {
        static SDMessenger manager;
        return manager;
      }
      /*! Destructor. */
      ~SDMessenger();
      /*! Copy constructor - deleted. */
      SDMessenger(SDMessenger const&) = delete;
      /*! Assignment operator - deleted. */
      void operator=(SDMessenger const&) = delete;
      /*! Add a sensitive detector to the list. */
      void addSD(SensitiveDetector* a_sd);
      /*! Get the list of SD names. */
      std::vector<G4String> getSDNames() const;
      /*! A G4UIcommand was issued; compare against the list of available
       * options, then parse the arguments if necessary. */
      void SetNewValue(G4UIcommand* a_cmd, G4String a_val);

    private:
      /*! Constructor. */
      SDMessenger();
      /*! Pointer to the SensitiveDetector that owns this messenger. */
      std::vector<SensitiveDetector*> m_sdList;
      /*! Currently selected SD name. */
      G4String m_currentSD;
      /*! The base directory: /mpnd/sd */
      G4UIdirectory* m_directory;
      /*! Select a sensitive detector by name (default = scintillator_sd).
       * */
      G4UIcmdWithAString* m_sdSelectCmd;
      /*! Add a particle to the particle filter for the selected SD. */
      G4UIcmdWithAString* m_particleAddCmd;
      /*! Remove a particle from the particle filter for the selected SD. */
      G4UIcmdWithAString* m_particleRemoveCmd;
      /*! Clear the particle filter for the selected SD. */
      G4UIcmdWithoutParameter* m_particleClearCmd;
      /*! Print the particle filter for the selected SD. */
      G4UIcmdWithoutParameter* m_particlePrintCmd;
      /*! Add an energy range to the energy filter for the selected SD. */
      UIcmdWithTwoDoublesAndUnit* m_energyAddCmd;
      /*! Remove an energy range from the energy filter for the selected
       * SD. Uses an index instead of having to put in the exact values -
       * use /mpnd/sd/energy/print to see the current filters and
       * their indices. */
      G4UIcmdWithAnInteger* m_energyRemoveCmd;
      /*! Clear the energy filter for the selected SD. */
      G4UIcmdWithoutParameter* m_energyClearCmd;
      /*! Print the energy filter for the selected SD. */
      G4UIcmdWithoutParameter* m_energyPrintCmd;
  };
}

#endif
