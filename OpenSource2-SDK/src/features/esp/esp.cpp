#include <iomanip>
#include <sstream>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "../../sdk/interfaces/interfaces.hpp"

#include "../../game/state.hpp"
#include "../../gamedata/gamedata.hpp"
#include "../../menu/menu.hpp"
#include "../features.hpp"
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

  static const char* szESPType[] = {"2D Box", "3D Box"};
  static int iESPType = 0;

  switch (iESPType) {
    case 0:
      ESP::DrawPlayerBoxes() = true;
      ESP::DrawPlayerBoxes3D() = false;
      break;
    case 1:
      ESP::DrawPlayerBoxes3D() = true;
      ESP::DrawPlayerBoxes() = false;
      break;
  }

  ImGui::SeparatorText("Player ESP");
  ImGui::Combo(szESPType[iESPType], &iESPType, szESPType, 2);
  ImGui::Separator();
  ImGui::Checkbox("Players Skeleton", &ESP::DrawPlayerSkeleton());
  ImGui::Checkbox("Players Name", &ESP::DrawPlayerName());
  ImGui::Checkbox("Players Health-bar", &ESP::DrawPlayerHealthbar());
  ImGui::Checkbox("Active Weapon name", &ESP::DrawPlayerActiveWeaponName());

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

  static const char* szChamsType[] = {"Wireframe", "Primary White",
                                      "Reflectivity 90%", "Glass Transparent", "Simple Black"};

  ImGui::SeparatorText("Chams ESP");
  ImGui::Checkbox("Enable", &ESP::RenderChams());
  ImGui::Combo(szChamsType[ChamsMaterial()],
                 reinterpret_cast<int*>(&ESP::ChamsMaterial()), szChamsType, 5);

  pFeatures->drawing->RenderShadow(
      ImGui::GetWindowPos(), ImGui::GetWindowSize().x, ImGui::GetWindowSize().y,
      ImGui::GetStyle().WindowRounding, 4, 6);

  ImGui::PopFont();

  ImGui::End();
}

void ESP::OnRender() noexcept {
  if (DrawGUI()) RenderUI();

  if (!os2::iface::pEngine->IsInGame()) return;

  if (!Game::State::LocalController) return;

  if (!Game::State::LocalPawn) return;

  if (!Enabled()) return;

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
          it.shouldDrawEsp = pPlayerPawn->CalculateBBoxByCollision(it.boundingBox);
      } break;
      case EEntityType::BASE_WEAPON:
        it.shouldDrawEsp = pEntity->CalculateBBoxByCollision(it.boundingBox);
        break;
      case EEntityType::CHICKEN:
        it.shouldDrawEsp = pEntity->CalculateBBoxByCollision(it.boundingBox);
        break;
      case EEntityType::PLANTED_C4:
        it.shouldDrawEsp = pEntity->CalculateBBoxByCollision(it.boundingBox);
    }
  }
}

