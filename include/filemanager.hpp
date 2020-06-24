#ifndef _FILEMANAGER_H_
#define _FILEMANAGER_H_
// MPND
#include "hit.hpp"
// Geant4
// ROOT
#include "TFile.h"
#include "TTree.h"
// std
#include <string>
#include <queue>
#include <mutex>

namespace MPND {
  /*! \class FileManager
   * Singleton that handles file I/O. Has a thread-safe buffer where
   * event information is stored, which is periodically written to disk. */
  class FileManager {
    public:
      /*! Globally access the instance of this singleton. */
      static FileManager& get() {
        static FileManager manager;
        return manager;
      }
      /*! Destructor. */
      ~FileManager();
      /*! Copy constructor - deleted. */
      FileManager(FileManager const&) = delete;
      /*! Assignment operator - deleted. */
      void operator=(FileManager const&) = delete;

      /*! Get the output file path. */
      std::string const& getPath() const;
      /*! Set the output file path. */
      void setPath(std::string const& a_path);
      /*! Get the flag for whether or not we are writing to disk. */
      bool const& getWriteData() const;
      /*! Set the flag for whether or not we are writing to disk. */
      void setWriteData(bool const& a_write);
      /*! Get the buffer event limit. */
      std::uint64_t const& getBufferLimit();
      /*! Set the buffer event limit. */
      void setBufferLimit(std::uint64_t const& a_limit);
      /*! Get the max file size [GB]. */
      int const& getMaxFileSize() const;
      /*! Set the max file size [GB]. */
      void setMaxFileSize(int const& a_max);
      /*! Initialize - open a file, make the tree, etc. Must be called at
       * the start of each run. */
      void initialize();
      /*! Close the file, clean up, and return to an uninitialized state.
       * Must be called at the end of each run. */
      void close();
      /*! Add a hit to the queue to be written to disk. */
      void addHit(Hit const& a_hit);

    private:
      /*! Constructor. */
      FileManager();
      /*! Flag for whether or not the FileManager has been initialized for
       * this run. */
      bool m_fInitialized;
      /*! Flag for whether or not we save events to disk. */
      bool m_fWriteData;
      /*! ROOT file path. */
      std::string m_path;
      /*! Output ROOT file. */
      TFile* m_file;
      /*! Output ROOT tree. */
      TTree* m_tree;
      /*! Pointer to a SimHit, attached to the tree. */
      SimHit* m_hit;
      /*! How many events to store before emptying the buffer. This
       * basically controls the write frequency. */
      std::uint64_t m_bufferLimit;
      /*! Max size of the ROOT file before switching to another file [GB].
       * */
      int m_maxFileSize;
      /*! Mutex for the buffer. */
      std::mutex m_mutex;
      /*! Hit buffer. */
      std::queue<Hit> m_hitBuffer;
      /*! Write the contents of the m_hitBuffer to disk. */
      void write();
  };
}

#endif
