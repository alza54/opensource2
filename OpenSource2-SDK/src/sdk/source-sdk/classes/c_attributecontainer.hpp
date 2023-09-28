#pragma once

#include "econ/c_econitemview.hpp"

namespace os2::sdk {
  class C_AttributeContainer {
   public:
    PSCHEMA_FIELD(m_Item, "C_AttributeContainer", "m_Item", C_EconItemView);
  };
};  // namespace os2::sdk
