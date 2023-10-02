#include <iomanip>
#include <sstream>

#include "../../sdk/interfaces/interfaces.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/trigonometric.hpp>

#include "../../game/state.hpp"
#include "../../gamedata/gamedata.hpp"
#include "../../menu/menu.hpp"
#include "esp.hpp"

#define ESP_WINDOW_WIDTH (const unsigned int)512

using namespace os2::sdk;
using namespace Game::Data;

void ESP::RenderUI() noexcept {
  using namespace os2::menu;

  if (!os2::menu::IsOpen()) return;

  ImGui::SetNextWindowPos(ImGui::GetIO().DisplaySize / 2.f, ImGuiCond_Once,
                          {0.5f, 0.5f});

  ImGui::PushFont(fonts::libertad_mono);

  ImGui::Begin("ESP", &ESP::DrawGUI(), ImGuiWindowFlags_AlwaysAutoResize);

  ImGui::SetNextItemWidth(ESP_WINDOW_WIDTH);

  ImGui::Checkbox("Enable ESP", &ESP::Enabled());

  ImGui::SeparatorText("Player ESP");
  ImGui::Checkbox("Players box", &ESP::DrawPlayerBoxes());
  ImGui::Checkbox("Players skeleton", &ESP::DrawPlayerSkeleton());
  ImGui::Checkbox("Players name", &ESP::DrawPlayerName());
  ImGui::Checkbox("Players health-bar", &ESP::DrawPlayerHealthbar());
  ImGui::Checkbox("Active weapon name", &ESP::DrawPlayerActiveWeaponName());

  ImGui::SeparatorText("ESP Filters");
  ImGui::Checkbox("Ignore teammates", &ESP::IgnoreTeammates());
  ImGui::Checkbox("Ignore enemies", &ESP::IgnoreEnemies());
  ImGui::Checkbox("Ignore yourself", &ESP::IgnoreSelf());

  ImGui::SeparatorText("Weapon ESP");
  ImGui::Checkbox("Weapons box", &ESP::DrawDroppedWeaponBoxes());
  ImGui::Checkbox("Weapons name", &ESP::DrawDroppedWeaponName());
  ImGui::TextUnformatted("Maximum weapon visibility range:");
  ImGui::SliderFloat("##sliderWeapon", &ESP::MaxWeaponDistance(), 0.f, 150.f,
                     "%.2f meters");

  ImGui::SeparatorText("Other ESP");
  ImGui::Checkbox("Chickens box", &ESP::DrawChickenBox());

  ImGui::SeparatorText("Bomb ESP");
  ImGui::Checkbox("Planted C4", &ESP::DrawPlantedBomb());
  ImGui::Checkbox("C4 Timer", &ESP::DrawBombTimer());
  ImGui::Checkbox("C4 Timer Text", &ESP::DrawBombTimerText());
  ImGui::Checkbox("C4 Timer Overlay", &ESP::DrawBombTimerOverlay());

  ImGui::SeparatorText("Aim");
  ImGui::SliderFloat("FOV", &ESP::AimFOV(), 0.f, 90.f, "%.1f");

  RenderShadow(ImGui::GetWindowPos(), ImGui::GetWindowSize().x,
               ImGui::GetWindowSize().y, ImGui::GetStyle().WindowRounding, 4,
               6);

  ImGui::PopFont();

  ImGui::End();
}

