#pragma once

#include "cheapmemalloc.hpp"
#include "cutlmap.hpp"

namespace os2::sdk {
  template <typename T>
  class CUtlVector {
   public:
    auto begin() const { return m_data; }
    auto end() const { return m_data + m_size; }

    auto At(int i) const { return m_data[i]; }
    auto AtPtr(int i) const { return m_data + i; }

    bool Exists(T val) const {
      for (const auto& it : *this)
        if (it == val) return true;
      return false;
    }
    bool IsEmpty() const { return m_size == 0; }

    int m_size;
    char pad0[0x4];  // no idea
    T* m_data;
    char pad1[0x8];  // no idea
  };
};  // namespace os2::sdk
