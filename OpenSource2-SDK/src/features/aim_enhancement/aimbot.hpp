#pragma once

#include <map>

#include "../feature.hpp"

#include "../../sdk/source-sdk/classes/entity/c_csplayerpawn.hpp"
#include "../../sdk/source-sdk/classes/entity/ccsplayercontroller.hpp"

struct PlayerEntityBase_t {
  __int64 index;
  os2::sdk::C_CSPlayerPawn* pPawn;
  os2::sdk::CCSPlayerController* pController;
};

struct PlayerEntity_t {
  PlayerEntityBase_t base;
  std::string szPlayerName;
  std::string szPlaceName;
  uint8_t iTeam;
  float fDistance;
  float fFovDistance;
  bool bIsInFov;
  bool bIsVisible;
  float fDegAngle;
};

class AimBot : public Feature {
 public:
  CONSTRUCT_FEATURE(AimBot) {}

  CONFIG_GETTER(bool, Enabled)
  CONFIG_GETTER(bool, DrawGUI)
  CONFIG_GETTER(bool, DrawAimFOV)
  CONFIG_GETTER(float, FovRange)

  void RenderUI() noexcept;

  void OnRender() noexcept override;
  void OnCreateMove(os2::sdk::CCSGOInput* pCsgoInput,
                    os2::sdk::CUserCmd* pUserCmd,
                    glm::vec3& view_angles) noexcept override;
  void OnLevelInit() noexcept override;
  void OnRemoveEntity(os2::sdk::CEntityInstance* pInstance,
                      os2::sdk::CHandle hHandle) noexcept override;

  nlohmann::json ToJson() const override {
    return {{("enabled"), config_.Enabled},
            {("draw_gui"), config_.DrawGUI},
            {("draw_aim_fov"), config_.DrawAimFOV},
            {("fov_range"), config_.FovRange}};
  }

  void FromJson(const nlohmann::json& j) override {
    config_.Enabled = j.at(("enabled")).get<bool>();
    config_.DrawGUI = j.at(("draw_gui")).get<bool>();
    config_.DrawAimFOV = j.at(("draw_aim_fov")).get<bool>();
    config_.FovRange = j.at(("fov_range")).get<float>();
  }

 private:
  struct AimBotConfig {
    bool Enabled = false;
    bool DrawGUI = false;
    bool DrawAimFOV = true;
    float FovRange = 80.00f;
  } config_;

  std::map<int, PlayerEntity_t> entities_;
  std::mutex entities_mutex;

  void RenderAimFOV() noexcept;

  bool IsVisibleByTraceLine(os2::sdk::C_CSPlayerPawn* pLocalPawn,
                            const glm::vec3& vecTarget,
                            int iEntryIndex) noexcept;
};
