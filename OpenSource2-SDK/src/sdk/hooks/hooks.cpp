#include <thread>

#include "hooks.hpp"

#include "../memory/memory.hpp"
#include "../hooks/hook.hpp"

#include "../../features/game_hooks/game_hooks.hpp"

void os2::hooks::initialise() {
  os2::sdk::funchook_ctx = funchook_create();

  if (!os2::sdk::funchook_ctx) return;

  using namespace os2::hooks;

  input->Enter();
  os2::game::initialise();
  graphics->Enter();

  if (funchook_install(os2::sdk::funchook_ctx, 0) != FUNCHOOK_ERROR_SUCCESS)
    return;

  LOG(strings::log_hooks_initialised.c_str());
}

void os2::hooks::shutdown() {
  using namespace os2::hooks;

  g_isShuttingDown = true;

  if (!os2::sdk::funchook_ctx) return;

  input->Leave();
  os2::game::unhook();
  graphics->Leave();

  if (funchook_uninstall(os2::sdk::funchook_ctx, 0) != FUNCHOOK_ERROR_SUCCESS)
    return;

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  if (funchook_destroy(os2::sdk::funchook_ctx) != FUNCHOOK_ERROR_SUCCESS)
    return;
}
