#pragma once

#include <glm/vec2.hpp>

#include "../../virtual/virtual.hpp"

namespace os2::sdk {
  class CEngineClient {
   public:
    __forceinline std::int32_t GetMaxClients() {
      return CALL_VIRTUAL(std::int32_t, 31, this);
    }
    __forceinline bool IsInGame() { return CALL_VIRTUAL(bool, 32, this); }
    __forceinline bool IsConnected() { return CALL_VIRTUAL(bool, 33, this); }

    __forceinline glm::vec2 GetScreenSize() {
      std::int32_t w, h;

      CALL_VIRTUAL(void, 50, this, &w, &h);

      return {static_cast<float>(w), static_cast<float>(h)};
    }
  };
};  // namespace os2::sdk
