#pragma once

#include "vector.hpp"
#include "../types/quaternion.hpp"

class alignas(16) CTransform {
 public:
  alignas(16) os2::sdk::Vector m_Position;
  alignas(16) os2::sdk::Quaternion m_Orientation;

  glm::mat3x4 ToMatrix() const noexcept;
};
