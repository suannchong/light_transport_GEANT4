// MPND
#include "geometrymessenger.hpp"
#include "singlepixeldetector.hpp"
#include "pixelarraydetector.hpp"
#include "detectorconstruction.hpp"
// Geant4
#include "G4UnitsTable.hh"
// ROOT
// std

namespace MPND {
  GeometryMessenger::GeometryMessenger(DetectorConstruction* a_dc):
    G4UImessenger(),
    // Note: do not try to use m_dc in the constructor here, because it
    // is not itself constructed yet!
    m_dc(a_dc)
  {
    G4cout << "Creating GeometryMessenger..." << G4endl;

    // Directory: /mpnd/geometry
    m_directory = new G4UIdirectory("/mpnd/geometry/");
    m_directory->SetGuidance("Toggle, move, or modify geometric objects.");

    // Reload geometry: /mpnd/geometry/reload
    m_reloadCmd = new G4UIcmdWithoutParameter("/mpnd/geometry/reload",
                                              this);
    m_reloadCmd->SetGuidance("Reload the geometry. Rebuilds based on "
                             "current values.");
    // This works after init, as long as we aren't running
    m_reloadCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    // Set the current detector config: /mpnd/geometry/config
    m_configCmd = new G4UIcmdWithAString("/mpnd/geometry/config", this);
    m_configCmd->SetGuidance("Detector configuration.");
    m_configCmd->SetParameterName("config", true);
    auto curr_type = Detectors::ConfigString[0];
    m_configCmd->SetDefaultValue(curr_type.c_str());
    m_configCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    // SinglePixel commands
    // /mpnd/geometry/pixelarray/dimensions
    m_singlepixelDimensionsCmd = new G4UIcmdWith3VectorAndUnit(
        "/mpnd/geometry/singlepixel/dimensions", this);
    m_singlepixelDimensionsCmd->SetGuidance("Set the pixel dimenions.");
    m_singlepixelDimensionsCmd->SetParameterName("x", "y", "z", true);
    m_singlepixelDimensionsCmd->SetDefaultUnit("mm");
    m_singlepixelDimensionsCmd->AvailableForStates(G4State_PreInit,
                                                   G4State_Idle);

    // Set the current detector scint_mat: /mpnd/geometry/singlepixel/scint_mat
    m_singlepixelScintMatCmd = new G4UIcmdWithAString(
        "/mpnd/geometry/singlepixel/scint_mat", this);
    m_singlepixelScintMatCmd->SetGuidance("Scintillator material.");
    m_singlepixelScintMatCmd->SetParameterName("scint_mat", true);
    m_singlepixelScintMatCmd->AvailableForStates(G4State_PreInit,
                                                 G4State_Idle);

    // PixelArray commands
    // /mpnd/geometry/pixelarray/dimensions
    m_pixelarrayDimensionsCmd = new G4UIcmdWith3VectorAndUnit(
        "/mpnd/geometry/pixelarray/dimensions", this);
    m_pixelarrayDimensionsCmd->SetGuidance("Set the pixel dimenions.");
    m_pixelarrayDimensionsCmd->SetParameterName("x", "y", "z", true);
    m_pixelarrayDimensionsCmd->SetDefaultUnit("mm");
    m_pixelarrayDimensionsCmd->AvailableForStates(G4State_PreInit,
                                                  G4State_Idle);
    // mpnd/geometry/pixelarray/nx
    m_pixelarrayNXCmd = new G4UIcmdWithAnInteger(
        "/mpnd/geometry/pixelarray/nx", this);
    m_pixelarrayNXCmd->SetGuidance("Set the number of X pixels.");
    m_pixelarrayNXCmd->SetParameterName("nx", true);
    m_pixelarrayNXCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    // mpnd/geometry/pixelarray/ny
    m_pixelarrayNYCmd = new G4UIcmdWithAnInteger(
        "/mpnd/geometry/pixelarray/ny", this);
    m_pixelarrayNYCmd->SetGuidance("Set the number of Y pixels.");
    m_pixelarrayNYCmd->SetParameterName("ny", true);
    m_pixelarrayNYCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    // Set the current detector scint_mat: /mpnd/geometry/pixelarray/scint_mat
    m_pixelarrayScintMatCmd = new G4UIcmdWithAString(
        "/mpnd/geometry/pixelarray/scint_mat", this);
    m_pixelarrayScintMatCmd->SetGuidance("Scintillator material.");
    m_pixelarrayScintMatCmd->SetParameterName("scint_mat", true);
    m_pixelarrayScintMatCmd->AvailableForStates(G4State_PreInit,
                                                 G4State_Idle);
  }

