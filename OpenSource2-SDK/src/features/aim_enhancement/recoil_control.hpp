#pragma once

#include "../feature.hpp"

class RecoilControl : public Feature {
 public:
  RecoilControl()
      : Feature("Recoil Control") {}

  CONFIG_GETTER(bool, Enabled)
  CONFIG_GETTER(bool, DrawGUI)
  CONFIG_GETTER(bool, DrawPreview)
  CONFIG_GETTER(float, PerfectionX)
  CONFIG_GETTER(float, PerfectionY)

  void RenderUI() noexcept;
  void RenderRecoilPreview(ImVec2 vecWindowSize) noexcept;

  void OnRender() noexcept override;
  void OnCreateMove(os2::sdk::CCSGOInput* pCsgoInput,
                    os2::sdk::CUserCmd* pUserCmd,
                    glm::vec3& view_angles) noexcept override;

  nlohmann::json ToJson() const override {
    return {
      {("enabled"), config_.Enabled},
      {("draw_gui"), config_.DrawGUI},
      {("draw_preview"), config_.DrawPreview},
      {("perfection_x"), config_.PerfectionX},
      {("perfection_y"), config_.PerfectionY}
    };
  }

  void FromJson(const nlohmann::json& j) override {
    config_.Enabled = j.at(("enabled")).get<bool>();
    config_.DrawGUI = j.at(("draw_gui")).get<bool>();
    config_.DrawPreview = j.at(("draw_preview")).get<bool>();
    config_.PerfectionX = j.at(("perfection_x")).get<float>();
    config_.PerfectionY = j.at(("perfection_y")).get<float>();
  }

 private:
  struct RecoilControlConfig {
    bool Enabled = false;
    bool DrawGUI = false;
    bool DrawPreview = false;
    float PerfectionX = 100.f;
    float PerfectionY = 100.f;
  } config_;

  std::vector<glm::vec3> bulletsPre{};
  std::vector<glm::vec3> bulletsPost{};
};
