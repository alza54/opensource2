#pragma once

namespace os2::sdk {
  class CMaterialSystem2 {
   public:
    __forceinline CMaterial2*** find_or_create_material_from_resource(
        CMaterial2*** material, const char* material_name) {
      return CALL_VIRTUAL(CMaterial2***, 14, this, material, material_name);
    }
  };
};  // namespace os2::sdk
