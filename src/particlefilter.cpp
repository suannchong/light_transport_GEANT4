// MPND
#include "particlefilter.hpp"
// Geant4
#include "G4Step.hh"
#include "G4ParticleTable.hh"
// ROOT
// std

namespace MPND {
  ParticleFilter::ParticleFilter(G4String const& a_name):
    BaseFilter(a_name),
    m_particles()
  {
  }

  bool ParticleFilter::accept(G4Step const* a_step) const {
    // Is this particle in the list?
    auto pname = a_step->GetTrack()->GetDefinition()->GetParticleName();
    if (std::find(m_particles.begin(), m_particles.end(), pname)
        != m_particles.end()) {
      return true;
    }
    return false;
  }

  std::size_t ParticleFilter::size() const {
    return m_particles.size();
  }

  void ParticleFilter::add(G4String const& a_particle) {
    // Check that it was a valid particle name
    auto pd = G4ParticleTable::GetParticleTable()->FindParticle(
        a_particle);
    if (!pd) {
      G4String msg = "Particle " + a_particle + " not found in the ";
      msg += "particle table.";
      G4Exception("ParticleFilter::add()", "MPNDWarning",
                  JustWarning, msg);
    }
    // Check if it's in the list already - only add if it's not
    if (std::find(m_particles.begin(), m_particles.end(), a_particle)
        == m_particles.end()) {
      m_particles.push_back(a_particle);
      G4cout << a_particle << " added to " << m_name << G4endl;
    } else {
      G4cout << a_particle << " is already in this filter." << G4endl;
    }
    return;
  }

  void ParticleFilter::remove(G4String const& a_particle) {
    // Check that it was a valid particle name
    auto pd = G4ParticleTable::GetParticleTable()->FindParticle(
        a_particle);
    if (!pd) {
      G4String msg = "Particle " + a_particle + " not found in the ";
      msg += "particle table.";
      G4Exception("ParticleFilter::remove()", "MPNDWarning",
                  JustWarning, msg);
    }
    // Check if it's in the list, then remove it
    auto itr = std::find(m_particles.begin(), m_particles.end(),
                         a_particle);
    if (itr != m_particles.end()) {
      G4cout << a_particle << " removed from " << m_name << G4endl;
      m_particles.erase(itr);
    } else {
      G4cout << a_particle << " is not in this filter." << G4endl;
    }
    return;
  }

  void ParticleFilter::clear() {
    m_particles.clear();
    G4cout << "Particle filter " << m_name << " was cleared." << G4endl;
    return;
  }

  G4String ParticleFilter::print() const {
    G4String msg = "";
    for (std::size_t i=0;i < m_particles.size();++i) {
      msg += m_particles[i];
      // Only add ", " if we are not the last element
      if (i < (m_particles.size() - 1)) {
        msg += ", ";
      }
    }
    return msg;
  }
}
