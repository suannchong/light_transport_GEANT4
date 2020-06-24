#ifndef _SIMHIT_H_
#define _SIMHIT_H_
// MPND
// ROOT
#include "TVector3.h"
#include "TString.h"
// std

namespace MPND {
  /*! \class SimHit
   * Encapsulates a simulated hit in a volume with a sensitive detector
   * attached. This is intended to be a lightweight container for the
   * simulated hit data written out by Geant4, for use by processing and
   * analysis code. Geant4 should not be a dependency of this class. */
  class SimHit {
    public:
      /*! Constructor. */
      SimHit();
      /*! Constructor with parameters. */
      SimHit(TVector3 const& a_position, double const& a_energy,
             double const& a_time, TString const& a_particle);
      /*! Destructor. */
      ~SimHit() {}
      /*! Get the hit position [cm]. */
      inline TVector3 const& getPosition() const { return m_position; }
      /*! Set the hit position [cm]. */
      inline void setPosition(TVector3 const& a_position) {
        m_position = a_position;
        return;
      }
      /*! Get the hit energy deposited [keV]. */
      inline double const& getEnergy() const { return m_energy; }
      /*! Set the hit energy deposited [keV]. */
      inline void setEnergy(double const& a_energy) {
        m_energy = a_energy;
        return;
      }
      /*! Get the hit timestamp [ns]. */
      inline double const& getTime() const { return m_time; }
      /*! Set the hit timestamp [ns]. */
      inline void setTime(double const& a_time) {
        m_time = a_time;
        return;
      }
      /*! Get the particle that hit. */
      inline TString const& getParticle() const { return m_particle; }
      /*! Set the particle that hit. */
      inline void setParticle(TString const& a_particle) {
        m_particle = a_particle;
        return;
      }

    private:
      /*! Hit position. */
      TVector3 m_position;
      /*! Hit energy deposited. */
      double m_energy;
      /*! Hit timestamp. */
      double m_time;
      /*! Particle name of the particle that did the hit. */
      TString m_particle;
  };
}

#endif