void RenderBox3D(const BBox_t& bBox) noexcept {
  for (int i = 0; i < 8; ++i) {
    for (int j = 1; j <= 4; j <<= 1) {
      if (!(i & j))
        g_pGameData->DrawList()->AddLine(bBox.m_Vertices[i],
                                         bBox.m_Vertices[i + j],
                                         IM_COL32(0, 255, 0, 255), 2.5f);
    }
  }

  for (int i = 0; i < 8; ++i) {
    for (int j = 1; j <= 4; j <<= 1) {
      if (!(i & j))
        g_pGameData->DrawList()->AddLine(bBox.m_Vertices[i],
                                         bBox.m_Vertices[i + j],
                                         IM_COL32(255, 255, 255, 255));
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

    if (os2::math::WorldToScreen(
            os2::sdk::Vector(model_state.bones[i].position.x,
                             model_state.bones[i].position.y,
                             model_state.bones[i].position.z),
            bone_screen_position) &&
        os2::math::WorldToScreen(
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

  const ImVec2 min = bBox.m_Mins;
  const ImVec2 max = bBox.m_Maxs;

  if (DrawPlayerBoxes()) {
    g_pGameData->DrawList()->AddRect(
        min - ImVec2{1.f, 1.f}, max + ImVec2{1.f, 1.f}, IM_COL32(0, 0, 0, 255));
    g_pGameData->DrawList()->AddRect(
        min + ImVec2{1.f, 1.f}, max - ImVec2{1.f, 1.f}, IM_COL32(0, 0, 0, 255));
    g_pGameData->DrawList()->AddRect(min, max,
                                     isLocalPlayer ? IM_COL32(52, 131, 235, 255)
                                     : isEnemy     ? IM_COL32(255, 0, 0, 255)
                                                   : IM_COL32(0, 255, 0, 255));
  } else if (DrawPlayerBoxes3D()) {
    RenderBox3D(bBox);
  }

  if (DrawPlayerName()) {
    const char* szName = pPlayerController->m_sSanitizedPlayerName();
    if (szName && strlen(szName) > 0) {
      std::string szRenderName = "";

      ImVec2 textSize = ImGui::CalcTextSize(szName);

      if (textSize.x > (max.x - min.x)) {
        const int textOverflow = static_cast<int>(
            std::floor(((max.x - min.x) - textSize.x) / ImGui::CalcTextSize(" ").x));

        if (textOverflow > 1) {
          szRenderName =
              std::string(szName).substr(0, strlen(szName) - textOverflow) +
              "...";

          textSize = ImGui::CalcTextSize(szRenderName.c_str());
        }
      } else {
        szRenderName = szName;
      }

      const ImVec2 textPos = ImFloor(
          {(min.x + max.x - textSize.x) / 2.f, min.y - textSize.y - 2.f});
      g_pGameData->DrawList()->AddText(
          textPos + ImVec2{1, 1}, IM_COL32(0, 0, 0, 255), szRenderName.c_str());
      g_pGameData->DrawList()->AddText(textPos, IM_COL32(255, 255, 255, 255),
                                       szRenderName.c_str());
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

  const ImVec2 min = bBox.m_Mins;
  const ImVec2 max = bBox.m_Maxs;

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

  std::string szRenderName = "";

  const char* szWeaponName =
      os2::iface::pLocalize->FindSafe(pItemStaticData->m_pszItemBaseName);
  if (!szWeaponName || strlen(szWeaponName) < 1) return;

  const ImVec2 min = bBox.m_Mins;
  const ImVec2 max = bBox.m_Maxs;

  ImVec2 textSize = ImGui::CalcTextSize(szWeaponName);

  if (textSize.x > (max.x - min.x)) {
    const int textOverflow = static_cast<int>(
        std::floor(((max.x - min.x) - textSize.x) / ImGui::CalcTextSize(" ").x));

    if (textOverflow > 1) {
      szRenderName = std::string(szWeaponName)
                         .substr(0, strlen(szWeaponName) - textOverflow) +
                     "...";

      textSize = ImGui::CalcTextSize(szRenderName.c_str());
    }
  } else {
    szRenderName = szWeaponName;
  }

  const ImVec2 textPos =
      ImFloor({(min.x + max.x - textSize.x) / 2.f, max.y + textSize.y - 12.f});

  g_pGameData->DrawList()->AddText(
      textPos + ImVec2{1, 1}, IM_COL32(0, 0, 0, 255), szRenderName.c_str());
  g_pGameData->DrawList()->AddText(textPos, IM_COL32(255, 255, 255, 255),
                                   szRenderName.c_str());
}

void ESP::RenderChickenESP(C_Chicken* pChicken, const BBox_t& bBox) noexcept {
  CHandle hLeader = pChicken->m_leader();

  const ImVec2 min = bBox.m_Mins;
  const ImVec2 max = bBox.m_Maxs;

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

void ESP::RenderPlantedC4(C_PlantedC4* pBomb, const BBox_t& bBox) noexcept {
  const ImVec2 min = bBox.m_Mins;
  const ImVec2 max = bBox.m_Maxs;

  if (!DrawPlantedBomb() || !pBomb->m_bBombTicking()) return;

  const ImVec2 vec1 = {1.f, 1.f};

  RenderBox3D(bBox);

  const auto pDraw = pFeatures->drawing.get();

  const float blowTime =
      pBomb->m_flC4Blow() - Game::State::GlobalVars->current_time;

  const ImVec2 drawPosition = {min.x, min.y + 20 + 6};

  if (DrawBombTimer()) {
    const auto color = pDraw->BombColorFade(blowTime, pBomb->m_flTimerLength());

    pDraw->RenderProgressBar({(min.x + max.x - 64.f) / 2.f, max.y + 20 + 6},
                             72.f, color, blowTime / pBomb->m_flTimerLength());
  }

  if (DrawBombTimerText()) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2) << blowTime << " s";

    const auto szBlowTime = ss.str();

    pDraw->RenderCenteredTextWithOutline(drawPosition, max.x - min.x, 20, szBlowTime);
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

      pDraw->RenderCenteredTextWithOutline(
          window_pos, win_x, win_y, canDefuse ? "Bomb can defused" : "Run!");
    } else {
      std::ostringstream ss;
      ss << std::fixed << std::setprecision(1) << "Bomb explodes in "
         << blowTime << " s";

      ImU32 color = pDraw->BombColorFade(blowTime, pBomb->m_flTimerLength());

      pDraw->RenderCenteredTextWithOutline(window_pos, win_x, win_y,
                                           ss.str().c_str(), color, 0x000000FF);
    }

    ImGui::End();
  }
}