  /* Destructor. */
  GeometryMessenger::~GeometryMessenger() {
    G4cout << "Deleting GeometryMessenger..." << G4endl;
    delete m_directory;
    delete m_reloadCmd;
    delete m_configCmd;
    delete m_singlepixelDimensionsCmd;
    delete m_singlepixelScintMatCmd;
    delete m_pixelarrayScintMatCmd;
    delete m_pixelarrayDimensionsCmd;
    delete m_pixelarrayNXCmd;
    delete m_pixelarrayNYCmd;
  }

  /*! A command was issued by the user to modify the geometry. */
  void GeometryMessenger::SetNewValue(G4UIcommand* a_cmd, G4String a_val) {
    auto spdet = (SinglePixelDetector*)
                 m_dc->getDetector(Detectors::Config::singlepixel);
    auto padet = (PixelArrayDetector*)
                 m_dc->getDetector(Detectors::Config::pixelarray);
    // Reload geometry: /mpnd/geometry/reload
    if (a_cmd == m_reloadCmd) {
      m_dc->reloadGeometry();
    // Set the current geometry type: /svsc/geometry/config
    } else if (a_cmd == m_configCmd) {
      bool found = false;
      // Check if the provided argument is in the list
      for (size_t i=0;i < Detectors::NConfigs;++i) {
        a_val.toLower();
        if (a_val == Detectors::ConfigString[i].c_str()) {
          m_dc->setConfig(static_cast<Detectors::Config>(i));
          found = true;
          G4cout << "Detector config set to " << a_val << G4endl;
        }
      }
      if (!found) {
        G4cout << "Invalid detector config provided. Available configs: ";
        for (size_t i=0;i < Detectors::NConfigs;++i) {
          G4cout << Detectors::ConfigString[i] << " ";
        }
        G4cout << G4endl;
      }
      // /mpnd/geometry/singlepixel/scint_mat
     } else if (a_cmd == m_singlepixelScintMatCmd) {
       bool found = false;
       // Check if the provided argument is in the list
       for (int i=0;i < Material::NMATERIALS;++i) {
         a_val.toLower();
         if (a_val == Material::TypeString[i].c_str()) {
           spdet->setMaterial(static_cast<Material::Type>(i));
           found = true;
           G4cout << "Scintillator material set to " << a_val << G4endl;
         }
       }
       if (!found) {
         G4cout << "Invalid material provided. Available materials: ";
         for (size_t i=0;i < Material::NMATERIALS;++i) {
           G4cout << Material::TypeString[i] << " ";
         }
         G4cout << G4endl;
       }
       // /mpnd/geometry/pixelarray/scint_mat
     } else if (a_cmd == m_pixelarrayScintMatCmd){
       bool found = false;
       // Check if the provided argument is in the list
       for (int i = 0; i < Material::NMATERIALS; ++i){
         a_val.toLower();
         if (a_val == Material::TypeString[i].c_str()){
           spdet->setMaterial(static_cast<Material::Type>(i));
           found = true;
           G4cout << "Scintillator material set to " << a_val << G4endl;
         }
       }
       if (!found){
         G4cout << "Invalid material provided. Available materials: ";
         for (size_t i = 0; i < Material::NMATERIALS; ++i){
           G4cout << Material::TypeString[i] << " ";
         }
         G4cout << G4endl;
       }
      // /mpnd/geometry/pixelarray/dimensions
     } else if (a_cmd == m_singlepixelDimensionsCmd){
       auto dim = m_singlepixelDimensionsCmd->GetNew3VectorValue(a_val);
       spdet->setDimensions(dim);
       G4cout << "SinglePixelDetector dimensions set to "
              << G4BestUnit(dim, "Length") << G4endl;
       // /mpnd/geometry/pixelarray/dimensions
     } else if (a_cmd == m_pixelarrayDimensionsCmd){
       auto dim = m_pixelarrayDimensionsCmd->GetNew3VectorValue(a_val);
       padet->setDimensions(dim);
       G4cout << "PixelArrayDetector dimensions set to "
              << G4BestUnit(dim, "Length") << G4endl;
       // /mpnd/geometry/pixelarray/nx
     } else if (a_cmd == m_pixelarrayNXCmd) {
       auto nx = m_pixelarrayNXCmd->GetNewIntValue(a_val);
       if (nx < 1){
         G4cout << "ERROR: array size must be >= 1." << G4endl;
       }else{
         padet->setNX(nx);
         G4cout << "PixelArrayDetector set to " << nx << " X pixels."
                << G4endl;
       }
       // /mpnd/geometry/pixelarray/ny
     } else if (a_cmd == m_pixelarrayNYCmd) {
       auto ny = m_pixelarrayNYCmd->GetNewIntValue(a_val);
       if (ny < 1){
         G4cout << "ERROR: array size must be >= 1." << G4endl;
       }else {
         padet->setNY(ny);
         G4cout << "PixelArrayDetector set to " << ny << " Y pixels."
                << G4endl;
       }
     }
    // Command did not match, do nothing
    return;
  }
}
