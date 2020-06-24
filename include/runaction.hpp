#ifndef _RUNACTION_H_
#define _RUNACTION_H_
// MPND
// Geant4
#include "G4UserRunAction.hh"
#include "G4Run.hh"
// ROOT
#include "TStopwatch.h"
// std

namespace MPND {
  /* \class RunAction
   * Executes actions at the beginning and end of a run, and generates
   * Run objects. */
  class RunAction: public G4UserRunAction {
    public:
      /*! Constructor. */
      RunAction();
      /*! Destructor. */
      ~RunAction();

      /*! Generate a new G4Run-based run object for Geant4. */
      G4Run* GenerateRun();
      /*! Action taken immediately after the run is allocated, before
       * events are generated. */
      void BeginOfRunAction(G4Run const* a_run);
      /*! Action taken after a run is finished - the specified number of
       * events were generated and all particles tracked to death. */
      void EndOfRunAction(G4Run const* a_run);

    private:
      /*! Run timer. */
      TStopwatch m_timer;
  };
}

#endif
