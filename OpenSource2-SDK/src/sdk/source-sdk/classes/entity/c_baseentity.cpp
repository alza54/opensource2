#include "c_baseentity.hpp"

#include "../../../interfaces/interfaces.hpp"
#include "../../../memory/memory.hpp"
#include "../../../math/math.hpp"

#include <imgui/imgui_internal.h>

using namespace os2::sdk;

bool C_BaseEntity::IsBasePlayerController() {
  return CALL_VIRTUAL(bool, 144, this);
}

bool C_BaseEntity::IsBasePlayerWeapon() {
  return CALL_VIRTUAL(bool, 150, this);
}

bool C_BaseEntity::IsChicken() {
  SchemaClassInfoData_t* pClassInfo = Schema_DynamicBinding();
  if (!pClassInfo) return false;

  const char* className = pClassInfo->GetName();
  if (!className) return false;

  static constexpr auto C_Chicken = hash_32_fnv1a_const("C_Chicken");
  return hash_32_fnv1a_const(className) == C_Chicken;
}

bool C_BaseEntity::IsPlantedC4() {
  SchemaClassInfoData_t* pClassInfo = Schema_DynamicBinding();
  if (!pClassInfo) return false;

  const char* className = pClassInfo->GetName();
  if (!className) return false;

  static constexpr auto C_PlantedC4 = hash_32_fnv1a_const("C_PlantedC4");
  return hash_32_fnv1a_const(className) == C_PlantedC4;
}

// Useless
bool C_BaseEntity::IsPointCamera() {
  SchemaClassInfoData_t* pClassInfo = Schema_DynamicBinding();
  if (!pClassInfo) return false;

  const char* className = pClassInfo->GetName();
  if (!className) return false;

  static constexpr auto C_PointCamera = hash_32_fnv1a_const("C_PointCamera");
  return hash_32_fnv1a_const(className) == C_PointCamera;
}

bool C_BaseEntity::IsViewModel() { return CALL_VIRTUAL(bool, 242, this); }

glm::vec3 C_BaseEntity::GetOrigin() {
  static const glm::vec3 null{};

  CGameSceneNode* pGameSceneNode = m_pGameSceneNode();
  if (!pGameSceneNode) return null;

  return pGameSceneNode->m_vecAbsOrigin();
}

// Copyright @bruhmoment21
// This comes from:
// https://github.com/bruhmoment21/cs2-sdk/blob/dc9636aad20975a052d779b771e5708db80051be/cs2-sdk/sdk/src/bindings/baseentity.cpp#L77
bool C_BaseEntity::CalculateBBoxByCollision(BBox_t& out) {
  CGameSceneNode* node = m_pGameSceneNode();
  if (!node) return false;

  CCollisionProperty* collision = m_pCollision();
  if (!collision) return false;

  const Vector mins = collision->m_vecMins(), maxs = collision->m_vecMaxs();

  out.Invalidate();

  for (int i = 0; i < 8; ++i) {
    const glm::vec3 worldPoint{i & 1 ? maxs.x : mins.x, i & 2 ? maxs.y : mins.y,
                               i & 4 ? maxs.z : mins.z};

    if (!os2::math::WorldToScreen(
            os2::math::VecTransformMatrix(worldPoint,
                                          node->m_nodeToWorld().ToMatrix()),
            out.m_Vertices[i]))
      return false;

    out.m_Mins = ImMin(out.m_Mins, out.m_Vertices[i]);
    out.m_Maxs = ImMax(out.m_Maxs, out.m_Vertices[i]);
  }

  return true;
}

