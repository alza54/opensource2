#pragma once

#include <memory>

#include <funchook/src/funchook.h>

#include "../virtual/virtual.hpp"

#define HOOK_FUNCTION(hook) hook, #hook

namespace os2::sdk {
  namespace strings {
    inline const std::string log_hook_success =
        os2_string("%s hooked successfully. [ %p -> %p ]\n");
    inline const std::string log_hook_fail = os2_string("%s hook failed.\n");
  };  // namespace strings

  inline funchook_t *funchook_ctx = nullptr;

  template <typename T>
  class CHook {
   public:
    // Template has been used to avoid casts.
    template <typename OriginalT, typename HookT>
    void Hook(OriginalT _pOriginalFn, HookT &pHookFn, const char *szHookName) {
      if (this->m_pOriginalFn) return;

      void *pOriginalFn = static_cast<void *>(_pOriginalFn);

      if (!pOriginalFn) return;

      this->m_pOriginalFn =
          reinterpret_cast<decltype(this->m_pOriginalFn)>(pOriginalFn);

      int rv = funchook_prepare(funchook_ctx,
                                reinterpret_cast<void **>(&this->m_pOriginalFn),
                                reinterpret_cast<void *>(pHookFn));

      if (rv == FUNCHOOK_ERROR_SUCCESS) {
        LOG(strings::log_hook_success.c_str(), szHookName, pOriginalFn, pHookFn);
      } else {
        this->m_pOriginalFn = nullptr;
        LOG(strings::log_hook_fail.c_str(), szHookName);
      }
    }

    template <typename HookT>
    void HookVirtual(void *pClass, int index, HookT &pHookFn,
                     const char *szHookName) {
      this->Hook(vmt::GetVMethod(index, pClass), pHookFn, szHookName);
    }

    std::add_pointer_t<T> m_pOriginalFn;
  };
};  // namespace os2::sdk