void ESP::OnRender() noexcept {
  if (DrawGUI()) RenderUI();

  if (!os2::iface::pEngine->IsInGame()) return;

  if (!Game::State::LocalController) return;

  if (!Game::State::LocalPawn) return;

  if (!Enabled()) return;

  if (DrawAimFOV()) {
    auto screen_size = os2::iface::pEngine->GetScreenSize();

    float radius =
        glm::tan(glm::radians(AimFOV()) / 2.f) /
        glm::tan(
            glm::radians(
                (float)Game::State::LocalPawn->m_pCameraServices()->m_iFOV()) /
            2.f) *
        screen_size.x;

    g_pGameData->DrawList()->AddCircleFilled(
        {glm::floor(screen_size.x / 2), glm::floor(screen_size.y / 2)}, radius,
        ImColor(0, 0, 0, 30));

    g_pGameData->DrawList()->AddCircle(
        {glm::floor(screen_size.x / 2), glm::floor(screen_size.y / 2)}, radius,
        ImColor(0, 0, 0, 60));
  }

  const std::lock_guard<std::mutex> guard{g_pGameData->GetEntitiesMutex()};

  for (const auto& it : g_pGameData->GetEntities()) {
    if (!it.shouldDrawEsp) continue;

    C_BaseEntity* pEntity = it.handle.Get();
    if (!pEntity) continue;

    // Additional sanity check.
    CHandle hEntity = pEntity->GetRefEHandle();
    if (hEntity != it.handle) continue;

    switch (it.type) {
      case EEntityType::PLAYER_CONTROLLER:
        RenderPlayerESP((CCSPlayerController*)pEntity, it.boundingBox);
        break;
      case EEntityType::BASE_WEAPON:
        RenderWeaponESP((C_CSWeaponBase*)pEntity, it.boundingBox);
        break;
      case EEntityType::CHICKEN:
        RenderChickenESP((C_Chicken*)pEntity, it.boundingBox);
        break;
      case EEntityType::PLANTED_C4:
        RenderPlantedC4((C_PlantedC4*)pEntity, it.boundingBox);
        break;
    }
  }
}

void ESP::CalculateBoundingBoxes() noexcept {
  if (!os2::iface::pEngine->IsInGame()) return;

  const std::lock_guard<std::mutex> guard{g_pGameData->GetEntitiesMutex()};

  for (auto& it : g_pGameData->GetEntities()) {
    C_BaseEntity* pEntity = it.handle.Get();
    if (!pEntity) continue;

    // Additional sanity check.
    CHandle hEntity = pEntity->GetRefEHandle();
    if (hEntity != it.handle) continue;

    switch (it.type) {
      case EEntityType::PLAYER_CONTROLLER: {
        C_CSPlayerPawn* pPlayerPawn =
            ((CCSPlayerController*)pEntity)->m_hPawn().Get<C_CSPlayerPawn>();
        if (pPlayerPawn)
          it.shouldDrawEsp = pPlayerPawn->GetBoundingBox(it.boundingBox, false);
      } break;
      case EEntityType::BASE_WEAPON:
        it.shouldDrawEsp = pEntity->GetBoundingBox(it.boundingBox, true);
        break;
      case EEntityType::CHICKEN:
        it.shouldDrawEsp = pEntity->GetBoundingBox(it.boundingBox, false);
        break;
      case EEntityType::PLANTED_C4:
        it.shouldDrawEsp = pEntity->GetBoundingBox(it.boundingBox, true);
    }
  }
}

void ESP::RenderSkeleton(C_CSPlayerPawn* pPawn) noexcept {
  CGameSceneNode* game_scene_node = pPawn->m_pGameSceneNode();

  if (game_scene_node == nullptr) return;

  CSkeletonInstance* skeleton_instance = game_scene_node->GetSkeletonInstance();

  if (skeleton_instance == nullptr) return;

  CModelState model_state = skeleton_instance->m_modelState();

  const CStrongHandle<CModel> model = model_state.modelHandle;

  if (!model.is_valid()) return;

  for (std::int32_t i = 0; i < model->BoneCount; ++i) {
    const BitFlag bone_flag = model->GetBoneFlags(i);

    // Check if the current bone has the hitbox flag set.
    if (!bone_flag.has_flag(static_cast<std::uint32_t>(FLAG_HITBOX))) continue;

    const std::int32_t bone_parent_index = model->GetBoneParent(i);

    // Skip bones with no parent.
    if (bone_parent_index == -1) continue;

    ImVec2 bone_screen_position;
    ImVec2 bone_screen_parent_position;

    if (os2::math::WorldToScreenV1(
            os2::sdk::Vector(model_state.bones[i].position.x,
                             model_state.bones[i].position.y,
                             model_state.bones[i].position.z),
            bone_screen_position) &&
        os2::math::WorldToScreenV1(
            os2::sdk::Vector(model_state.bones[bone_parent_index].position.x,
                             model_state.bones[bone_parent_index].position.y,
                             model_state.bones[bone_parent_index].position.z),
            bone_screen_parent_position)) {
      g_pGameData->DrawList()->AddLine(
          {bone_screen_position.x, bone_screen_position.y},
          {bone_screen_parent_position.x, bone_screen_parent_position.y},
          ImColor(255, 255, 255, 255));
    }
  }
}

