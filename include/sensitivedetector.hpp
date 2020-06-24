#ifndef _SENSITIVEDETECTOR_H_
#define _SENSITIVEDETECTOR_H_
// MPND
#include "hit.hpp"
#include "particlefilter.hpp"
#include "energyfilter.hpp"
#include "mpndglobals.hpp"
// Geant4
#include "G4VSensitiveDetector.hh"
// ROOT
// std

namespace MPND {
  /*! \class SensitiveDetector
   * Makes a detector sensitive to particle tracks. Attach this object
   * to a G4LogicalVolume, and when a whitelisted particle enters,
   * its Hits (Edep > 0) will be recorded.
   * SensitiveDetector memory is managed by the G4SDManager, and they
   * are automatically added to the SDMessenger list when they are
   * constructed. */
  class SensitiveDetector: public G4VSensitiveDetector {
    public:
      /*! Constructor. */
      SensitiveDetector(G4String const& a_name);
      ~SensitiveDetector();

      /*! Initialize the hits collection. */
      void Initialize(G4HCofThisEvent* a_hc);
      /*! Process a G4Step, save the information in a Hit, and add to the
       * collection for this event. */
      bool ProcessHits(G4Step* a_step, G4TouchableHistory*);
      /*! Get a filter of the specified type. */
      BaseFilter* getFilter(Filters::Type const& a_type);
      /*! Set a filter of the specified type. */
      void setFilter(Filters::Type const& a_type, BaseFilter* a_filter);

    private:
      /*! Collection ID. */
      int m_id;
      /*! Pointer to the hits collection. */
      HitsCollection* m_hitsCollection;
      /*! List of filters. */
      std::array<BaseFilter*, Filters::NTypes> m_filters;
  };
}

#endif
