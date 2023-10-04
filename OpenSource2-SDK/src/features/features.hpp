#pragma once

#include <iostream>
#include <utility>

#include "aim_enhancement/recoil_control.hpp"
#include "aim_enhancement/aimbot.hpp"
#include "esp/esp.hpp"
#include "movement/bunnyhop.hpp"
#include "triggerbot/triggerbot.hpp"
#include "drawing/drawing.hpp"
#include "fov_changer/fov_changer.hpp"

class Features {
 public:
  std::unique_ptr<BunnyHop> bunnyHop = std::make_unique<BunnyHop>(this);
  std::unique_ptr<RecoilControl> recoilControl =
      std::make_unique<RecoilControl>(this);
  std::unique_ptr<AimBot> aimBot = std::make_unique<AimBot>(this);
  std::unique_ptr<TriggerBot> triggerBot = std::make_unique<TriggerBot>(this);
  std::unique_ptr<ESP> esp = std::make_unique<ESP>(this);
  std::unique_ptr<Drawing> drawing = std::make_unique<Drawing>(this);
  std::unique_ptr<FovChanger> fov_changer = std::make_unique<FovChanger>(this);

 public:
  void OnCreateMove(os2::sdk::CCSGOInput* pCsgoInput, os2::sdk::CUserCmd* cmd,
                    glm::vec3& view_angles) noexcept;

  [[maybe_unused]] void OnFrameStageNotify() noexcept;

  void OnRender() noexcept;

  void OnLevelInit() noexcept;

  void OnRemoveEntity(os2::sdk::CEntityInstance* pInstance,
                      os2::sdk::CHandle hHandle) noexcept;
};

inline std::optional<Features> gameFeatures;
