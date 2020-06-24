// MPND
#include "messengermanager.hpp"
#include "runmessenger.hpp"
// Geant4
// ROOT
// std

namespace MPND {
  /*! Constructor. */
  MessengerManager::MessengerManager() {
    // Allocate messengers
    m_messengers.push_back(new RunMessenger);
  }

  /*! Destructor. */
  MessengerManager::~MessengerManager() {
    // Clean up
    for (auto& m : m_messengers) {
      delete m;
    }
  }
}
