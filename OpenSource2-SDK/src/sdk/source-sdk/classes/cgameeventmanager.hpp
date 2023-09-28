#pragma once

namespace os2::sdk {
  class CGameEventManager {
   public:
    bool add_listener(os2::sdk::IGameEventListener2* listener, const char* name,
                      const bool server_side) {
      return CALL_VIRTUAL(bool, 3, this, listener, name, server_side);
    }

    void remove_listener(os2::sdk::IGameEventListener2* listener) {
      CALL_VIRTUAL(void, 5, this, listener);
    }
  };
};  // namespace os2::sdk
