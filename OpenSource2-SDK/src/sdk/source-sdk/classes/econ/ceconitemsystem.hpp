#pragma once

#include "ceconitemschema.hpp"

namespace os2::sdk {
  class CEconItemSystem {
   public:
    auto GetEconItemSchema() {
      return *reinterpret_cast<CEconItemSchema**>((uintptr_t)(this) + 0x8);
    }
  };
};  // namespace os2::sdk
