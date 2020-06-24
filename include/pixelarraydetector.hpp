#ifndef _PIXELARRAYDETECTOR_H_
#define _PIXELARRAYDETECTOR_H_
// MPND
#include "basevolume.hpp"
#include "world.hpp"
#include "materialsmanager.hpp"
// Geant4
// ROOT
// std

namespace MPND {
  /*! \class PixelArrayDetector
   * Holds/assembles an array of pixels. */
  class PixelArrayDetector: public BaseVolume {
    public:
      /*! Constructor. */
      PixelArrayDetector(G4String const& a_name);
      /*! Destructor. */
      ~PixelArrayDetector();
      
      /*! Get the number of X pixels. */
      int const& getNX() const;
      /*! Set the number of X pixels. */
      void setNX(int const& a_nx);
      /*! Get the number of Y pixels. */
      int const& getNY() const;
      /*! Set the number of Y pixels. */
      void setNY(int const& a_ny);
      /*! Set defaults. 8x8 array of 6x6x10 mm pixels. */
      void setDefaults();
      /*! Build the assembly. */
      void build(BaseVolume* a_mother);
      // void build(G4LogicalVolume* a_mother_log);
      /*! Set the material type */
      void setMaterial(Material::Type const& a_mat);

    private:
      /*! Number of pixels in the X dimension. */
      int m_nX;
      /*! Number of pixels in the Y dimension. */
      int m_nY;
      /*! Scintillator material type for this detector */
      Material::Type m_material;
  };
}

#endif
