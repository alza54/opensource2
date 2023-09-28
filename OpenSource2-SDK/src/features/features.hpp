#pragma once

#include <iostream>
#include <utility>

#include "movement/bunnyhop.hpp"
#include "aim_enhancement/recoil_control.hpp"
#include "esp/esp.hpp"

#include "../sdk/hooks/hooks.hpp"

#define IS_UNLOADING (os2::hooks::g_isShuttingDown)

class Features {
 public:
  std::unique_ptr<BunnyHop> bunnyHop = std::make_unique<BunnyHop>();
  std::unique_ptr<RecoilControl> recoilControl = std::make_unique<RecoilControl>();
  std::unique_ptr<ESP> esp = std::make_unique<ESP>();

 public:
  void OnCreateMove(os2::sdk::CCSGOInput* pCsgoInput, os2::sdk::CUserCmd* cmd, glm::vec3& view_angles) noexcept {
    if (IS_UNLOADING) return;

    bunnyHop->OnCreateMove(pCsgoInput, cmd, view_angles);
    recoilControl->OnCreateMove(pCsgoInput, cmd, view_angles);
  }

  [[maybe_unused]] void OnFrameStageNotify() noexcept {}

  void OnRender() noexcept {
    if (IS_UNLOADING) return;

    bunnyHop->OnRender();
    recoilControl->OnRender();
    esp->OnRender();
  }
};
