#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <cmath>

namespace os2::sdk {
  class Vector {
   public:
    Vector(const glm::vec3& vec) noexcept : x{vec.x}, y{vec.y}, z{vec.z} {}
    Vector(float x_, float y_, float z_) noexcept : x{x_}, y{y_}, z{z_} {}
    Vector() noexcept : x(0.f), y(0.f), z(0.f) {}

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
