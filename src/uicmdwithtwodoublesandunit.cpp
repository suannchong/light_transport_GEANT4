// MPND
#include "uicmdwithtwodoublesandunit.hpp"
// Geant4
// ROOT
// std

namespace MPND {
  // I just copied G4UIcmdWith3Vector for most of this
  UIcmdWithTwoDoublesAndUnit::UIcmdWithTwoDoublesAndUnit(
      G4String const& a_cmd_path, G4UImessenger* a_messenger):
    G4UIcommand(a_cmd_path, a_messenger)
  {
    SetParameter(new G4UIparameter('d'));
    SetParameter(new G4UIparameter('d'));
    auto punit = new G4UIparameter('s');
    punit->SetParameterName("Unit");
    SetParameter(punit);
  }

  void UIcmdWithTwoDoublesAndUnit::SetParameterName(
      char const* a_name0, char const* a_name1, bool a_omittable,
      bool a_current_as_default) {
    auto p0 = GetParameter(0);
    p0->SetParameterName(a_name0);
    p0->SetOmittable(a_omittable);
    p0->SetCurrentAsDefault(a_current_as_default);
    auto p1 = GetParameter(1);
    p1->SetParameterName(a_name1);
    p1->SetOmittable(a_omittable);
    p1->SetCurrentAsDefault(a_current_as_default);
    return;
  }

  void UIcmdWithTwoDoublesAndUnit::SetDefaultValue(
      std::pair<double, double> const& a_val) {
    GetParameter(0)->SetDefaultValue(a_val.first);
    GetParameter(1)->SetDefaultValue(a_val.second);
    return;
  }

  void UIcmdWithTwoDoublesAndUnit::SetDefaultUnit(G4String const& a_unit) {
    auto punit = GetParameter(2);
    punit->SetOmittable(true);
    punit->SetDefaultValue(a_unit);
    SetUnitCategory(CategoryOf(a_unit));
    return;
  }

  void UIcmdWithTwoDoublesAndUnit::SetUnitCategory(
      G4String const& a_category) {
    SetUnitCandidates(UnitsList(a_category));
    return;
  }

  void UIcmdWithTwoDoublesAndUnit::SetUnitCandidates(
      G4String const& a_clist) {
    GetParameter(2)->SetParameterCandidates(a_clist);
    return;
  }

  std::pair<double, double> UIcmdWithTwoDoublesAndUnit::GetNewDoublePair(
      G4String const& a_cmd_args) {
    double p0, p1;
    char units[32];
    std::stringstream in_stream(a_cmd_args.data());
    in_stream >> p0 >> p1 >> units;
    auto uval = ValueOf(units);
    return std::make_pair(p0*uval, p1*uval);
  }
}
