#pragma once

#include "../../../schema/schema.hpp"

#include "../../../math/math.hpp"

#include "../chandle.hpp"

namespace os2::sdk {
  class CEntityIdentity {
   public:
    SCHEMA_FIELD(m_designerName, "CEntityIdentity", "m_designerName",
                 const char*);
    SCHEMA_FIELD(m_flags, "CEntityIdentity", "m_flags", uint32_t);
  };
};  // namespace os2::sdk
