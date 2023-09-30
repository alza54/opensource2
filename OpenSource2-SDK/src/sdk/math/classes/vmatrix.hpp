#pragma once

#include <glm/mat4x4.hpp>

namespace os2::sdk {
  class VMatrix {
   public:
    inline glm::mat4x4 glm() const noexcept {
      return glm::mat4x4{m[0][0], m[0][1], m[0][2], m[0][3],
                         m[1][0], m[1][1], m[1][2], m[1][3],
                         m[2][0], m[2][1], m[2][2], m[2][3],
                         m[3][0], m[3][1], m[3][2], m[3][3]};
    }

    auto operator[](int i) const noexcept { return m[i]; }

    float m[4][4];
  };
};  // namespace os2::sdk
