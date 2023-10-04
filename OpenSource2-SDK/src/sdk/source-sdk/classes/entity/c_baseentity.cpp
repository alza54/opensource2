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

bool C_BaseEntity::GetBoundingBox(BBox_t& out, bool computeSurroundingBox) {
  CCollisionProperty* pCollision = m_pCollision();
  if (!pCollision) return false;

  Vector min{}, max{};
  if (computeSurroundingBox) {
    if (!ComputeHitboxSurroundingBox(min, max)) return false;
  } else {
    const glm::vec3 absOrigin = GetOrigin();
    min = pCollision->m_vecMins() + absOrigin;
    max = pCollision->m_vecMaxs() + absOrigin;
  }

  out.x = out.y = std::numeric_limits<float>::max();
  out.w = out.h = -std::numeric_limits<float>::max();

  for (int i = 0; i < 8; ++i) {
    const Vector point{i & 1 ? max.x : min.x, i & 2 ? max.y : min.y,
                       i & 4 ? max.z : min.z};
    glm::vec2 screen;
    if (!os2::math::WorldToScreen(point.glm(), screen)) return false;

    out.x = std::min(out.x, screen.x);
    out.y = std::min(out.y, screen.y);
    out.w = std::max(out.w, screen.x);
    out.h = std::max(out.h, screen.y);
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
