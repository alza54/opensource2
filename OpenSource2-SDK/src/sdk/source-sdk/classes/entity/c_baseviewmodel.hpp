#pragma once

#include "c_basemodelentity.hpp"

namespace os2::sdk {
  class C_BaseViewModel : public C_BaseModelEntity {
   public:
    SCHEMA_FIELD(m_hWeapon, "C_BaseViewModel", "m_hWeapon", CHandle);
  };
};  // namespace os2::sdk
