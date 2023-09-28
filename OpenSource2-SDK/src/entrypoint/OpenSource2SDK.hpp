#pragma once

#include <chrono>
#include <string>
#include <iostream>
#include <utility>

#include "../sdk/utilities/static_string.hpp"

typedef std::pair<std::string, std::chrono::duration<float>>
    WaitUntilModulesLoaded_t;

#define DEBUG_PREFIX os2::entrypoint::debug_console_prefix

namespace os2::entrypoint {
  const std::string game_path_part = os2_string(
      "Steam\\steamapps\\common\\Counter-Strike Global "
      "Offensive\\game");

  const std::string debug_console_title = os2_string("OpenSource2 SDK");

  const std::string debug_console_prefix = os2_string("[OpenSource2 SDK] ");

  namespace debug {
  /**
   * @brief "Loaded all modules, last: %s. Took %.2f s\n"
   */
  const std::string modules_loaded =
      os2_string("Loaded all modules, last: %s. Took %.2f s\n");
  }  // namespace debug

  /**
   * @brief Waits till the game loads all modules. Does not hardcode module names.
   * @param timeout Time after function considers the module loading ends
   * @param pool_rate Thread sleep time duration between enumerating modules
   * @return <Last Loaded Module Name, Execution Total Duration>
   */
  WaitUntilModulesLoaded_t WaitUntilModulesLoaded(
      std::chrono::duration<float> timeout = std::chrono::milliseconds(1500),
      std::chrono::duration<float> pool_rate = std::chrono::milliseconds(15));

  void BeginUnloadProcedure() noexcept;
};  // namespace os2::entrypoint
