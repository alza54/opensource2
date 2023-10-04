#pragma once

#include <imgui/imgui.h>

#include <iostream>
#include <sstream>
#include <string>

#include "../feature.hpp"

class TriggerBot : public Feature {
 public:
  CONSTRUCT_FEATURE(TriggerBot) {}

  CONFIG_GETTER(bool, Enabled)
  CONFIG_GETTER(bool, DrawGUI)
  CONFIG_GETTER(bool, EnableTeamFire)
  CONFIG_GETTER(float, MaxFlashIntensity)
  CONFIG_GETTER(float, MaxSmokeDensity)
  CONFIG_GETTER(bool, ScopedOnly)

  void RenderUI() noexcept;

  void OnRender() noexcept override;
  void OnCreateMove(os2::sdk::CCSGOInput* pCsgoInput,
                    os2::sdk::CUserCmd* pUserCmd,
                    glm::vec3& view_angles) noexcept override;

  nlohmann::json ToJson() const override {
    return {{("enabled"), config_.Enabled},
            {("draw_gui"), config_.DrawGUI},
            {("enable_team_fire"), config_.EnableTeamFire},
            {("max_flash_intensity"), config_.MaxFlashIntensity},
            {("max_smoke_density"), config_.MaxSmokeDensity},
            {("scoped_only", config_.ScopedOnly)}};
  }

  void FromJson(const nlohmann::json& j) override {
    config_.Enabled = j.at(("enabled")).get<bool>();
    config_.DrawGUI = j.at(("draw_gui")).get<bool>();
    config_.EnableTeamFire = j.at(("enable_team_fire")).get<bool>();
    config_.MaxFlashIntensity = j.at(("max_flash_intensity")).get<float>();
    config_.MaxSmokeDensity = j.at(("max_smoke_density")).get<float>();
    config_.ScopedOnly = j.at(("scoped_only")).get<bool>();
  }

 private:
  struct TriggerBotConfig {
    bool Enabled = false;
    bool DrawGUI = false;
    bool EnableTeamFire = false;
    float MaxFlashIntensity = 0.4f;
    float MaxSmokeDensity = 0.3f;
    bool ScopedOnly = true;
  } config_;
};
