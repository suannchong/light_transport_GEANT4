#ifndef _ENERGYFILTER_H_
#define _ENERGYFILTER_H_
// MPND
// Geant4
#include "basefilter.hpp"
// ROOT
// std
#include <vector>
#include <utility>

namespace MPND {
  /*! \class EnergyFilter
   * A list of energy ranges allowed by the filter. The filter range is
   * [min, max), e.g. it checks >= min (inclusive) and < max (exclusive).
   * If the filter is left empty, accept() will always yield false. The
   * filter performs a logical OR between multiple energy ranges, e.g.
   * a value that is in any of the windows will be accepted. */
  class EnergyFilter: public BaseFilter {
    public:
      /*! Constructor - must provide a name. */
      EnergyFilter(G4String const& a_name);
      /*! Check whether or not to accept the step. If you inherit from
       * this class, make sure to call EnergyFilter::accept(G4Step*)
       * from your accept() function. */
      bool accept(G4Step const* a_step) const override;
      /*! Number of energy ranges currently in this filter. */
      std::size_t size() const override;
      /*! Add an energy range to the list. */
      void add(double const& a_min, double const& a_max);
      /*! Add an energy range to the list. */
      void add(std::pair<double, double> const& a_pair);
      /*! Remove an energy range from the list. */
      void remove(double const& a_min, double const& a_max);
      /*! Remove an energy range from the list using the index. */
      void remove(std::size_t const& a_index);
      /*! Clear the list of energy windows. */
      void clear();
      /*! Get a G4String of the energy range in the form of [min, max). */
      G4String printRange(std::size_t const& a_index) const;

    protected:
      /*! List of allowed energy ranges/windows. [min, max). */
      std::vector<std::pair<double, double>> m_energies;
  };
}

#endif