void ESP::RenderPlayerESP(CCSPlayerController* pPlayerController,
                          const BBox_t& bBox) noexcept {
  if (!pPlayerController->m_bPawnIsAlive()) return;

  C_CSPlayerPawn* pPawn = pPlayerController->m_hPawn().Get<C_CSPlayerPawn>();
  if (!pPawn) return;

  const bool isLocalPlayer = pPlayerController->m_bIsLocalPlayerController();
  const bool isEnemy =
      pPawn->IsEnemyWithTeam(Game::State::LocalController->m_iTeamNum());

  // Filters
  if (IgnoreTeammates() && !isLocalPlayer && !isEnemy)
    return;
  else if (IgnoreEnemies() && isEnemy)
    return;
  else if (IgnoreSelf() && isLocalPlayer)
    return;

  const ImVec2 min = {bBox.x, bBox.y};
  const ImVec2 max = {bBox.w, bBox.h};

  if (DrawPlayerBoxes()) {
    g_pGameData->DrawList()->AddRect(
        min - ImVec2{1.f, 1.f}, max + ImVec2{1.f, 1.f}, IM_COL32(0, 0, 0, 255));
    g_pGameData->DrawList()->AddRect(
        min + ImVec2{1.f, 1.f}, max - ImVec2{1.f, 1.f}, IM_COL32(0, 0, 0, 255));
    g_pGameData->DrawList()->AddRect(min, max,
                                     isLocalPlayer ? IM_COL32(52, 131, 235, 255)
                                     : isEnemy     ? IM_COL32(255, 0, 0, 255)
                                                   : IM_COL32(0, 255, 0, 255));
  }

  if (DrawPlayerName()) {
    const char* szName = pPlayerController->m_sSanitizedPlayerName();
    if (szName && strlen(szName) > 0) {
      const ImVec2 textSize = ImGui::CalcTextSize(szName);
      const ImVec2 textPos = ImFloor(
          {(min.x + max.x - textSize.x) / 2.f, min.y - textSize.y - 2.f});
      g_pGameData->DrawList()->AddText(textPos + ImVec2{1, 1},
                                       IM_COL32(0, 0, 0, 255), szName);
      g_pGameData->DrawList()->AddText(textPos, IM_COL32(255, 255, 255, 255),
                                       szName);
    }
  }

  if (DrawPlayerSkeleton()) {
    RenderSkeleton(pPawn);
  }

  if (DrawPlayerHealthbar()) {
    const int iClampedHp = std::min(pPlayerController->m_iPawnHealth(), 100u);

    const ImVec2 barMin = min - ImVec2{5, 0};
    const ImVec2 barMax = ImVec2{min.x - 2, max.y};

    const float green = iClampedHp * 2.f * 2.55f;
    const float greenClamped = (std::min)(255.f, green);
    const float red = (std::min)(255.f, 510 - green);

    const float height = ((barMax.y - barMin.y) * iClampedHp) / 100.f;

    g_pGameData->DrawList()->AddRectFilled(
        barMin - ImVec2{0, 1}, barMax + ImVec2{0, 1}, IM_COL32(0, 0, 0, 255));
    if (iClampedHp > 0) {
      g_pGameData->DrawList()->AddRectFilled(
          ImVec2{barMin.x + 1, barMax.y - (std::max)(1.f, height)},
          ImVec2{barMax.x - 1, barMax.y}, IM_COL32(red, greenClamped, 0, 255));
    }
  }

  if (DrawPlayerActiveWeaponName()) {
    CPlayer_WeaponServices* pWeaponServices = pPawn->m_pWeaponServices();
    if (pWeaponServices) {
      C_CSWeaponBase* pActiveWeapon =
          pWeaponServices->m_hActiveWeapon().Get<C_CSWeaponBase>();
      if (pActiveWeapon) {
        RenderWeaponName(pActiveWeapon, bBox);
      }
    }
  }
}

