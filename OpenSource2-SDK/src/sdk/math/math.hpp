#pragma once

#include "classes/vector.hpp"
#include "classes/vmatrix.hpp"
#include "types/quaternion.hpp"

struct ImVec2;

namespace os2::math {
  bool WorldToScreen(const os2::sdk::Vector& in, ImVec2& out) noexcept;
  void TransformAABB(const glm::mat3x4& mat, const glm::vec3& minsIn,
                     const glm::vec3& maxsIn, glm::vec3& minsOut,
                     glm::vec3& maxsOut) noexcept;
  void UpdateViewMatrix(os2::sdk::VMatrix* pViewMatrix) noexcept;
  void ClampAngle(glm::vec3& angle) noexcept;
  void NormalizeAngle(glm::vec3& angle) noexcept;
  float CalculateFOV(const glm::vec3& source, const glm::vec3& dest) noexcept;
  float CalculateAngleRadians(const glm::vec3& source,
                              const glm::vec3& dest) noexcept;
  glm::vec3 VecTransformMatrix(const glm::vec3& vec,
                               const glm::mat3x4& mat) noexcept;
  glm::vec3 ToAngle(const glm::vec3& vec) noexcept;
  glm::vec3 CalculateRelativeAngle(const glm::vec3 source,
                                   const glm::vec3 dest,
                                   const glm::vec3& view_angles) noexcept;
  float AbsoluteDotProduct(const glm::vec3& source,
                           const glm::vec3& rhs) noexcept;
  template <typename T>
  constexpr auto deg2rad(T degrees) noexcept;
  glm::vec3 FromAngle(const glm::vec3& angle) noexcept;
};  // namespace os2::math
