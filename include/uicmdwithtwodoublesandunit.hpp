#ifndef _UICMDWITHTWODOUBLES_H_
#define _UICMDWITHTWODOUBLES_H_
// MPND
// Geant4
#include "G4UIcommand.hh"
// ROOT
// std
#include <utility>

namespace MPND {
  /*! \class UIcmdWithTwoDoublesAndUnit
   * Custom UI class to receive a command that requires two doubles,
   * with units specified as the third parameter. */
  class UIcmdWithTwoDoublesAndUnit: public G4UIcommand {
    public:
      /*! Constructor. Must provide the command path and a pointer to the
       * UI messenger, same as the other commands. */
      UIcmdWithTwoDoublesAndUnit(G4String const& a_cmd_path,
                                 G4UImessenger* a_messenger);
      /*! Set the parameter name. */
      void SetParameterName(char const* a_name0, char const* a_name1,
                            bool a_omittable,
                            bool a_current_as_default = false);
      /*! Set the default values. */
      void SetDefaultValue(std::pair<double, double> const& a_val);
      /*! Set the default units. */
      void SetDefaultUnit(G4String const& a_unit);
      /*! Set the unit category. */
      void SetUnitCategory(G4String const& a_category);
      /*! Set the unit candidates list. */
      void SetUnitCandidates(G4String const& a_clist);
      /*! Parse the arguments and return the pair. */
      static std::pair<double, double> GetNewDoublePair(
          G4String const& a_cmd_args);
  };
}

#endif