void ESP::RenderWeaponESP(C_CSWeaponBase* pWeapon,
                          const BBox_t& bBox) noexcept {
  if (pWeapon->m_hOwnerEntity().IsValid()) return;

  if (MaxWeaponDistance() != 0.f &&
      Game::State::LocalPawn->DistanceToSquared(pWeapon) >=
          (MaxWeaponDistance() * MaxWeaponDistance()))
    return;

  const ImVec2 min = {bBox.x, bBox.y};
  const ImVec2 max = {bBox.w, bBox.h};

  if (DrawDroppedWeaponBoxes()) {
    g_pGameData->DrawList()->AddRect(
        min - ImVec2{1.f, 1.f}, max + ImVec2{1.f, 1.f}, IM_COL32(0, 0, 0, 255));
    g_pGameData->DrawList()->AddRect(
        min + ImVec2{1.f, 1.f}, max - ImVec2{1.f, 1.f}, IM_COL32(0, 0, 0, 255));
    g_pGameData->DrawList()->AddRect(min, max, IM_COL32(255, 255, 255, 255));
  }

  if (DrawDroppedWeaponName()) {
    RenderWeaponName(pWeapon, bBox);
  }
}

void ESP::RenderWeaponName(C_CSWeaponBase* pWeapon,
                           const BBox_t& bBox) noexcept {
  // Function to avoid spaghetti code.
  C_AttributeContainer* pAttributeContainer = pWeapon->m_AttributeManager();
  if (!pAttributeContainer) return;

  C_EconItemView* pItemView = pAttributeContainer->m_Item();
  if (!pItemView) return;

  CEconItemDefinition* pItemStaticData = pItemView->GetStaticData();
  if (!pItemStaticData) return;

  const char* szWeaponName =
      os2::iface::pLocalize->FindSafe(pItemStaticData->m_pszItemBaseName);
  if (!szWeaponName || strlen(szWeaponName) < 1) return;

  const ImVec2 min = {bBox.x, bBox.y};
  const ImVec2 max = {bBox.w, bBox.h};

  const ImVec2 textSize = ImGui::CalcTextSize(szWeaponName);
  const ImVec2 textPos =
      ImFloor({(min.x + max.x - textSize.x) / 2.f, max.y + textSize.y - 12.f});

  g_pGameData->DrawList()->AddText(textPos + ImVec2{1, 1},
                                   IM_COL32(0, 0, 0, 255), szWeaponName);
  g_pGameData->DrawList()->AddText(textPos, IM_COL32(255, 255, 255, 255),
                                   szWeaponName);
}

void ESP::RenderChickenESP(C_Chicken* pChicken, const BBox_t& bBox) noexcept {
  CHandle hLeader = pChicken->m_leader();

  const ImVec2 min = {bBox.x, bBox.y};
  const ImVec2 max = {bBox.w, bBox.h};

  if (DrawChickenBox()) {
    g_pGameData->DrawList()->AddRect(
        min - ImVec2{1.f, 1.f}, max + ImVec2{1.f, 1.f}, IM_COL32(0, 0, 0, 255));
    g_pGameData->DrawList()->AddRect(
        min + ImVec2{1.f, 1.f}, max - ImVec2{1.f, 1.f}, IM_COL32(0, 0, 0, 255));
    g_pGameData->DrawList()->AddRect(
        min, max,
        IM_COL32(hLeader == Game::State::LocalPawn->GetRefEHandle() ? 160 : 255,
                 0, 255, 255));
  }
}

