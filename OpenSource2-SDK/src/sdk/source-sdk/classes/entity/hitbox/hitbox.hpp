#pragma once

#include "../../cutlvector.hpp"

namespace os2::sdk {
  inline const std::string animationsystem_dll = ANIMATIONSYSTEM_DLL;

  class CHitBox {
   public:
    SCHEMA_EXTENDED(m_vMinBounds, animationsystem_dll, "CHitBox", "m_vMinBounds",
                    Vector, 0);
    SCHEMA_EXTENDED(m_vMaxBounds, animationsystem_dll, "CHitBox", "m_vMaxBounds",
                    Vector, 0);

   private:
    char pad[0x70];  // sizeof CHitBox
  };

  class CHitBoxSet {
   public:
    SCHEMA_EXTENDED(m_HitBoxes, animationsystem_dll, "CHitBoxSet", "m_HitBoxes",
                    CUtlVector<CHitBox>, 0);
  };
};  // namespace os2::sdk
