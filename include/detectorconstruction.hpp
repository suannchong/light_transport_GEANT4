#ifndef _DETECTORCONSTRUCTION_H_
#define _DETECTORCONSTRUCTION_H_
// MPND
#include "world.hpp"
#include "mpndglobals.hpp"
// Geant4
#include "G4VUserDetectorConstruction.hh"
// ROOT
// std

namespace MPND {
  class GeometryMessenger;
  /*! \class DetectorConstruction
   * Responsible for serving Geant4 the top-level physical volume in
   * the simulation via the Construct() function. ConstructSDandField()
   * is additionally used for multithreaded support. */
  class DetectorConstruction: public G4VUserDetectorConstruction {
    public:
      /*! Constructor. */
      DetectorConstruction();
      /*! Destructor. */
      ~DetectorConstruction();
      /*! Build the geometry. Calls build() on the selected *Detector
       * object. */
      G4VPhysicalVolume* Construct() override;
      /*! Construct sensitive detectors. This is called for every worker
       * thread generated - so each thread gets its own copy of the
       * sensitive detectors even though the geometric objects are shared.
       * */
      void ConstructSDandField() override;
      /*! Get the currently-selected geometry config. */
      Detectors::Config const& getConfig() const;
      /*! Set the currently-selected geometry config. */
      void setConfig(Detectors::Config const& a_config);
      /* Get the pointer to a specified geometry. */
      BaseVolume* getDetector(Detectors::Config const& a_config);
      /*! Completely clears out the geometry (deletes shapes, logical
       * volumes, etc) and forces the RunManager to reinitialize the
       * geometry, calling ConstructSDandField() from the correct
       * threads. Also issues visualization commands, if applicable, to
       * reload the scene. */
      void reloadGeometry();
      /*! Connect a logical volume to a sensitive detector. */
      void linkLVSD(G4String const& a_lvname, G4String const& a_sdname);
      /*! Disconnect a logical volume from a sensitive detector. */
      void unlinkLVSD(G4String const& a_lvname, G4String const& a_sdname);

    private:
      /*! Flag for whether the SD memory has been allocated. There is one
       * for each thread. */
      std::vector<bool> m_fSDAllocated;
      /*! Pointer to the top level (world) volume. */
      World* m_world;
      /*! Which geometry is selected. */
      Detectors::Config m_config;
      /*! List of *Detector objects. */
      std::array<BaseVolume*, Detectors::NConfigs> m_detectors;
      /*! Messenger for the geometry commands. */
      GeometryMessenger* m_messenger;
      /*! Links between logical volumes and sensitive detectors. These are
       * connected by calling SetSensitiveDetector on a per-thread basis
       * inside of ConstructSDandField. first=LV, second=SD names. */
      std::vector<std::pair<G4String, G4String>> m_linkLVSD;
  };
}

#endif
