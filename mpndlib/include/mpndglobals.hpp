#ifndef _MPNDGLOBALS_H_
#define _MPNDGLOBALS_H_
// MPND
// ROOT
// std
#include <cstddef>
#include <string>
#include <array>

namespace MPND {
  /*! \namespace Detectors
   * Encapsulates detector configuration constants. */
  namespace Detectors {
    /*! Number of available detector configurations. */
    std::size_t const NConfigs = 2;
    /*! Detector config - a list of detector configurations available for
     * simulation. */
    enum class Config : int {
      singlepixel = 0,
      pixelarray = 1
    };
    /*! String versions of detector types for easy reference. */
    std::array<std::string, NConfigs> const ConfigString = {
      "singlepixel", "pixelarray"
    };
  }

  /*! \namespace Filters
   * Constants for the *Filter classes. */
  namespace Filters {
    /*! Number of filter types. */
    std::size_t const NTypes = 2;
    /*! \enum type
     * Filter type enum - a full list of filter types. */
    enum class Type : int {
      particle = 0, energy = 1
    };
    /*! String versions of the filter types. */
    std::array<std::string, NTypes> const TypeString {
      "particle", "energy"
    };
  }
}

#endif