void ESP::RenderShadow(ImVec2 position, float width, float height,
                       float rounding, int shadowOffset, int shadowLayers,
                       unsigned int shadowColor) noexcept {
  for (int i = 1; i <= shadowLayers; i++) {
    // Compute new color by reducing the alpha component for each layer
    unsigned int currentShadowColor =
        (shadowColor & 0xFFFFFF00) |
        (unsigned int)(((float)(shadowColor & 0xFF) *
                        (1.0f - (i / (float)shadowLayers))));

    // Expand the shadow size for each layer
    float offsetX = shadowOffset * (i / (float)shadowLayers);
    float offsetY = shadowOffset * (i / (float)shadowLayers);

    g_pGameData->DrawList()->AddRectFilled(
        ImVec2(position.x - offsetX, position.y + offsetY),
        ImVec2(position.x + width + offsetX, position.y + height + offsetY),
        currentShadowColor, rounding);
  }
}

void ESP::RenderCenteredTextWithOutline(ImVec2 position, float width,
                                        float height, std::string text,
                                        unsigned int textColor,
                                        unsigned int textBorderColor,
                                        float offset) noexcept {
  ImDrawList* drawList = g_pGameData->DrawList();

  const char* szText = text.c_str();

  ImGui::PushFont(os2::menu::fonts::cartograph);

  // Calculate text size
  ImVec2 textSizeVec = ImGui::CalcTextSize(szText);

  // Compute position for centered text
  ImVec2 textPos = ImVec2(position.x + (width - textSizeVec.x) * 0.5f,
                          position.y + (height - textSizeVec.y) * 0.5f);

  // Draw text outline/border
  for (float x = -offset; x <= offset; x += offset) {
    for (float y = -offset; y <= offset; y += offset) {
      // Skip rendering at center since that's where the main text will be
      if (x != 0.0f || y != 0.0f)
        drawList->AddText(ImVec2(textPos.x + x, textPos.y + y), textBorderColor,
                          szText);
    }
  }

  // Draw the centered text
  drawList->AddText(textPos, textColor, szText);

  ImGui::PopFont();
}

void ESP::RenderProgressBar(ImVec2 position, float width, unsigned int color,
                            float progress, bool shadow) noexcept {
  // Ensure progress is between 0 and 1
  progress = (progress < 0.0f) ? 0.0f : (progress > 1.0f) ? 1.0f : progress;

  ImDrawList* drawList = g_pGameData->DrawList();

  const int height = 20;        // This can be adjusted based on your preference
  const float rounding = 4.0f;  // Corner rounding
  const int borderWidth = 1;    // This can be adjusted
  const auto borderColor =
      IM_COL32(220, 220, 220, 255);  // Light gray color for border
  const auto bgColor = IM_COL32(50, 50, 50, 255);  // Dark gray for background

  if (shadow) RenderShadow(position, width, height, rounding, 4, 6);

  // Background
  drawList->AddRectFilled(position,
                          ImVec2(position.x + width, position.y + height),
                          bgColor, rounding);

  // Progress
  int progressWidth = static_cast<int>(progress * width);
  drawList->AddRectFilled(
      position, ImVec2(position.x + progressWidth, position.y + height), color,
      rounding, ImDrawFlags_RoundCornersAll);

  // Border
  drawList->AddRect(position, ImVec2(position.x + width, position.y + height),
                    borderColor, rounding, ImDrawFlags_RoundCornersAll,
                    borderWidth);
}

