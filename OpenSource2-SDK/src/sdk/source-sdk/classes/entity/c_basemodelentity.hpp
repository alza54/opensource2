#pragma once

#include "c_baseentity.hpp"

namespace os2::sdk {
  class C_BaseModelEntity : public C_BaseEntity {
   public:
    void SetModel(const char* name);

    SCHEMA_FIELD(m_vecViewOffset, "C_BaseModelEntity", "m_vecViewOffset",
                 glm::vec3);
  };
};  // namespace os2::sdk
