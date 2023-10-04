#pragma once

#include "../feature.hpp"

class FovChanger : public Feature {
 public:
  CONSTRUCT_FEATURE(FovChanger) {}

  CONFIG_GETTER(bool, Enabled)
  CONFIG_GETTER(bool, DrawGUI)
  CONFIG_GETTER(bool, ChangeSceneFOV)
  CONFIG_GETTER(float, SceneCameraFOV)
  CONFIG_GETTER(float, InGameFOV)

  void RenderUI() noexcept;

  void SetSceneCameraFOV(float fov) noexcept;

  void OnRender() noexcept override;

  void OnCreateMove(os2::sdk::CCSGOInput* pCsgoInput,
                    os2::sdk::CUserCmd* pUserCmd,
                    glm::vec3& view_angles) noexcept override {}

  nlohmann::json ToJson() const override {
    return {{("enabled"), config_.Enabled},
            {("draw_gui"), config_.DrawGUI},
            {("change_scene_fov"), config_.ChangeSceneFOV},
            {("scene_camera_fov"), config_.SceneCameraFOV},
            {("in_game_fov", config_.InGameFOV)}};
  }

  void FromJson(const nlohmann::json& j) override {
    config_.Enabled = j.at(("enabled")).get<bool>();
    config_.DrawGUI = j.at(("draw_gui")).get<bool>();
    config_.ChangeSceneFOV = j.at(("change_scene_fov")).get<bool>();
    config_.SceneCameraFOV = j.at(("scene_camera_fov")).get<float>();
    config_.InGameFOV = j.at(("in_game_fov")).get<float>();
  }

 private:
  struct BunnyhopConfig {
    bool Enabled = false;
    bool DrawGUI = false;
    bool ChangeSceneFOV = false;
    float SceneCameraFOV = 100.f;
    float InGameFOV = 90.f;
  } config_;
};
