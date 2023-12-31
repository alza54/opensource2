#pragma once

#include "../feature.hpp"

using namespace os2;

class ESP : public Feature {
 public:
  enum Material {
    WIREFRAME,
    PRIMARY_WHITE,
    REFLECTIVITY,
    GLASS_OVERLAY,
    BLACK_SIMPLE
  };

  CONSTRUCT_FEATURE(ESP) {}

  CONFIG_GETTER(bool, Enabled)
  CONFIG_GETTER(bool, DrawGUI)
  CONFIG_GETTER(bool, DrawPlayerBoxes)
  CONFIG_GETTER(bool, DrawPlayerBoxes3D)
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

  CONFIG_GETTER(bool, RenderChams)
  CONFIG_GETTER(Material, ChamsMaterial)

  void RenderUI() noexcept;
  void OnRender() noexcept override;

  void OnCreateMove(os2::sdk::CCSGOInput*, os2::sdk::CUserCmd*,
                    glm::vec3&) noexcept override {}

  void CalculateBoundingBoxes() noexcept;

  nlohmann::json ToJson() const override {
    return {
        {("enabled"), config_.Enabled},
        {("draw"),
         {{("gui"), config_.DrawGUI},
          {("player_boxes"), config_.DrawPlayerBoxes},
          {("player_boxes_3d"), config_.DrawPlayerBoxes3D},
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
          {("chicken_box"), config_.DrawChickenBox}}},
        {("ignore"),
         {{("teammates"), config_.IgnoreTeammates},
          {("enemies"), config_.IgnoreEnemies},
          {("self"), config_.IgnoreSelf}}},
        {("chams"),
         {{("enabled"), config_.RenderChams},
          {("material"), config_.ChamsMaterial}}},
        {("max_weapon_distance"), config_.MaxWeaponDistance}};
  }

  void FromJson(const nlohmann::json& j) override {
    config_.Enabled = j.at(("enabled")).get<bool>();

    const auto& draw = j.at("draw");
    config_.DrawGUI = draw.at(("gui")).get<bool>();
    config_.DrawPlayerBoxes = draw.at(("player_boxes")).get<bool>();
    config_.DrawPlayerBoxes3D = draw.at(("player_boxes_3d")).get<bool>();
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

    const auto& chams = j.at("chams");
    config_.RenderChams = chams.at(("render")).get<bool>();
    config_.ChamsMaterial = chams.at(("material")).get<Material>();

    const auto& ignore = j.at("ignore");
    config_.IgnoreTeammates = ignore.at(("teammates")).get<bool>();
    config_.IgnoreEnemies = ignore.at(("enemies")).get<bool>();
    config_.IgnoreSelf = ignore.at(("self")).get<bool>();

    config_.MaxWeaponDistance = j.at(("max_weapon_distance")).get<float>();
  }

 private:
  void RenderSkeleton(sdk::C_CSPlayerPawn* pPawn) noexcept;
  void RenderPlayerESP(sdk::CCSPlayerController* pPlayerController,
                       const sdk::BBox_t& bBox) noexcept;
  void RenderWeaponESP(sdk::C_CSWeaponBase* pWeapon,
                       const sdk::BBox_t& bBox) noexcept;
  void RenderWeaponName(sdk::C_CSWeaponBase* pWeapon,
                        const sdk::BBox_t& bBox) noexcept;
  void RenderChickenESP(sdk::C_Chicken* pChicken,
                        const sdk::BBox_t& bBox) noexcept;
  void RenderPlantedC4(sdk::C_PlantedC4* pBomb,
                       const sdk::BBox_t& bBox) noexcept;

  struct EspConfig {
    bool Enabled = false;
    bool DrawGUI = false;
    bool DrawPlayerBoxes = false;
    bool DrawPlayerBoxes3D = false;
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
    bool RenderChams = false;
    Material ChamsMaterial = Material::PRIMARY_WHITE;
    float MaxWeaponDistance = 0.0f;
  } config_;
};
