// MPND
#include "simhit.hpp"
// ROOT
// std

namespace MPND {
  // These are defined here instead of the header because ROOT does not
  // seem to pick it up that way when it goes to create the dictionary
  SimHit::SimHit():
    m_position(0., 0., 0.),
    m_energy(0.),
    m_time(0.),
    m_particle("")
  {}

  SimHit::SimHit(TVector3 const& a_position, double const& a_energy,
                 double const& a_time, TString const& a_particle):
    m_position(a_position),
    m_energy(a_energy),
    m_time(a_time),
    m_particle(a_particle)
  {}
}
