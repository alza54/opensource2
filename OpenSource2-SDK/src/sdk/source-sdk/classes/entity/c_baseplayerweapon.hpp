#pragma once

#include "../econ/c_econentity.hpp"

namespace os2::sdk {
  class C_BasePlayerWeapon : public C_EconEntity {
   public:
    SCHEMA_FIELD(m_nNextPrimaryAttackTick, "C_BasePlayerWeapon",
                 "m_nNextPrimaryAttackTick", int)
  };
};  // namespace os2::sdk
