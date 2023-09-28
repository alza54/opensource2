#pragma once

#include "../../../math/classes/vector.hpp"

#include "../../../schema/schema.hpp"

namespace os2::sdk {
  class CCollisionProperty {
   public:
    SCHEMA_FIELD(m_vecMins, "CCollisionProperty", "m_vecMins", Vector);
    SCHEMA_FIELD(m_vecMaxs, "CCollisionProperty", "m_vecMaxs", Vector);
    SCHEMA_FIELD(m_solidFlags, "CCollisionProperty", "m_usSolidFlags",
                 std::uint16_t);

    auto CollisionMask() {
      return *reinterpret_cast<std::uint16_t*>((uintptr_t)(this) + 0x38);
    }
  };
};  // namespace os2::sdk
