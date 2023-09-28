#pragma once

#include "../../../schema/schema.hpp"

#include "../chandle.hpp"

namespace os2::sdk {
  class CCSPlayer_ViewModelServices {
   public:
    PSCHEMA_FIELD(m_hViewModel, "CCSPlayer_ViewModelServices", "m_hViewModel",
                  CHandle);
  };
};  // namespace os2::sdk
