#pragma once

#include "../feature.hpp"

class RecoilControl : public Feature {
 public:
  CONSTRUCT_FEATURE(RecoilControl) {}

  CONFIG_GETTER(bool, Enabled)
  CONFIG_GETTER(bool, DrawGUI)
  CONFIG_GETTER(float, PerfectionX)
  CONFIG_GETTER(float, PerfectionY)

  void RenderUI() noexcept;

  void OnRender() noexcept override;
  void OnCreateMove(os2::sdk::CCSGOInput* pCsgoInput,
                    os2::sdk::CUserCmd* pUserCmd,
                    glm::vec3& view_angles) noexcept override;

  nlohmann::json ToJson() const override {
    return {{("enabled"), config_.Enabled},
            {("draw_gui"), config_.DrawGUI},
            {("perfection_x"), config_.PerfectionX},
            {("perfection_y"), config_.PerfectionY}};
  }

  void FromJson(const nlohmann::json& j) override {
    config_.Enabled = j.at(("enabled")).get<bool>();
    config_.DrawGUI = j.at(("draw_gui")).get<bool>();
    config_.PerfectionX = j.at(("perfection_x")).get<float>();
    config_.PerfectionY = j.at(("perfection_y")).get<float>();
  }

 private:
  struct RecoilControlConfig {
    bool Enabled = false;
    bool DrawGUI = false;
    float PerfectionX = 100.f;
    float PerfectionY = 100.f;
  } config_;

  std::vector<glm::vec3> bulletsPre{};
  std::vector<glm::vec3> bulletsPost{};
};
