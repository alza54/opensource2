#pragma once

#include "../feature.hpp"

class BunnyHop : public Feature {
 public:
  BunnyHop() : Feature("BunnyHop"), bWasLastTimeOnGround(false) {}

  CONFIG_GETTER(bool, Enabled)
  CONFIG_GETTER(bool, DrawGUI)
  CONFIG_GETTER(bool, DrawWatermark)
  CONFIG_GETTER(int, Perfection)

  void RenderUI() noexcept;

  void OnRender() noexcept override;
  void OnCreateMove(os2::sdk::CCSGOInput* pCsgoInput,
                    os2::sdk::CUserCmd* pUserCmd,
                    glm::vec3& view_angles) noexcept override;

  nlohmann::json ToJson() const override {
    return {{("enabled"), config_.Enabled},
            {("draw_gui"), config_.DrawGUI},
            {("draw_watermark"), config_.DrawWatermark},
            {("perfection"), config_.Perfection}};
  }

  void FromJson(const nlohmann::json& j) override {
    config_.Enabled = j.at(("enabled")).get<bool>();
    config_.DrawGUI = j.at(("draw_gui")).get<bool>();
    config_.DrawWatermark = j.at(("draw_watermark")).get<bool>();
    config_.Perfection = j.at(("perfection")).get<int>();
  }

 private:
  struct BunnyhopConfig {
    bool Enabled = false;
    bool DrawGUI = false;
    bool DrawWatermark = false;
    int Perfection = 100;
  } config_;

  bool bWasLastTimeOnGround;
};
