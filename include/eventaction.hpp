#ifndef _EVENTACTION_H_
#define _EVENTACTION_H_
// MPND
// Geant4
#include "G4UserEventAction.hh"
#include "G4Event.hh"
// ROOT
// std

namespace MPND {
  /*! \class EventAction
   * Controls what happens at the beginning and end of event generation.
   * Only used to report event number - SD hits collection processing is
   * performed by Run::RecordEvent(). */
  class EventAction: public G4UserEventAction {
    public:
      /*! Constructor. */
      EventAction();
      /*! Destructor. */
      ~EventAction();

      /*! Actions taken immediately after a G4Event is generated. */
      void BeginOfEventAction(G4Event const*);
      /*! Actions taken after the initial particle(s) and all of their
       * children have died. */
      void EndOfEventAction(G4Event const* a_event);
  };
}

#endif
