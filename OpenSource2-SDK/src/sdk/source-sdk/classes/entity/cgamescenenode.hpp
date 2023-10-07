#pragma once

#include "../../../math/classes/transform.hpp"

namespace os2::sdk {
  class CSkeletonInstance;

  class CGameSceneNode {
   public:
    bool GetBonePositionAndRotation(std::int32_t bone_index,
                                    glm::vec3& bone_position,
                                    glm::vec4& bone_rotation);
    void SetMeshGroupMask(uint64_t meshGroupMask);

    __forceinline CSkeletonInstance* GetSkeletonInstance() {
      return CALL_VIRTUAL(CSkeletonInstance*, 8, this);
    }

    SCHEMA_FIELD(m_nodeToWorld, "CGameSceneNode", "m_nodeToWorld", CTransform);
    SCHEMA_FIELD(m_angAbsRotation, "CGameSceneNode", "m_angAbsRotation",
                 glm::vec3);
    SCHEMA_FIELD(m_angRotation, "CGameSceneNode", "m_angRotation", glm::vec3);
    SCHEMA_FIELD(m_bDormant, "CGameSceneNode", "m_bDormant", bool);
    SCHEMA_FIELD(m_vecAbsOrigin, "CGameSceneNode", "m_vecAbsOrigin", glm::vec3);
    SCHEMA_FIELD(m_vecOrigin, "CGameSceneNode", "m_vecOrigin", glm::vec3);
  };
};  // namespace os2::sdk
