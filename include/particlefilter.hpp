#ifndef _PARTICLEFILTER_H_
#define _PARTICLEFILTER_H_
// MPND
// Geant4
#include "basefilter.hpp"
// ROOT
// std
#include <vector>

namespace MPND {
  /*! \class ParticleFilter
   * Basically just a particle whitelist for the SensitiveDetectors. If
   * the list is empty, accept() will always return false. Otherwise, the
   * filter just checks if the particle in the step matches any one of the
   * items on the list. */
  class ParticleFilter: public BaseFilter {
    public:
      /*! Constructor - must provide a name. */
      ParticleFilter(G4String const& a_name);
      /*! Check whether or not to accept the step. If you inherit from
       * this class, make sure to call ParticleFilter::accept(G4Step*)
       * from your accept() function. */
      bool accept(G4Step const* a_step) const override;
      /*! Number of particles in the filter. */
      std::size_t size() const override;
      /*! Add a particle to the list. */
      void add(G4String const& a_particle);
      /*! Remove a particle from the list. */
      void remove(G4String const& a_particle);
      /*! Clear the particle list. */
      void clear();
      /*! Get a string of the list. */
      G4String print() const;

    protected:
      /*! List of allowed particles. */
      std::vector<G4String> m_particles;
  };
}

#endif
