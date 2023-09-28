#pragma once

#include "classes/vector.hpp"
#include "classes/vmatrix.hpp"

struct ImVec2;

namespace os2::math {
  bool WorldToScreen(const glm::vec3& world_position, glm::vec2& screen_position);
  bool WorldToScreenV1(const os2::sdk::Vector& in, ImVec2& out);
  void UpdateViewMatrix(os2::sdk::VMatrix* pViewMatrix);
  void ClampAngle(glm::vec3& angle);
  void NormalizeAngle(glm::vec3& angle);
};  // namespace os2::math
