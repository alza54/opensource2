#pragma once

#include "../../virtual/virtual.hpp"

#include "../classes/econ/ceconitemsystem.hpp"

namespace os2::sdk {
  class CSource2Client {
   public:
    auto GetEconItemSystem() { return CALL_VIRTUAL(CEconItemSystem*, 111, this); }
  };
};  // namespace os2::sdk
