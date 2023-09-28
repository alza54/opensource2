#pragma once

#include "../../memory/memory.hpp"
#include "../../virtual/virtual.hpp"

#include "entity/c_baseviewmodel.hpp"
#include "entity/c_chicken.hpp"
#include "entity/c_plantedc4.hpp"
#include "entity/ccsplayercontroller.hpp"

namespace os2::sdk {
  class CGameEntitySystem {
   public:
    static CGameEntitySystem* GetInstance();
    static CCSPlayerController* GetLocalPlayerController();

    template <typename T = C_BaseEntity>
    T* GetBaseEntity(int index) {
      if (!os2::fn::GetBaseEntity) return nullptr;

      return (T*)(os2::fn::GetBaseEntity(this, index));
    }

    int GetHighestEntityIndex() {
      int highestIdx = -1;

      if (!os2::fn::GetHighestEntityIndex) return highestIdx;

      os2::fn::GetHighestEntityIndex(this, &highestIdx);

      return highestIdx;
    }
  };
};  // namespace os2::sdk
