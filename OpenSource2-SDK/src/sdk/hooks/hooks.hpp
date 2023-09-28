#pragma once

#include "wndproc/wndproc_hook.hpp"
#include "directx11/directx11_hook.hpp"

namespace os2::hooks {
  namespace strings {
    static inline const std::string log_hooks_initialised =
        os2_string("[+] Hooks Initialise() successful.\n");
  };  // namespace strings

  inline bool g_isShuttingDown = false;

  void initialise();
  void shutdown();

  inline std::shared_ptr<WndProcHook> input = std::make_shared<WndProcHook>();
  inline std::shared_ptr<DirectXHook> graphics = std::make_shared<DirectXHook>();
};  // namespace os2::hooks
