// MPND
#include "filemanager.hpp"
// Geant4
#include "G4ios.hh"
// ROOT
// std

namespace MPND {
  FileManager::FileManager():
    m_fInitialized(false),
    m_fWriteData(true),
    m_path("sim.root"),
    m_file(nullptr),
    m_tree(nullptr),
    m_bufferLimit(1000),
    m_maxFileSize(100)
  {
    G4cout << "Creating FileManager..." << G4endl;
  }

  FileManager::~FileManager() {
    G4cout << "Deleting FileManager..." << G4endl;
    if (m_fInitialized) {
      close();
    }
  }

  std::string const& FileManager::getPath() const {
    return m_path;
  }

  void FileManager::setPath(std::string const& a_path) {
    m_path = a_path;
    return;
  }

  bool const& FileManager::getWriteData() const {
    return m_fWriteData;
  }

  void FileManager::setWriteData(bool const& a_write) {
    m_fWriteData = a_write;
    return;
  }

  std::uint64_t const& FileManager::getBufferLimit() {
    return m_bufferLimit;
  }

  void FileManager::setBufferLimit(std::uint64_t const& a_limit) {
    m_bufferLimit = a_limit;
    return;
  }

  int const& FileManager::getMaxFileSize() const {
    return m_maxFileSize;
  }

  void FileManager::setMaxFileSize(int const& a_max) {
    m_maxFileSize = a_max;
    TTree::SetMaxTreeSize(m_maxFileSize * 1000000000);
    return;
  }

  void FileManager::initialize() {
    if (m_fInitialized) {
      return;
    }
    if (m_fWriteData) {
      m_hit = new SimHit;
      m_file = new TFile(m_path.c_str(), "RECREATE");
      m_tree = new TTree("Hits", "Simulation hit data");
      m_tree->Branch("hit", &m_hit);
      G4cout << "Writing data to " << m_path << G4endl;
    } else {
      G4cout << "Not writing data" << G4endl;
    }
    m_fInitialized = true;
    return;
  }

  void FileManager::close() {
    if (!m_fInitialized) {
      return;
    }
    // Make sure we've written/cleared the buffer
    write();
    if (m_fWriteData) {
      m_file->Write();
      m_file->Close();
      delete m_hit;
    }
    m_fInitialized = false;
    return;
  }

  void FileManager::addHit(Hit const& a_hit) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_hitBuffer.push(a_hit);
    if (m_hitBuffer.size() == m_bufferLimit) {
      // This thread will block the buffer from being touched because of
      // the above lock_guard until write() is finished. Not optimal but
      // it is ok for now
      write();
    }
    return;
  }

  void FileManager::write() {
    if (m_fWriteData) {
      while (!m_hitBuffer.empty()) {
        // Copy the Hit data to a SimHit
        auto& hit = m_hitBuffer.front();
        *m_hit = hit.toSimHit();
        m_tree->Fill();
        m_hitBuffer.pop();
      }
    } else {
      // Swap the buffer with an empty one (there's no clear() function)
      // Probably faster than manually pop()ing the elements
      std::queue<Hit>().swap(m_hitBuffer);
    }
    return;
  }
}
