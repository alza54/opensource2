#pragma once

#include "../feature.hpp"

class TriggerBot : public Feature {
 public:
  TriggerBot() : Feature("TriggerBot") {}

  CONFIG_GETTER(bool, Enabled)
  CONFIG_GETTER(bool, DrawGUI)
  CONFIG_GETTER(bool, EnableTeamFire)

  void RenderUI() noexcept;

  void OnRender() noexcept override;
  void OnCreateMove(os2::sdk::CCSGOInput* pCsgoInput,
                    os2::sdk::CUserCmd* pUserCmd,
                    glm::vec3& view_angles) noexcept override;

  nlohmann::json ToJson() const override {
    return {{("enabled"), config_.Enabled},
            {("draw_gui"), config_.DrawGUI},
            {("enable_team_fire"), config_.EnableTeamFire}};
  }

  void FromJson(const nlohmann::json& j) override {
    config_.Enabled = j.at(("enabled")).get<bool>();
    config_.DrawGUI = j.at(("draw_gui")).get<bool>();
    config_.EnableTeamFire = j.at(("enable_team_fire")).get<bool>();
  }

 private:
  struct TriggerBotConfig {
    bool Enabled = false;
    bool DrawGUI = false;
    bool EnableTeamFire = false;
  } config_;
};
