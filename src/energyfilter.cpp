// MPND
#include "energyfilter.hpp"
// Geant4
#include "G4Step.hh"
#include "G4UnitsTable.hh"
// ROOT
// std

namespace MPND {
  EnergyFilter::EnergyFilter(G4String const& a_name):
    BaseFilter(a_name),
    m_energies()
  {
  }

  bool EnergyFilter::accept(G4Step const* a_step) const {
    // First check if no windows are set - if so, reject
    if (m_energies.size() == 0) {
      return false;
    }
    // See if the energy deposited fits any of the ranges
    auto edep = a_step->GetTotalEnergyDeposit();
    std::vector<bool> accept(m_energies.size(), true);
    for (std::size_t i=0;i < m_energies.size();++i) {
      auto ewindow = m_energies[i];
      if (edep < ewindow.first) {
        accept[i] = false;
      }
      if (edep >= ewindow.second) {
        accept[i] = false;
      }
    }
    // Check if the value was in any of the windows (any of accept[] is
    // true)
    for (auto const& val : accept) {
      if (val) {
        return true;
      }
    }
    return false;
  }

  std::size_t EnergyFilter::size() const {
    return m_energies.size();
  }

  void EnergyFilter::add(double const& a_min, double const& a_max) {
    // Check that this range is not already added
    if (std::find(m_energies.begin(), m_energies.end(),
                  std::make_pair(a_min, a_max)) != m_energies.end()) {
      G4cout << "Energy range is already in the filter." << G4endl;
    } else {
      m_energies.push_back(std::make_pair(a_min, a_max));
      G4cout << "Energy range [" << G4BestUnit(a_min, "Energy")
        << ", " << G4BestUnit(a_max, "Energy") << ") added to "
        << m_name << G4endl;
    }
    return;
  }

  void EnergyFilter::add(std::pair<double, double> const& a_pair) {
    // Check that this range is not already added
    if (std::find(m_energies.begin(), m_energies.end(), a_pair)
        != m_energies.end()) {
      G4cout << "Energy range is already in the filter." << G4endl;
    } else {
      m_energies.push_back(a_pair);
      G4cout << "Energy range [" << G4BestUnit(a_pair.first, "Energy")
        << ", " << G4BestUnit(a_pair.second, "Energy") << ") "
        << "added to " << m_name << G4endl;
    }
    return;
  }

  void EnergyFilter::remove(double const& a_min, double const& a_max) {
    // Search for the window
    auto itr = std::find(m_energies.begin(), m_energies.end(),
                         std::make_pair(a_min, a_max));
    if (itr != m_energies.end()) {
      m_energies.erase(itr);
      G4cout << "Energy range [" << G4BestUnit(a_min, "Energy")
        << ", " << G4BestUnit(a_max, "Energy") << ") removed from "
        << m_name << G4endl;
    } else {
      G4cout << "Energy range [" << G4BestUnit(a_min, "Energy")
        << ", " << G4BestUnit(a_max, "Energy") << ") not found in "
        << m_name << G4endl;
    }
    return;
  }

  void EnergyFilter::remove(std::size_t const& a_index) {
    if (a_index >= m_energies.size()) {
      G4Exception("EnergyFilter::remove(index)", "Invalid Index",
          JustWarning, "Invalid energy filter index provided.");
    }
    auto val = m_energies[a_index];
    G4cout << "Energy range [" << G4BestUnit(val.first, "Energy")
      << ", " << G4BestUnit(val.second, "Energy") << ") removed from "
      << m_name << G4endl;
    m_energies.erase(m_energies.begin() + a_index);
    return;
  }

  void EnergyFilter::clear() {
    m_energies.clear();
    G4cout << "Energy filter " << m_name << " was cleared." << G4endl;
    return;
  }

  G4String EnergyFilter::printRange(std::size_t const& a_index) const {
    if (a_index >= m_energies.size()) {
      G4Exception("EnergyFilter::printRange(index)", "Invalid Index",
          JustWarning, "Invalid energy filter index provided.");
    }
    G4String msg = "[";
    msg += G4BestUnit(m_energies[a_index].first, "Energy");
    msg += ", ";
    msg += G4BestUnit(m_energies[a_index].second, "Energy");
    msg += ")";
    return msg;
  }
}
