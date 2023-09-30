#pragma once

#include "../../../schema/schema.hpp"
#include "../chandle.hpp"
#include "../cnetworkutlvectorbase.hpp"
#include "c_csweaponbase.hpp"

namespace os2::sdk {
  class CPlayer_WeaponServices {
   public:
    SCHEMA_FIELD(m_hActiveWeapon, "CPlayer_WeaponServices", "m_hActiveWeapon",
                 CHandle);
  };
};  // namespace os2::sdk
