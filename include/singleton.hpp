#ifndef _SINGLETON_H_
#define _SINGLETON_H_
// MPND
// Geant4
// ROOT
// std

namespace MPND {
  /*! \namespace Singleton
   * Contains a single function to instantiate all singletons. Should be
   * called at the start of the program. */
  namespace Singleton {
    /*! Calls get() on every singleton in the program so an instance is
     * created and it gets initialized. Call at the very beginning of the
     * program. */
    void initialize();
  }
}

#endif
