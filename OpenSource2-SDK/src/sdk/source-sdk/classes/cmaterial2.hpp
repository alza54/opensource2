#pragma once

namespace os2::sdk {
  class CMaterial2 {
   public:
    __forceinline const char* get_name() {
      return CALL_VIRTUAL(const char*, 0, this);
    }

    __forceinline const char* get_name_with_module() {
      return CALL_VIRTUAL(const char*, 1, this);
    }
  };
};  // namespace os2::sdk
