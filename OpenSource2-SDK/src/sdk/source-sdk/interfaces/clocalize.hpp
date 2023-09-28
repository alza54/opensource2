#pragma once

#include "../../virtual/virtual.hpp"

namespace os2::sdk {
  class CLocalize {
   public:
    auto FindSafe(const char* tokenName) {
      return CALL_VIRTUAL(const char*, 17, this, tokenName);
    }
  };
};  // namespace os2::sdk
