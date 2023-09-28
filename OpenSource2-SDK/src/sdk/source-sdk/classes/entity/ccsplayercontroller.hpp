#pragma once

#include "c_csplayerpawn.hpp"
#include "cbaseplayercontroller.hpp"

namespace os2::sdk {
  class CCSPlayerController : public CBasePlayerController {
   public:
    SCHEMA_FIELD(m_sSanitizedPlayerName, "CCSPlayerController",
                 "m_sSanitizedPlayerName", const char*);
    SCHEMA_FIELD(m_iPawnHealth, "CCSPlayerController", "m_iPawnHealth", uint32_t);
    SCHEMA_FIELD(m_iPawnArmor, "CCSPlayerController", "m_iPawnArmor", uint32_t);
    SCHEMA_FIELD(m_bPawnHasDefuser, "CCSPlayerController", "m_bPawnHasDefuser",
                 bool);
    SCHEMA_FIELD(m_bPawnHasHelmet, "CCSPlayerController", "m_bPawnHasHelmet",
                 bool);
    SCHEMA_FIELD(m_bPawnIsAlive, "CCSPlayerController", "m_bPawnIsAlive", bool);
  };
};  // namespace os2::sdk
