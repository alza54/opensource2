#pragma once

#include "../../../schema/schema.hpp"

#include "../../../math/math.hpp"

#include "../chandle.hpp"

namespace os2::sdk {
  class CEntityIdentity {
   public:
    SCHEMA_FIELD(m_designerName, "CEntityIdentity", "m_designerName",
                 const char*);
  };
};  // namespace os2::sdk
