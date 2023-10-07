#include "math.hpp"

#include <algorithm>
#include <glm/trigonometric.hpp>
#include <numbers>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "../interfaces/interfaces.hpp"
#include "../memory/memory.hpp"

inline glm::mat4x4 m_ViewMatrix;

// An alternative way to calculate the world to screen matrix.
void os2::math::UpdateViewMatrix(os2::sdk::VMatrix* pViewMatrix) noexcept {
  if (!pViewMatrix) return;
  m_ViewMatrix = pViewMatrix->glm();
}

// s/o:
// https://github.com/bruhmoment21/cs2-sdk/blob/dc9636aad20975a052d779b771e5708db80051be/cs2-sdk/src/math/math.cpp#L39
bool os2::math::WorldToScreen(const os2::sdk::Vector& in,
                              ImVec2& out) noexcept {
  if (!ImGui::GetCurrentContext()) return false;

  const float z = m_ViewMatrix[3][0] * in.x + m_ViewMatrix[3][1] * in.y +
                  m_ViewMatrix[3][2] * in.z + m_ViewMatrix[3][3];
  if (z < 0.001f) return false;

  out = ImGui::GetIO().DisplaySize * 0.5f;
  out.x *= 1.0f + (m_ViewMatrix[0][0] * in.x + m_ViewMatrix[0][1] * in.y +
                   m_ViewMatrix[0][2] * in.z + m_ViewMatrix[0][3]) /
                      z;
  out.y *= 1.0f - (m_ViewMatrix[1][0] * in.x + m_ViewMatrix[1][1] * in.y +
                   m_ViewMatrix[1][2] * in.z + m_ViewMatrix[1][3]) /
                      z;

  // Prevents rounded corners.
  out = ImFloor(out);

  return true;
}

// s/o:
// https://github.com/bruhmoment21/cs2-sdk/blob/dc9636aad20975a052d779b771e5708db80051be/cs2-sdk/src/math/math.cpp#L39
void os2::math::TransformAABB(const glm::mat3x4& mat, const glm::vec3& minsIn,
                              const glm::vec3& maxsIn, glm::vec3& minsOut,
                              glm::vec3& maxsOut) noexcept {
  const glm::vec3 localCenter{(minsIn + maxsIn) * 0.5f};
  const glm::vec3 localExtent{maxsIn - localCenter};

  const glm::vec3 worldAxisX{mat[0][0], mat[0][1], mat[0][2]};
  const glm::vec3 worldAxisY{mat[1][0], mat[1][1], mat[1][2]};
  const glm::vec3 worldAxisZ{mat[2][0], mat[2][1], mat[2][2]};

  const glm::vec3 worldCenter = VecTransformMatrix(localCenter, mat);
  const glm::vec3 worldExtent{AbsoluteDotProduct(localExtent, worldAxisX),
                              AbsoluteDotProduct(localExtent, worldAxisY),
                              AbsoluteDotProduct(localExtent, worldAxisZ)};

  minsOut = worldCenter - worldExtent;
  maxsOut = worldCenter + worldExtent;
}

void os2::math::ClampAngle(glm::vec3& angle) noexcept {
  NormalizeAngle(angle);

  angle.x = std::clamp(angle.x, -89.0f, 89.0f);
  angle.y = std::clamp(angle.y, -180.0f, 180.0f);
  angle.z = 0.0f;
}

void os2::math::NormalizeAngle(glm::vec3& angle) noexcept {
  angle.x = std::remainderf(angle.x, 180.f);
  angle.y = std::remainderf(angle.y, 360.f);
}

glm::vec3 os2::math::ToAngle(const glm::vec3& vec) noexcept {
  return {glm::degrees(std::atan2(-vec.z, std::hypot(vec.x, vec.y))),
          glm::degrees(std::atan2(vec.y, vec.y)), 0.00f};
}

glm::vec3 os2::math::CalculateRelativeAngle(
    const glm::vec3 source, const glm::vec3 dest,
    const glm::vec3& view_angles) noexcept {
  glm::vec3 delta = dest - source;

  ToAngle(delta);

  delta -= view_angles;

  NormalizeAngle(delta);

  return delta;
}

float os2::math::CalculateFOV(const glm::vec3& source,
                              const glm::vec3& dest) noexcept {
  glm::vec3 delta = dest - source;

  os2::math::NormalizeAngle(delta);

  return glm::length(delta);
}

float os2::math::CalculateAngleRadians(const glm::vec3& source,
                                       const glm::vec3& dest) noexcept {
  glm::vec3 delta = dest - source;

  os2::math::NormalizeAngle(delta);

  return std::atan2(delta.y, delta.x);
}

glm::vec3 os2::math::VecTransformMatrix(const glm::vec3& vec,
                                        const glm::mat3x4& mat) noexcept {
  return {
      vec.x * mat[0][0] + vec.y * mat[0][1] + vec.z * mat[0][2] + mat[0][3],
      vec.x * mat[1][0] + vec.y * mat[1][1] + vec.z * mat[1][2] + mat[1][3],
      vec.x * mat[2][0] + vec.y * mat[2][1] + vec.z * mat[2][2] + mat[2][3]};
}

float os2::math::AbsoluteDotProduct(const glm::vec3& source,
                                    const glm::vec3& rhs) noexcept {
  return std::abs(source.x * rhs.x) + std::abs(source.y * rhs.y) +
         std::abs(source.z * rhs.z);
}

template <typename T>
constexpr auto os2::math::deg2rad(T degrees) noexcept {
  return degrees * (std::numbers::pi_v<T> / static_cast<T>(180));
}

glm::vec3 os2::math::FromAngle(const glm::vec3& angle) noexcept {
  return glm::vec3{std::cos(deg2rad(angle.x)) * std::cos(deg2rad(angle.y)),
                   std::cos(deg2rad(angle.x)) * std::sin(deg2rad(angle.y)),
                   -std::sin(deg2rad(angle.x))};
}
