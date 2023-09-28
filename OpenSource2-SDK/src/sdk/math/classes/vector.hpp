#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <cmath>

namespace os2::sdk {
  class Vector {
   public:
    inline Vector operator+(const Vector& rhs) const noexcept {
      return Vector{this->x + rhs.x, this->y + rhs.y, this->z + rhs.z};
    }

    inline Vector operator-(const Vector& rhs) const noexcept {
      return Vector{this->x - rhs.x, this->y - rhs.y, this->z - rhs.z};
    }

    inline Vector operator*(const float rhs) const noexcept {
      return Vector{this->x * rhs, this->y * rhs, this->z * rhs};
    }

    inline Vector operator/(const float rhs) const noexcept {
      return Vector{this->x / rhs, this->y / rhs, this->z / rhs};
    }

    inline float Length2D() const noexcept { return std::sqrt(x * x + y * y); }

    inline glm::vec3 glm() const noexcept { return glm::vec3{x, y, z}; }

    float x, y, z;
  };

  class Vector4D {
   public:
    inline glm::vec4 glm() const noexcept { return glm::vec4{x, y, z, w}; }

    float x, y, z, w;
  };
};  // namespace os2::sdk
