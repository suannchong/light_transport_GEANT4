#ifndef _BASEFILTER_H_
#define _BASEFILTER_H_
// MPND
// Geant4
#include "G4String.hh"
class G4Step;
// ROOT
// std

namespace MPND {
  /*! \class BaseFilter
   * Abstract base class for filters. G4VSDFilter exists, but the SDManager
   * does some manual memory management, or tries to. Objects inheriting
   * from G4VSDFilter must be heap-allocated or the program crashes on
   * exit, which is annoying. Use BaseFilter instead. */
  class BaseFilter {
    public:
      /*! Constructor - must provide a name. */
      BaseFilter(G4String const& a_name): m_name(a_name) {}
      /*! Destructor. */
      virtual ~BaseFilter() {}
      /*! Pure virtual accept(). Must be overridden. */
      virtual bool accept(G4Step const* a_step) const = 0;
      /*! Pure virtual size(). Must be overridden. Every filter must
       * be able to hold multiple values, since the SensitiveDetectors can
       * only hold one of each. */
      virtual std::size_t size() const = 0;
      /*! Get the filter name. */
      inline G4String const& getName() const { return m_name; }
      /*! Set the filter name. */
      inline void setName(G4String const& a_name) {
        m_name = a_name;
        return;
      }

    protected:
      /*! Name of this filter. Not required to be unique. */
      G4String m_name;
  };
}

#endif
