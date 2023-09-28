#pragma once

#include "../../virtual/virtual.hpp"

namespace os2::sdk {
  class CGameEntitySystem;

  class CGameResourceService {
   public:
    CGameEntitySystem* GetGameEntitySystem() {
      return *reinterpret_cast<CGameEntitySystem**>((uintptr_t)(this) + 0x58);
    }
  };
};  // namespace os2::sdk
