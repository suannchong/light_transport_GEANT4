#ifndef _SINGLEPIXELDETECTOR_H_
#define _SINGLEPIXELDETECTOR_H_
// MPND
#include "basevolume.hpp"
#include "world.hpp"
#include "materialsmanager.hpp"
// Geant4
// ROOT
// std

namespace MPND {
  /*! \class SinglePixelDetector
   * Holds/assembles the single pixel detector geometry components. */
  class SinglePixelDetector: public BaseVolume {
    public:
      /*! Constructor. */
      SinglePixelDetector(G4String const& a_name);
      /*! Destructor. */
      ~SinglePixelDetector();

      /*! Set defaults. A 6x6x10 mm pixel. */
      void setDefaults();
      /*! Build the assembly. */
      void build(BaseVolume* a_mother);
      // void build(G4LogicalVolume* a_mother_log);
      /*! Set the material type.*/
      void setMaterial(Material::Type const& a_mat);

    private:
      /*! Scintillator material type for this detector */
      Material::Type m_material;
  };
}

#endif
