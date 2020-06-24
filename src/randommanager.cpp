// MPND
#include "randommanager.hpp"
// Geant4
#include "globals.hh"
#include "Randomize.hh"
// ROOT
#include "TRandom3.h"
// std

namespace MPND {
  RandomManager::RandomManager():
    m_generator(),
    m_mutex()
  {
    G4cout << "Creating RandomManager..." << G4endl;
    reseed();
  }

  RandomManager::~RandomManager() {
    G4cout << "Deleting RandomManager..." << G4endl;
  }

  void RandomManager::reseed() {
    // Pull a random number from /dev/random
    std::ifstream dev_rand("/dev/random");
    dev_rand.read((char*)&m_seed, sizeof(std::uint32_t));
    dev_rand.close();

    // Set the Geant4, ROOT, and std generator seeds to be the same
    G4Random::setTheSeed(m_seed);
    gRandom->SetSeed(m_seed);
    m_generator.seed(m_seed);
    return;
  }

  std::uint32_t const& RandomManager::getSeed() const {
    return m_seed;
  }

  double RandomManager::uniform() const {
    return G4UniformRand();
  }

  double RandomManager::gaussian(double const& a_mean,
                                 double const& a_sigma) const {
    return G4RandGauss::shoot(a_mean, a_sigma);
  }

  int RandomManager::poisson(double const& a_mean) {
    std::lock_guard<std::mutex> guard(m_mutex);
    std::poisson_distribution<int> dist(a_mean);
    return dist(m_generator);
  }
}
