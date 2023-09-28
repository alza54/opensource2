#pragma once

#include "cgcclient.hpp"

namespace os2::sdk {
  class CGCClientSystem {
   public:
    static CGCClientSystem* GetInstance();

    CGCClient* GetCGCClient() {
      return reinterpret_cast<CGCClient*>((uintptr_t)(this) + 0xB8);
    }
  };
};  // namespace os2::sdk
