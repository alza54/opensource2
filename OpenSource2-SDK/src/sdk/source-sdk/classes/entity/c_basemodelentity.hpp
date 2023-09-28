#pragma once

#include "c_baseentity.hpp"

namespace os2::sdk {
  class C_BaseModelEntity : public C_BaseEntity {
   public:
    void SetModel(const char* name);
  };
};  // namespace os2::sdk
