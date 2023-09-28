#pragma once

#include "../../../schema/schema.hpp"
#include "../chandle.hpp"

#include "c_basemodelentity.hpp"

namespace os2::sdk {
  class C_Chicken : public C_BaseModelEntity {
   public:
    SCHEMA_FIELD(m_leader, "C_Chicken", "m_leader", CHandle);
  };
};  // namespace os2::sdk
