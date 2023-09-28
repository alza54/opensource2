#include "../../../memory/memory.hpp"

#include "cgamescenenode.hpp"

using namespace os2::sdk;

void CGameSceneNode::SetMeshGroupMask(uint64_t meshGroupMask) {
  if (!os2::fn::SetMeshGroupMask) return;
  return os2::fn::SetMeshGroupMask(this, meshGroupMask);
}

bool CGameSceneNode::GetBonePositionAndRotation(const std::int32_t bone_index,
                                                glm::vec3& bone_position,
                                                glm::vec4& bone_rotation) {
  CSkeletonInstance* skeleton = GetSkeletonInstance();

  if (skeleton == nullptr) return false;

  const CModelState model_state = skeleton->m_modelState();

  const CBoneData* bone_array = model_state.bones;

  if (bone_array == nullptr) return false;

  const CBoneData& data = bone_array[bone_index];

  bone_position = data.position;
  bone_rotation = data.rotation;

  return true;
}
