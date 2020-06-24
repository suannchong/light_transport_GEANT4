#ifndef _PHYSICSLIST_H_
#define _PHYSICSLIST_H_
// MPND
// Geant4
#include "G4VModularPhysicsList.hh"
// ROOT
// std

namespace MPND {
  /*! \class PhysicsList
   * Main physics list for the simulation. Extends the standard
   * QGSP_BERT_HP physics list with optical/scintillation physics. */
  class PhysicsList: public G4VModularPhysicsList {
    public:
      /*! Constructor. */
      PhysicsList(int const& a_verbose = 0);
      /*! Destructor. */
      ~PhysicsList();
  };
}

#endif
