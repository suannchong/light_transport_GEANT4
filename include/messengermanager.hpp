#ifndef _MESSENGERMANAGER_H_
#define _MESSENGERMANAGER_H_
// MPND
// Geant4
#include "G4UImessenger.hh"
// ROOT
// std

namespace MPND {
  /*! \class MessengerManager
   * Manages all Messenger classes, from allocation to deletion. This
   * singleton is initialized early in the runtime to ensure commands
   * are available as soon as it starts up. */
  class MessengerManager {
    public:
      /*! Globally access the instance of this singleton. */
      static MessengerManager& get() {
        static MessengerManager manager;
        return manager;
      }
      /*! Destructor. */
      ~MessengerManager();
      /*! Copy constructor - deleted. */
      MessengerManager(MessengerManager const&) = delete;
      /*! Assignment operator - deleted. */
      void operator=(MessengerManager const&) = delete;

    private:
      /*! Constructor. */
      MessengerManager();
      /*! Vector of messengers. */
      std::vector<G4UImessenger*> m_messengers;
  };
}

#endif
