// SVSC
#include "runmessenger.hpp"
#include "filemanager.hpp"
// Geant4
// ROOT
// std

namespace MPND {
  RunMessenger::RunMessenger():
    G4UImessenger()
  {
    // Directory: /mpnd/run
    m_directory = new G4UIdirectory("/mpnd/run/");
    m_directory->SetGuidance("Change parameters of the run.");

    // Set the output file path: /mpnd/run/path
    auto& fman = FileManager::get();
    m_pathCmd = new G4UIcmdWithAString("/mpnd/run/path", this);
    m_pathCmd->SetGuidance("Output file path.");
    m_pathCmd->SetParameterName("path", true);
    m_pathCmd->SetDefaultValue(fman.getPath().c_str());
    m_pathCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    // Toggle data saving: /mpnd/run/write_data
    m_writeDataCmd = new G4UIcmdWithABool("/mpnd/run/write_data", this);
    m_writeDataCmd->SetGuidance("Toggle writing data to file.");
    m_writeDataCmd->SetParameterName("write_data", true);
    m_writeDataCmd->SetDefaultValue(fman.getWriteData());
    m_writeDataCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    // Set the buffer limit: /mpnd/run/buffer_limit
    m_bufferLimitCmd = new G4UIcmdWithAnInteger("/mpnd/run/buffer_limit",
                                              this);
    m_bufferLimitCmd->SetGuidance("Max number of events to store in "
                                  "FileManager buffer before writing.");
    m_bufferLimitCmd->SetParameterName("buffer_limit", true);
    m_bufferLimitCmd->SetDefaultValue(fman.getBufferLimit());
    m_bufferLimitCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    // Set TTree::SetMaxTreeSize(): /mpnd/run/max_file_size
    m_maxFileSizeCmd = new G4UIcmdWithAnInteger("/mpnd/run/max_file_size",
                                                this);
    m_maxFileSizeCmd->SetGuidance("Max file size in GB.");
    m_maxFileSizeCmd->SetParameterName("max_file_size", true);
    m_maxFileSizeCmd->SetDefaultValue(fman.getMaxFileSize());
    m_maxFileSizeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  }

  RunMessenger::~RunMessenger() {
    delete m_directory;
    delete m_pathCmd;
    delete m_writeDataCmd;
    delete m_bufferLimitCmd;
    delete m_maxFileSizeCmd;
  }

  void RunMessenger::SetNewValue(G4UIcommand* a_cmd, G4String a_val) {
    auto& fman = FileManager::get();
    if (a_cmd == m_pathCmd) {
      fman.setPath(a_val);
      G4cout << "Output file path set to " << a_val << G4endl;
    } else if (a_cmd == m_writeDataCmd) {
      auto val = m_writeDataCmd->GetNewBoolValue(a_val);
      fman.setWriteData(val);
      G4cout << "Write data set to " << (val ? "true" : "false") << G4endl;
    } else if (a_cmd == m_bufferLimitCmd) {
      auto val = m_bufferLimitCmd->GetNewIntValue(a_val);
      if (val <= 0) {
        G4cout << "Buffer limit must be > 0." << G4endl;
      } else {
        fman.setBufferLimit((std::uint64_t)val);
        G4cout << "Buffer limit set to " << val << G4endl;
      }
    } else if (a_cmd == m_maxFileSizeCmd) {
      auto val = m_maxFileSizeCmd->GetNewIntValue(a_val);
      if (val <= 0) {
        G4cout << "Maximum file size must be > 0." << G4endl;
      } else {
        fman.setMaxFileSize(val);
        G4cout << "Maximum file size set to " << val << " GB" << G4endl;
      }
    } // Command did not match, do nothing
    return;
  }
}
