#pragma once

#include <cstdint>
#include <cstdio>

#include "../framework.hpp"

#include "../utilities/static_string.hpp"

#define CALL_VIRTUAL(retType, idx, ...) \
  os2::sdk::vmt::CallVirtual<retType>(idx, __VA_ARGS__)

namespace os2::sdk::vmt {
  namespace strings {
  inline const std::string log_vmt_null_class =
      os2_string("Tried getting virtual function from a null class.\n");
  inline const std::string log_vmt_null_vtable =
      os2_string("Tried getting virtual function from a null vtable.\n");
  inline const std::string log_vmt_null_vfunc =
      os2_string("Tried calling a null virtual function.\n");
  };  // namespace strings

  template <typename T = void*>
  inline T GetVMethod(uint32_t uIndex, void* pClass) {
    if (!pClass) {
      LOG(strings::log_vmt_null_class.c_str());
      return T{};
    }

    void** pVTable = *static_cast<void***>(pClass);
    if (!pVTable) {
      LOG(strings::log_vmt_null_vtable.c_str());
      return T{};
    }

    return reinterpret_cast<T>(pVTable[uIndex]);
  }

  template <typename T, typename... Args>
  inline T CallVirtual(uint32_t uIndex, void* pClass, Args... args) {
    auto pFunc = GetVMethod<T(__thiscall*)(void*, Args...)>(uIndex, pClass);
    if (!pFunc) {
      LOG(strings::log_vmt_null_vfunc.c_str());
      return T{};
    }

    return pFunc(pClass, args...);
  }
};  // namespace os2::sdk::vmt
