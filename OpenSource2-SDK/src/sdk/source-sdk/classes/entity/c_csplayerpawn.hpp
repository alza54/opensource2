#pragma once

#include "c_csplayerpawnbase.hpp"

namespace os2::sdk {
  class C_CSPlayerPawn : public C_CSPlayerPawnBase {
   public:
    bool IsEnemyWithTeam(int team);

    [[nodiscard]] bool IsDormant();

    std::uint16_t GetCollisionMask();

    std::uint32_t GetOwnerHandle();

    __forceinline glm::vec3 GetEyePosition() {
      glm::vec3 eye_position;

      CALL_VIRTUAL(std::uint64_t, 159, this, &eye_position);

      return eye_position;
    }

    __forceinline glm::vec3 GetEyeAngles() {
      glm::vec3 eye_position;

      CALL_VIRTUAL(std::uint64_t, 160, this, &eye_position);

      return eye_position;
    }
  };
};  // namespace os2::sdk
