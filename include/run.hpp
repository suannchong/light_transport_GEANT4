#ifndef _RUN_H_
#define _RUN_H_
// MPND
// Geant4
#include "G4Run.hh"
// ROOT
// std

namespace MPND {
  /*! \class Run
   * Thread-safe Run object. RecordEvent is used like
   * EventAction::EndOfEventAction() in sequential code. Merge() is not
   * used - we instead have our own FileManager. */
  class Run: public G4Run {
    public:
      /*! Constructor. */
      Run();
      /*! Denstructor. */
      ~Run();

      /*! Record an event. */
      void RecordEvent(G4Event const* a_event);
      /*! Merge the provided run object into this one. */
      void Merge(G4Run const* a_run);
  };
}

#endif
