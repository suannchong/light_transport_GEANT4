// MPND
#include "basevolume.hpp"
// Geant4
// ROOT
// std

namespace MPND {
  BaseVolume::BaseVolume(G4String const& a_name):
    m_name(a_name),
    m_position(0., 0., 0.),
    m_dimensions(0., 0., 0.),
    m_vis(nullptr)
  {
  }

  BaseVolume::~BaseVolume() {
  }

  G4String const& BaseVolume::getName() const {
    return m_name;
  }

  G4ThreeVector const& BaseVolume::getPosition() const {
    return m_position;
  }

  void BaseVolume::setPosition(G4ThreeVector const& a_pos) {
    m_position = a_pos;
    return;
  }

  void BaseVolume::setPosition(double const& a_x,
                               double const& a_y,
                               double const& a_z) {
    m_position.setX(a_x);
    m_position.setY(a_y);
    m_position.setZ(a_z);
    return;
  }

  G4ThreeVector const& BaseVolume::getDimensions() const {
    return m_dimensions;
  }

  void BaseVolume::setDimensions(G4ThreeVector const& a_dim) {
    m_dimensions = a_dim;
    return;
  }

  void BaseVolume::setDimensions(double const& a_x,
                                 double const& a_y,
                                 double const& a_z) {
    m_dimensions.setX(a_x);
    m_dimensions.setY(a_y);
    m_dimensions.setZ(a_z);
    return;
  }

  G4LogicalVolume *BaseVolume::getLogical() const
  {
    return m_logical;
  }

  G4PVPlacement *BaseVolume::getPhysical() const
  {
    return m_physical;
  }
}
