#pragma once

#include "../feature.hpp"

using namespace os2;

class ESP : public Feature {
 public:
  ESP() : Feature("ESP") {}

  CONFIG_GETTER(bool, Enabled)
  CONFIG_GETTER(bool, DrawGUI)
  CONFIG_GETTER(bool, DrawPlayerBoxes)
  CONFIG_GETTER(bool, DrawPlayerName)
  CONFIG_GETTER(bool, DrawPlayerHealthbar)
  CONFIG_GETTER(bool, DrawPlayerActiveWeaponName)
  CONFIG_GETTER(bool, DrawPlayerSkeleton)

  CONFIG_GETTER(bool, IgnoreTeammates)
  CONFIG_GETTER(bool, IgnoreEnemies)
  CONFIG_GETTER(bool, IgnoreSelf)

  CONFIG_GETTER(bool, DrawDroppedWeaponBoxes)
  CONFIG_GETTER(bool, DrawDroppedWeaponName)
  CONFIG_GETTER(float, MaxWeaponDistance)

  CONFIG_GETTER(bool, DrawPlantedBomb)
  CONFIG_GETTER(bool, DrawBombTimer)
  CONFIG_GETTER(bool, DrawBombTimerText)
  CONFIG_GETTER(bool, DrawBombTimerOverlay)

  CONFIG_GETTER(bool, DrawChickenBox)

  CONFIG_GETTER(bool, DrawAimFOV)
  CONFIG_GETTER(float, AimFOV)

  void RenderUI() noexcept;
  void OnRender() noexcept override;

  void OnCreateMove(os2::sdk::CCSGOInput*, os2::sdk::CUserCmd*,
                            glm::vec3&) noexcept override {}

  void CalculateBoundingBoxes() noexcept;

  unsigned int BombFadeColor(float x, float y) noexcept;

  void RenderShadow(ImVec2 position, float width, float height,
                    float rounding = 0.0f, int shadowOffset = 2,
                    int shadowLayers = 3,
                    unsigned int color = 0x20000000) noexcept;
  void RenderProgressBar(ImVec2 position, float width, unsigned int color,
                         float progress, bool shadow = true) noexcept;
  void RenderCenteredTextWithOutline(ImVec2 position, float width, float height,
                                     std::string text,
                                     unsigned int textColor = 0xFF000000,
                                     unsigned int textBorderColor = 0xFFFFFFFF,
                                     float offset = 1.0f) noexcept;

  nlohmann::json ToJson() const override {
    return {
        {("enabled"), config_.Enabled},
        {("draw"),
         {{("gui"), config_.DrawGUI},
          {("player_boxes"), config_.DrawPlayerBoxes},
          {("player_name"), config_.DrawPlayerName},
          {("player_healthbar"), config_.DrawPlayerHealthbar},
          {("player_active_weapon_name"), config_.DrawPlayerActiveWeaponName},
          {("player_skeleton"), config_.DrawPlayerSkeleton},
          {("dropped_weapon_boxes"), config_.DrawDroppedWeaponBoxes},
          {("dropped_weapon_name"), config_.DrawDroppedWeaponName},
          {("planted_bomb"), config_.DrawPlantedBomb},
          {("bomb_timer"), config_.DrawBombTimer},
          {("bomb_timer_text"), config_.DrawBombTimerText},
          {("bomb_timer_overlay"), config_.DrawBombTimerOverlay},
          {("chicken_box"), config_.DrawChickenBox},
          {("aim_fov"), config_.DrawAimFOV}}},
        {("ignore"),
         {{("teammates"), config_.IgnoreTeammates},
          {("enemies"), config_.IgnoreEnemies},
          {("self"), config_.IgnoreSelf}}},
        {("max_weapon_distance"), config_.MaxWeaponDistance},
        {("aim_fov_value"), config_.AimFOV}};
  }

  void FromJson(const nlohmann::json& j) override {
    config_.Enabled = j.at(("enabled")).get<bool>();

    const auto& draw = j.at("draw");
    config_.DrawGUI = draw.at(("gui")).get<bool>();
    config_.DrawPlayerBoxes = draw.at(("player_boxes")).get<bool>();
    config_.DrawPlayerName = draw.at(("player_name")).get<bool>();
    config_.DrawPlayerHealthbar = draw.at(("player_healthbar")).get<bool>();
    config_.DrawPlayerActiveWeaponName =
        draw.at(("player_active_weapon_name")).get<bool>();
    config_.DrawPlayerSkeleton = draw.at(("player_skeleton")).get<bool>();
    config_.DrawDroppedWeaponBoxes =
        draw.at(("dropped_weapon_boxes")).get<bool>();
    config_.DrawDroppedWeaponName =
        draw.at(("dropped_weapon_name")).get<bool>();
    config_.DrawPlantedBomb = draw.at(("planted_bomb")).get<bool>();
    config_.DrawBombTimer = draw.at(("bomb_timer")).get<bool>();
    config_.DrawBombTimerText = draw.at(("bomb_timer_text")).get<bool>();
    config_.DrawBombTimerOverlay = draw.at(("bomb_timer_overlay")).get<bool>();
    config_.DrawChickenBox = draw.at(("chicken_box")).get<bool>();
    config_.DrawAimFOV = draw.at(("aim_fov")).get<bool>();

    const auto& ignore = j.at("ignore");
    config_.IgnoreTeammates = ignore.at(("teammates")).get<bool>();
    config_.IgnoreEnemies = ignore.at(("enemies")).get<bool>();
    config_.IgnoreSelf = ignore.at(("self")).get<bool>();

    config_.MaxWeaponDistance = j.at(("max_weapon_distance")).get<float>();
    config_.AimFOV = j.at(("aim_fov_value")).get<float>();
  }

 private:
  void RenderSkeleton(sdk::C_CSPlayerPawn* pPawn) noexcept;
  void RenderPlayerESP(sdk::CCSPlayerController* pPlayerController,
                       const sdk::BBox_t& bBox) noexcept;
  void RenderWeaponESP(sdk::C_WeaponCSBase* pWeapon,
                       const sdk::BBox_t& bBox) noexcept;
  void RenderWeaponName(sdk::C_WeaponCSBase* pWeapon,
                        const sdk::BBox_t& bBox) noexcept;
  void RenderChickenESP(sdk::C_Chicken* pChicken,
                        const sdk::BBox_t& bBox) noexcept;
  void RenderPlantedC4(sdk::C_PlantedC4* pBomb,
                       const sdk::BBox_t& bBox) noexcept;

  struct EspConfig {
    bool Enabled = false;
    bool DrawGUI = false;
    bool DrawPlayerBoxes = false;
    bool DrawPlayerName = false;
    bool DrawPlayerHealthbar = false;
    bool DrawPlayerActiveWeaponName = false;
    bool DrawPlayerSkeleton = false;
    bool IgnoreTeammates = false;
    bool IgnoreEnemies = false;
    bool IgnoreSelf = true;
    bool DrawDroppedWeaponBoxes = false;
    bool DrawDroppedWeaponName = false;
    bool DrawPlantedBomb = false;
    bool DrawBombTimer = false;
    bool DrawBombTimerText = false;
    bool DrawBombTimerOverlay = false;
    bool DrawChickenBox = false;
    float MaxWeaponDistance = 0.0f;
    bool DrawAimFOV = false;
    float AimFOV = 0.0f;
  } config_;
};
