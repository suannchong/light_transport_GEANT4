// MPND
#include "sensitivedetector.hpp"
#include "sdmessenger.hpp"
// Geant4
#include "G4VProcess.hh"
#include "G4UnitsTable.hh"
#include "CLHEP/Units/SystemOfUnits.h"
using namespace CLHEP;
// ROOT
// std

namespace MPND {
  SensitiveDetector::SensitiveDetector(G4String const& a_name):
    G4VSensitiveDetector(a_name),
    m_id(-1),
    m_hitsCollection(nullptr),
    // default filters will/should be allocated on SD initialization in
    // GeometryManager. All filters set to nullptr on construction
    m_filters({nullptr})
  {
    G4cout << "Creating SensitiveDetector " << a_name << "..." << G4endl;
    G4String hc_name = a_name + "HC";
    collectionName.insert(hc_name);
    // Add to the list, so the messenger works
    SDMessenger::get().addSD(this);
  }

  SensitiveDetector::~SensitiveDetector() {
    G4cout << "Deleting SensitiveDetector " << GetName() << "..." << G4endl;
  }

  void SensitiveDetector::Initialize(G4HCofThisEvent* a_hc) {
    // Check if we have a valid ID
    if (m_id < 0) {
      m_id = GetCollectionID(0);
    }
    // Create a hits collection for this SD
    m_hitsCollection = new HitsCollection(SensitiveDetectorName,
                                          collectionName[0]);
    // Give the collection to the G4Event, so we can get it at the end
    // in Run::RecordEvent()
    a_hc->AddHitsCollection(m_id, m_hitsCollection);
    return;
  }

  bool SensitiveDetector::ProcessHits(G4Step* a_step,
                                      G4TouchableHistory*) {
    // Check filters
    for (auto& f : m_filters) {
      // Check if this filter even exists; it's possible that one was not
      // set
      if (f) {
        if (!f->accept(a_step)) {
          return true;
        }
      }
    }

    auto track = a_step->GetTrack();
    // MPND:: needed because of a naming conflict I think? Probably with
    // ROOT
    auto hit = new MPND::Hit(
        track->GetTrackID(),
        track->GetParentID(),
        track->GetVolume()->GetName(),
        track->GetDefinition()->GetParticleName(),
        (track->GetCreatorProcess()
         ? track->GetCreatorProcess()->GetProcessName()
         : "generator"),
        track->GetPosition(),
        a_step->GetTotalEnergyDeposit(),
        track->GetGlobalTime());
    m_hitsCollection->insert(hit);
    G4cout << "Got a hit!" << G4endl;
    G4cout << "Volume: " << hit->getVolumeName() << "\n";
    G4cout << "Particle: " << hit->getParticleName() << "\n";
    G4cout << "Creator process: " << hit->getCreatorProcess() << "\n";
    G4cout << "Position: " << G4BestUnit(hit->getPosition(), "Length")
      << "\n";
    G4cout << "Energy deposited: "
      << G4BestUnit(hit->getEnergy(), "Energy") << "\n";
    G4cout << "Time: " << G4BestUnit(hit->getTime(), "Time") << "\n"
      << G4endl;
    return true;
  }

  BaseFilter* SensitiveDetector::getFilter(Filters::Type const& a_type) {
    return m_filters[static_cast<int>(a_type)];
  }

  void SensitiveDetector::setFilter(Filters::Type const& a_type,
                                    BaseFilter* a_filter) {
    m_filters[static_cast<int>(a_type)] = a_filter;
    return;
  }
}
