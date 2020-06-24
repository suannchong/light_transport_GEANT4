#ifndef _ACTIONINITIALIZATION_H_
#define _ACTIONINITIALIZATION_H_
// MPND
// Geant4
#include "G4VUserActionInitialization.hh"
// ROOT
// std

namespace MPND {
  /*! \class ActionInitialization
   * Initializes all user actions. Necessary for multithreaded Geant4. */
  class ActionInitialization: public G4VUserActionInitialization {
    public:
      /*! Constructor. */
      ActionInitialization();
      /*! Destructor. */
      ~ActionInitialization();

      /*! Create the action classes in sequential mode. */
      void Build() const;
      /*! Create the action classes in multithreaded mode. */
      void BuildForMaster() const;
  };
}

#endif
