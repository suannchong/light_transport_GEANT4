#ifndef _MATERIALSMANAGER_H_
#define _MATERIALSMANAGER_H_
// MPND
// Geant4
#include "G4Material.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
// ROOT
// std
#include <string>
#include <array>

namespace MPND {
  /*! \namespace Material
   * Just holds the enum and string names for materials. Access with
   * Material::NAME, e.g. Material::Vacuum. */
  namespace Material {
    /*! Number of custom materials. */
    int const NMATERIALS = 7;
    /*! Enum list of materials. */
    enum Type { Vacuum, Air, NaI, EJ200, BSiGlass, GS20, SiPM};
    /*! String list of materials. */
    std::string const TypeString[NMATERIALS] = {
      "vacuum", "air", "nai", "ej200", "bsiglass", "gs20", "sipm"
    };
    // /*! Number of custom reflectors. */
    // int const NREFLECTORS = 2;
    // /*! Enum list of reflectors. */
    // enum ReflectorType {Teflon, ESR};
    // /*! String list of reflectors. */
    // std::string const ReflectorTypeString[NREFLECTORS] = {"teflon", "esr"};
  }
  /*! \class MaterialsManager
   * Singleton that manages all custom materials and reflectors. */
  class MaterialsManager {
    public:
      /*! Globally access the instance of this singleton. */
      static MaterialsManager& get() {
        static MaterialsManager manager;
        return manager;
      }
      /*! Destructor. */
      ~MaterialsManager();
      /*! Copy constructor - deleted. */
      MaterialsManager(MaterialsManager const&) = delete;
      /*! Assignment operator - deleted. */
      void operator=(MaterialsManager const&) = delete;

      /*! Get the pointer to a material. */
      G4Material* get(Material::Type const& a_type) const;
      // /*! Get the pointer to a reflector. */
      //G4OpticalSurface* get(G4String const& a_name,
      //                      ReflectorType const& a_ref_type,
      //                      G4OpticalSurfaceFinish const& a_finish);

    private:
      /*! Constructor. */
      MaterialsManager();
      /*! List of materials. */
      std::array<G4Material*, Material::NMATERIALS> m_materials;
      /*! Top level build function. */
      void buildAll();
      /*! Build custom isotopes and elements. */
      void buildElements();
      /*! Build vacuum - uses G4_Galactic. */
      void buildVacuum();
      /*! Build air - uses G4_AIR. */
      void buildAir();
      /*! Build NaI - uses G4_SODIUM_IODIDE. */
      void buildNaI();
      /*! Build borosilicate glass, typical PMT entrance window material.
       * */
      void buildBSiGlass();
      /*! Build EJ200 - just the gamma response for now, no neutron
       * response. */
      void buildEJ200();
      /*! Build GS20 - 6Li-enriched glass scintillator. */
      void buildGS20();
      /*! Build SiPM - just Si */
      void buildSiPM();
    };
}

#endif
