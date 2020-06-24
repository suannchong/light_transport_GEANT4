#ifndef _GENERALMANAGER_H_
#define _GENERALMANAGER_H_
// MPND
#include "detectorconstruction.hpp"
// Geant4
// ROOT
// std

namespace MPND {
  /*! \class GeneralManager
   * Singleton that keeps track of general values not specifically
   * applicable to the geometry, file I/O, run, etc, such as the
   * multithreading flag. */
  class GeneralManager {
    public:
      /*! Globally access the instance of this singleton. */
      static GeneralManager& get() {
        static GeneralManager manager;
        return manager;
      }
      /*! Destructor. */
      ~GeneralManager();
      /*! Copy constructor - deleted. */
      GeneralManager(GeneralManager const&) = delete;
      /*! Assignment operator - deleted. */
      void operator=(GeneralManager const&) = delete;

      /*! Get the multithreaded status flag. */
      bool const& getMT() const;
      /*! Set the multithreaded status flag. */
      void setMT(bool const& a_toggle);
      /*! Get a pointer to the DetectorConstruction. */
      DetectorConstruction* getDetectorConstruction();
      void setDetectorConstruction(DetectorConstruction* a_dc);

    private:
      /*! Constructor. */
      GeneralManager();
      /*! Flag for multithreading being enabled/disabled. */
      bool m_fMT;
      /*! Pointer to the DetectorConstruction, which has some useful
       * functions related to the sensitive detectors. */
      DetectorConstruction* m_dc;
  };
}

#endif