void ESP::RenderPlantedC4(C_PlantedC4* pBomb, const BBox_t& bBox) noexcept {
  const ImVec2 min = {bBox.x, bBox.y};
  const ImVec2 max = {bBox.w, bBox.h};

  if (!DrawPlantedBomb() || !pBomb->m_bBombTicking()) return;

  const ImVec2 vec1 = {1.f, 1.f};

  g_pGameData->DrawList()->AddRect(min - vec1, max + vec1,
                                   IM_COL32(255, 0, 0, 255));
  g_pGameData->DrawList()->AddRect(min + vec1, max - vec1,
                                   IM_COL32(255, 0, 0, 255));

  const float blowTime =
      pBomb->m_flC4Blow() - Game::State::GlobalVars->current_time;

  const ImVec2 drawPosition = {(min.x + max.x - bBox.w) / 2.f, max.y + 20 + 6};

  if (DrawBombTimer()) {
    const auto color = BombFadeColor(blowTime, pBomb->m_flTimerLength());

    RenderProgressBar({(min.x + max.x - 64.f) / 2.f, max.y + 20 + 6}, 72.f,
                      color, blowTime / pBomb->m_flTimerLength());
  }

  if (DrawBombTimerText()) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2) << blowTime << " s";

    const auto szBlowTime = ss.str();

    RenderCenteredTextWithOutline(drawPosition, bBox.w, 20, szBlowTime);
  }

  if (DrawBombTimerOverlay()) {
    ImGuiWindowFlags windowFlags =
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize;
    if (!os2::menu::IsOpen()) windowFlags |= ImGuiWindowFlags_NoInputs;

    ImGui::SetNextWindowBgAlpha(0.3f);
    ImGui::SetNextWindowPos({24.f, 24.f}, ImGuiCond_Once);
    ImGui::SetNextWindowSize({256.f, 64.f});
    ImGui::Begin("Bomb Watermark", nullptr, windowFlags);

    const auto [win_x, win_y] = ImGui::GetWindowSize();
    const auto window_pos = ImGui::GetWindowPos();

    if (pBomb->m_hBombDefuser().IsValid()) {
      const bool canDefuse =
          pBomb->m_flC4Blow() >= pBomb->m_flDefuseCountDown();

      auto pDefuser = pBomb->m_hBombDefuser().Get<CCSPlayerController>();

      if (pDefuser->GetRefEHandle().GetEntryIndex() ==
          Game::State::LocalController->GetRefEHandle().GetEntryIndex()) {
        RenderCenteredTextWithOutline(window_pos, win_x, win_y,
                                      canDefuse ? "You can defuse" : "Run!");
      } else {
        std::ostringstream ss;
        ss << pDefuser->m_sSanitizedPlayerName() << " defuses the bomb and"
           << (canDefuse ? "can" : "can't") << " defuse";

        RenderCenteredTextWithOutline(window_pos, win_x, win_y,
                                      ss.str().c_str());
      }
    } else {
      std::ostringstream ss;
      ss << std::fixed << std::setprecision(1) << "Bomb explodes in "
         << blowTime << " s";

      ImU32 color = BombFadeColor(blowTime, pBomb->m_flTimerLength());
      RenderCenteredTextWithOutline(window_pos, win_x, win_y, ss.str().c_str(),
                                    color, 0x000000FF);
    }

    ImGui::End();
  }
}

unsigned int ESP::BombFadeColor(float x, float y) noexcept {
  if (y <= 0)
    return IM_COL32(255, 0, 0, 255);  // If max is 0 or less, return full red.

  double z = static_cast<double>(x) / y;

  int r, g;

  if (z <= 0.05) {
    // Very dark red
    r = 255;
    g = 0;
  } else if (z <= 0.10) {
    // Transition between very dark red and very intense red
    double factor = (z - 0.05) * 20;
    r = 255;
    g = static_cast<int>(20 * (1 - factor));
  } else if (z <= 0.5) {
    // Transition between very intense red and regular green-to-red fade
    double factor = (z - 0.10) / 0.40;
    r = static_cast<int>(220 + (255 - 220) * factor);
    g = static_cast<int>(20 * (1 - factor));
  } else {
    // Regular green-to-red fade
    r = static_cast<int>(255.0 * (1 - z));
    g = static_cast<int>(255.0 * z);
  }

  return IM_COL32(r, g, 0, 255);
}