// Copyright:
// https://github.com/bruhmoment21/cs2-sdk/blob/dc9636aad20975a052d779b771e5708db80051be/cs2-sdk/sdk/src/bindings/baseentity.cpp#L77
bool C_BaseEntity::CalculateBBoxByHitbox(BBox_t& out) {
  constexpr int MAX_HITBOXES = 64;

  os2::sdk::CHitBoxSet* hitBoxSet = GetHitboxSet(0);
  if (!hitBoxSet) return false;

  const CUtlVector<CHitBox>& hitBoxes = hitBoxSet->m_HitBoxes();
  if (hitBoxes.m_size > MAX_HITBOXES) {
    LOG("[bbox] hitBoxTransforms[{}] way too small! (%i)\n", MAX_HITBOXES);
    return false;
  }

  CTransform hitBoxTransforms[MAX_HITBOXES];
  int hitBoxCount = HitboxToWorldTransforms(hitBoxSet, hitBoxTransforms);
  if (!hitBoxCount) return false;

  glm::vec3 mins{std::numeric_limits<float>::max()},
      maxs{-std::numeric_limits<float>::max()};
  for (int i = 0; i < hitBoxCount; ++i) {
    const glm::mat3x4 hitBoxMatrix = hitBoxTransforms[i].ToMatrix();
    auto hitBox = hitBoxes.AtPtr(i);

    glm::vec3 worldMins, worldMaxs;
    os2::math::TransformAABB(hitBoxMatrix, hitBox->m_vMinBounds().glm(),
                             hitBox->m_vMaxBounds().glm(), worldMins,
                             worldMaxs);

    mins = {std::min(mins.x, worldMins.x), std::min(mins.y, worldMins.y),
            std::min(mins.z, worldMins.z)};
    maxs = {std::max(maxs.x, worldMaxs.x), std::min(maxs.y, worldMaxs.y),
            std::min(maxs.z, worldMaxs.z)};
  }

  out.Invalidate();

  for (int i = 0; i < 8; ++i) {
    const Vector worldPoint{i & 1 ? maxs.x : mins.x, i & 2 ? maxs.y : mins.y,
                            i & 4 ? maxs.z : mins.z};

    if (!os2::math::WorldToScreen(worldPoint, out.m_Vertices[i])) return false;

    out.m_Mins = ImMin(out.m_Mins, out.m_Vertices[i]);
    out.m_Maxs = ImMax(out.m_Maxs, out.m_Vertices[i]);
  }

  return true;
}

bool C_BaseEntity::ComputeHitboxSurroundingBox(Vector& mins, Vector& maxs) {
  if (!os2::fn::ComputeHitboxSurroundingBox) return false;
  return os2::fn::ComputeHitboxSurroundingBox(this, mins, maxs);
}

float C_BaseEntity::DistanceToSquared(C_BaseEntity* pEntity) {
  const Vector& currentOrigin = GetOrigin();
  const Vector& entityOrigin = pEntity->GetOrigin();

  const float _x = (currentOrigin.x - entityOrigin.x);
  const float _y = (currentOrigin.y - entityOrigin.y);
  const float _z = (currentOrigin.z - entityOrigin.z);

  // https://developer.valvesoftware.com/wiki/Dimensions_(Half-Life_2_and_Counter-Strike:_Source)/en
  // 1 foot = 12 units => 1 unit = 0.0254 meters.
  return (_x * _x + _y * _y + _z * _z) * 0.00064516f;
}

bool C_BaseEntity::GetBonePosition(const std::int32_t boneIndex,
                                   glm::vec3& bonePosition) {
  CGameSceneNode* gameSceneNode = m_pGameSceneNode();

  if (gameSceneNode == nullptr) return false;

  glm::vec4 boneRotation;

  return gameSceneNode->GetBonePositionAndRotation(boneIndex, bonePosition,
                                                   boneRotation);
}

CHitBoxSet* C_BaseEntity::GetHitboxSet(int i) {
  if (!os2::fn::GetHitboxSet) return nullptr;

  return os2::fn::GetHitboxSet(this, i);
}

int C_BaseEntity::HitboxToWorldTransforms(CHitBoxSet* hitBoxSet,
                                          CTransform* hitboxToWorld) {
  if (!os2::fn::HitboxToWorldTransforms) return -1;

  return os2::fn::HitboxToWorldTransforms(this, hitBoxSet, hitboxToWorld, 1024);
}
