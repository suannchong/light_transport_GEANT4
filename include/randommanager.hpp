#ifndef _RANDOMMANAGER_H_
#define _RANDOMMANAGER_H_
// MPND
// Geant4
// ROOT
// std
#include <random>
#include <mutex>

namespace MPND {
  /*! \class RandomManager
   * Singleton that provides random numbers from a desired distribution.
   * These functions are all thread safe. */
  class RandomManager {
    public:
      /*! Globally access the instance of this singleton. */
      static RandomManager& get() {
        static RandomManager manager;
        return manager;
      }
      /*! Destructor. */
      ~RandomManager();
      /*! Copy constructor - deleted. */
      RandomManager(RandomManager const&) = delete;
      /*! Assignment operator - deleted. */
      void operator=(RandomManager const&) = delete;
      
      /*! Pull a new random number seed from /dev/random. */
      void reseed();
      /*! Get the current seed. */
      std::uint32_t const& getSeed() const;
      /*! Get a random number from a uniform distribution [0, 1]. Just
       * a pass-through for G4UniformRand(). */
      double uniform() const;
      /*! Get a random number from a Gaussian distribution with the
       * provided mean and standard deviation. Just a pass-through for
       * G4RandGauss::shoot() */
      double gaussian(double const& a_mean, double const& a_sigma) const;
      /*! Get a random number from a Poisson distribution with the
       * provided mean. */
      int poisson(double const& a_mean);

    private:
      /*! Constructor. */
      RandomManager();
      /*! Current random seed. */
      std::uint32_t m_seed;
      /*! Standard library Mersenne Twister generator. */
      std::mt19937 m_generator;
      /*! Mutex for thread safety, where needed. */
      std::mutex m_mutex;
  };
}

#endif
