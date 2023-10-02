#pragma once

#include <iostream>
#include <utility>

#include "aim_enhancement/recoil_control.hpp"
#include "esp/esp.hpp"
#include "movement/bunnyhop.hpp"
#include "triggerbot/triggerbot.hpp"

class Features {
 public:
  std::unique_ptr<BunnyHop> bunnyHop = std::make_unique<BunnyHop>();
  std::unique_ptr<RecoilControl> recoilControl =
      std::make_unique<RecoilControl>();
  std::unique_ptr<TriggerBot> triggerBot = std::make_unique<TriggerBot>();
  std::unique_ptr<ESP> esp = std::make_unique<ESP>();

 public:
  void OnCreateMove(os2::sdk::CCSGOInput* pCsgoInput, os2::sdk::CUserCmd* cmd,
                    glm::vec3& view_angles) noexcept;

  [[maybe_unused]] void OnFrameStageNotify() noexcept;

  void OnRender() noexcept;
};
