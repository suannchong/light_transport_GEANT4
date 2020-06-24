#ifndef _GEOMETRYMESSENGER_H_
#define _GEOMETRYMESSENGER_H_
// MPND
//#include "detectorconstruction.hpp"
// Geant4
#include "G4UImessenger.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
// ROOT
// std

namespace MPND {
class DetectorConstruction;
  /*! \class GeometryMessenger
   * Manages communication between the UI and the geometry. */
  class GeometryMessenger: public G4UImessenger {
    public:
      /*! Constructor. */
      GeometryMessenger(DetectorConstruction* a_dc);
      /*! Destructor. */
      ~GeometryMessenger();
      /*! A G4UIcommand was issued; compare against the list of available
       * options, then parse the arguments if necessary. */
      void SetNewValue(G4UIcommand* a_cmd, G4String a_val);

    private:
      /*! Pointer to the DetectorConstruction. */
      DetectorConstruction* m_dc;
      /*! The base directory: /mpnd/geometry */
      G4UIdirectory* m_directory;
      /*! Reload command: /mpnd/geometry/reload */
      G4UIcmdWithoutParameter* m_reloadCmd;
      /*! Change detector config: /mpnd/geometry/config [arg] */
      G4UIcmdWithAString* m_configCmd;

      /*! SinglePixelDetector - set the pixel dimensions. */
      G4UIcmdWith3VectorAndUnit* m_singlepixelDimensionsCmd;
      /*! SinglePixelDetector - set the scintillator material. */
      G4UIcmdWithAString *m_singlepixelScintMatCmd;

      /*! PixelArrayDetector - set the pixel dimensions. */
      G4UIcmdWith3VectorAndUnit* m_pixelarrayDimensionsCmd;
      /*! PixelArrayDetector - set the number of X pixels. */
      G4UIcmdWithAnInteger* m_pixelarrayNXCmd;
      /*! PixelArrayDetector - set the number of Y pixels. */
      G4UIcmdWithAnInteger* m_pixelarrayNYCmd;
      /*! PixelArrayDetector - set the scintillator material */
      G4UIcmdWithAString *m_pixelarrayScintMatCmd;
      
  };
}

#endif
