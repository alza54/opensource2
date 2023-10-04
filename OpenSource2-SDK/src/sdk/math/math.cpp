#include "math.hpp"
#include <numbers>

#include <glm/trigonometric.hpp>

#include <algorithm>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "../memory/memory.hpp"
#include "../interfaces/interfaces.hpp"

static glm::mat4x4 g_viewMatrix;

// An alternative way to calculate the world to screen matrix.
void os2::math::UpdateViewMatrix(os2::sdk::VMatrix* pViewMatrix) noexcept  {
  if (!pViewMatrix) return;
  g_viewMatrix = pViewMatrix->glm();
}

bool os2::math::WorldToScreenV1(const os2::sdk::Vector& in,
                                ImVec2& out) noexcept {
  if (!ImGui::GetCurrentContext()) return false;

  const float z = g_viewMatrix[3][0] * in.x + g_viewMatrix[3][1] * in.y +
                  g_viewMatrix[3][2] * in.z + g_viewMatrix[3][3];
  if (z < 0.001f) return false;

  out = ImGui::GetIO().DisplaySize * 0.5f;
  out.x *= 1.0f + (g_viewMatrix[0][0] * in.x + g_viewMatrix[0][1] * in.y +
                   g_viewMatrix[0][2] * in.z + g_viewMatrix[0][3]) /
                      z;
  out.y *= 1.0f - (g_viewMatrix[1][0] * in.x + g_viewMatrix[1][1] * in.y +
                   g_viewMatrix[1][2] * in.z + g_viewMatrix[1][3]) /
                      z;

  // So 'rounded' corners will not appear.
  out = ImFloor(out);
  return true;
}

bool os2::math::WorldToScreen(const glm::vec3& world_position,
                              glm::vec2& screen_position) noexcept {
  static auto& view_matrix =
      *reinterpret_cast<glm::mat4x4*>(os2::fn::ViewMatrixAddress);

  const glm::vec4 temp(world_position, 1.0f);

  // Perform the transformation calculations.
  screen_position.x = glm::dot(temp, view_matrix[0]);
  screen_position.y = glm::dot(temp, view_matrix[1]);

  const float w = glm::dot(temp, view_matrix[3]);

  // Check if the transformed point is behind the viewer's perspective.
  if (w < 0.001f) return false;

  const float inverse_w = 1.0f / w;

  // Apply perspective division to obtain normalized screen coordinates.
  screen_position.x *= inverse_w;
  screen_position.y *= inverse_w;

  const glm::vec2 screen_size = os2::iface::pEngine->GetScreenSize();

  // Get the center of the screen.
  float x = screen_size.x / 2.0f;
  float y = screen_size.y / 2.0f;

  // Calculate the final screen position.
  x += 0.5f * screen_position.x * screen_size.x + 0.5f;
  y -= 0.5f * screen_position.y * screen_size.y + 0.5f;

  screen_position.x = x;
  screen_position.y = y;

  return true;
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

template <typename T>
constexpr auto os2::math::deg2rad(T degrees) noexcept {
  return degrees * (std::numbers::pi_v<T> / static_cast<T>(180));
}

glm::vec3 os2::math::FromAngle(const glm::vec3& angle) noexcept {
  return glm::vec3{std::cos(deg2rad(angle.x)) * std::cos(deg2rad(angle.y)),
                   std::cos(deg2rad(angle.x)) * std::sin(deg2rad(angle.y)),
                   -std::sin(deg2rad(angle.x))};
}
