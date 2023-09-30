#pragma once

#include "c_baseplayerpawn.hpp"
#include "ccsplayer_viewmodelservices.hpp"

namespace os2::sdk {
  class EntitySpottedState_t {
   public:
    bool m_bSpotted;                    // 0x8 - 0x9
    unsigned char pad_9[0x3];           // 0x9 - 0xC
    std::uint32_t m_bSpottedByMask[2];  // 0xC - 0x14
  };

  static_assert(sizeof(EntitySpottedState_t) == 0x14 - 0x8);

  class C_CSPlayerPawnBase : public C_BasePlayerPawn {
   public:
    SCHEMA_FIELD(m_pViewModelServices, "C_CSPlayerPawnBase",
                 "m_pViewModelServices", CCSPlayer_ViewModelServices*);
    SCHEMA_FIELD(m_flFlashDuration, "C_CSPlayerPawnBase", "m_flFlashDuration",
                 float);
    SCHEMA_FIELD(m_entitySpottedState, "C_CSPlayerPawnBase",
                 "m_entitySpottedState", EntitySpottedState_t);
    SCHEMA_FIELD(m_bHasMovedSinceSpawn, "C_CSPlayerPawnBase",
                 "m_bHasMovedSinceSpawn", bool);
    SCHEMA_FIELD(m_szLastPlaceName, "C_CSPlayerPawnBase", "m_szLastPlaceName",
                 char[18]);
  };
};  // namespace os2::sdk
