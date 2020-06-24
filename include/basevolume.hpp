#ifndef _BASEVOLUME_H_
#define _BASEVOLUME_H_
// MPND
// Geant4
#include "G4String.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
// ROOT
// std

namespace MPND {
  /*! \class BaseVolume
   * Base class for volumetric objects. These objects all have the common
   * properties of: name, position (3D), dimensions (3D), rotation, a
   * material, and visual attributes. Inheritors must override
   * setDefaults(), which will set sane/physical default values for the
   * object, and build(), which takes the mother G4LogicalVolume as an
   * argument - for "top level" objects in the simulation, this is
   * typically the World logical volume. */
  class BaseVolume {
    public:
      /*! Constructor. */
      BaseVolume(G4String const& a_name);
      /*! Destructor. */
      virtual ~BaseVolume();
    
      /*! Get the base name of the object. */
      G4String const& getName() const;
      /*! Get the G4ThreeVector position of the object. */
      G4ThreeVector const& getPosition() const;
      /*! Set the G4ThreeVector position of the object. */
      void setPosition(G4ThreeVector const& a_pos);
      /*! Set the G4ThreeVector position of the object. */
      void setPosition(double const& a_x,
                       double const& a_y,
                       double const& a_z);
      /*! Get the G4ThreeVector dimensions of the object. */
      G4ThreeVector const& getDimensions() const;
      /*! Set the G4ThreeVector dimensions of the object. */
      void setDimensions(G4ThreeVector const& a_dim);
      /*! Set the G4ThreeVector dimensions of the object. */
      void setDimensions(double const& a_x,
                         double const& a_y,
                         double const& a_z);

      /*! Set sane/physical defaults. Must be overridden by inheritors
       * of this object. */
      virtual void setDefaults() = 0;
      /*! Build this volume by allocating a G4Solid, G4LogicalVolume, and
       * G4PVPlacement. Must be overridden by inheritors of this object.
       * */
      virtual void build(BaseVolume* a_mother) = 0;
      // virtual void build(G4LogicalVolume* a_mother_log) = 0;
      /*! Get the logical volume. */
      G4LogicalVolume *getLogical() const;
      /*! Get the physical volume. */
      G4PVPlacement *getPhysical() const;

    protected:
      /*! Base name for the volume. Solids, logical volumes, and
       *  PVPlacements will append to this name, ie. "<vname>_log" and
       *  "<vname>_phys". */
      G4String m_name;
      /*! Position of the center of this object, relative to the center of
       *  its mother (= absolute position if the mother is the World). */
      G4ThreeVector m_position;
      /*! Dimensions of this object, assuming 3 values is adequate to
       *  describe it. */
      G4ThreeVector m_dimensions;
      /*! Geant4 visual attributes, for drawing. */
      G4VisAttributes* m_vis;
      /*! Base Volume logical volume. */
      G4LogicalVolume* m_logical;
      /*! Base Volume physical volume. */
      G4PVPlacement* m_physical;
  };
}

#endif
