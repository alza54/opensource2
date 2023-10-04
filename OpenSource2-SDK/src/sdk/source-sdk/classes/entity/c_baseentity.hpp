#pragma once

#include "ccollisionproperty.hpp"
#include "centityinstance.hpp"
#include "cgamescenenode.hpp"

#include "../bitflag.hpp"

namespace os2::sdk {
  struct BBox_t {
    float x, y, w, h;
  };

  class C_BaseEntity : public CEntityInstance {
   public:
    bool IsBasePlayerController();
    bool IsBasePlayerWeapon();
    bool IsChicken();
    bool IsViewModel();
    bool IsPlantedC4();
    bool IsPointCamera();

    glm::vec3 GetOrigin();
    bool GetBoundingBox(os2::sdk::BBox_t& out,
                        bool computeSurroundingBox = false);
    bool ComputeHitboxSurroundingBox(Vector& mins, Vector& maxs);
    float DistanceToSquared(C_BaseEntity* pEntity);

    bool GetBonePosition(const std::int32_t boneIndex, glm::vec3& bonePosition);

    SCHEMA_FIELD(m_pGameSceneNode, "C_BaseEntity", "m_pGameSceneNode",
                 CGameSceneNode*);
    SCHEMA_FIELD(m_pCollision, "C_BaseEntity", "m_pCollision",
                 CCollisionProperty*);
    SCHEMA_FIELD(m_iTeamNum, "C_BaseEntity", "m_iTeamNum", uint8_t);
    SCHEMA_FIELD(m_hOwnerEntity, "C_BaseEntity", "m_hOwnerEntity", CHandle);
    SCHEMA_FIELD(m_fFlags, "C_BaseEntity", "m_fFlags", BitFlag);
    SCHEMA_FIELD(m_vecVelocity, "C_BaseEntity", "m_vecVelocity", Vector);

    [[nodiscard]] bool OnGround() noexcept {
      return (m_fFlags().has_flag(1)) != 0;
    }
  };
};  // namespace os2::sdk
