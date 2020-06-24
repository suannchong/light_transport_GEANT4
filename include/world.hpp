#ifndef _WORLD_H_
#define _WORLD_H_
// MPND
#include "basevolume.hpp"
// Geant4
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolume.hh"
// ROOT
// std

namespace MPND {
  /*! \class World
   * The experimental world. This is the top-level volume in the
   * simulation. */
  class World: public BaseVolume {
    public:
      /*! Constructor. */
      World(G4String const& a_name);
      /*! Destructor. */
      ~World();

      // /*! Get the logical volume. */
      // G4LogicalVolume* getLogical() const;
      // /*! Get the physical volume. */
      // G4PVPlacement* getPhysical() const;
      /*! Set a safe default size and position - 2x2x2 m3 and (0, 0, 0). */
      void setDefaults();
      /*! Build this volume. No outer (mother) volume, due to being the
       * world. */
      void build(BaseVolume* a_lv = nullptr);
      // void build(G4LogicalVolume* a_lv = nullptr);

    private:
      /*! The experimental world is a box. */
      G4Box* m_solid;
      // /*! World logical volume. */
      // G4LogicalVolume* m_logical;
      // /*! World physical volume. */
      // G4PVPlacement* m_physical;
  };
}

#endif
