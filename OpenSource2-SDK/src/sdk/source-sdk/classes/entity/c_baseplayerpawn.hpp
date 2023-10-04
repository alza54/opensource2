#pragma once

#include "c_basemodelentity.hpp"
#include "cplayer_weaponservices.hpp"
#include "cplayer_cameraservices.hpp"

namespace os2::sdk {
  class C_BasePlayerPawn : public C_BaseModelEntity {
   public:
    SCHEMA_FIELD(m_hController, "C_BasePlayerPawn", "m_hController", CHandle);
    SCHEMA_FIELD(m_pWeaponServices, "C_BasePlayerPawn", "m_pWeaponServices",
                 CPlayer_WeaponServices*);
    SCHEMA_FIELD(m_pCameraServices, "C_BasePlayerPawn", "m_pCameraServices",
                 CPlayer_CameraServices*);
  };
};  // namespace os2::sdk
