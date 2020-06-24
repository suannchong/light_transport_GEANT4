#ifndef _RUNMESSENGER_H_
#define _RUNMESSENGER_H_
// MPND
// Geant4
#include "G4UImessenger.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
// ROOT
// std

namespace MPND {
  /*! \class RunMessenger
   * Manages communication between the UI and the FileManager. */
  class RunMessenger: public G4UImessenger {
    public:
      /*! Constructor. */
      RunMessenger();
      /*! Destructor. */
      ~RunMessenger();

      /*! A G4UIcommand was issued; compare against the list of available
       * options, then parse the arguments if necessary. */
      void SetNewValue(G4UIcommand* a_cmd, G4String a_val);

    private:
      /*! The base directory: /mpnd/geometry */
      G4UIdirectory* m_directory;
      /*! Set the output file path. */
      G4UIcmdWithAString* m_pathCmd;
      /*! Toggle whether or not data is written to disk. */
      G4UIcmdWithABool* m_writeDataCmd;
      /*! Set the buffer limit. */
      G4UIcmdWithAnInteger* m_bufferLimitCmd;
      /*! Set the max file size. */
      G4UIcmdWithAnInteger* m_maxFileSizeCmd;
  };
}

#endif
