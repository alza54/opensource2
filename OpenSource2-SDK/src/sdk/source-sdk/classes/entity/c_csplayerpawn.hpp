#pragma once

#include "c_csplayerpawnbase.hpp"

namespace os2::sdk {
  class C_CSPlayerPawn : public C_CSPlayerPawnBase {
   public:
    SCHEMA_FIELD(m_aimPunchCache, "C_CSPlayerPawn", "m_aimPunchCache",
                 CUtlVector<glm::vec3>);
    SCHEMA_FIELD(m_szLastPlaceName, "C_CSPlayerPawn", "m_szLastPlaceName",
                 char[18]);

    bool IsEnemyWithTeam(int team);

    [[nodiscard]] bool IsDormant();

    std::uint16_t GetCollisionMask();

    std::uint32_t GetOwnerHandle();

    __forceinline glm::vec3 GetEyePosition() {
      return this->m_pGameSceneNode()->m_vecOrigin() + this->m_vecViewOffset();
    }

    __forceinline glm::vec3 GetEyeAngles() {
      glm::vec3 eye_position;

      CALL_VIRTUAL(std::uint64_t, 160, this, &eye_position);

      return eye_position;
    }
  };
};  // namespace os2::sdk
