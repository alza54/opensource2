#include <imgui/imgui.h>
#include <numbers>

#include <glm/trigonometric.hpp>

#include "aimbot.hpp"

#include "../../game/state.hpp"
#include "../../menu/menu.hpp"
#include "../features.hpp"

#include "../../gamedata/gamedata.hpp"

#include "../../sdk/source-sdk/classes/types/c_trace.hpp"

using namespace Game;
using namespace os2::sdk;

#define HEAD_BONE_INDEX 6

bool AimBot::IsVisibleByTraceLine(C_CSPlayerPawn* pLocalPawn,
                                  const glm::vec3& vecTarget,
                                  int iEntryIndex) noexcept {
  if (!pLocalPawn || vecTarget == glm::vec3{0.f, 0.f, 0.f}) return false;

  try {
    C_TraceFilter filter(0x1C3003, pLocalPawn, nullptr, 4);
    C_Ray ray = {};
    C_GameTrace trace = {};

    glm::vec3 ep = pLocalPawn->GetEyePosition();
    Vector vecStart{ep.x, ep.y, ep.z};
    Vector vecEnd{vecTarget};

    os2::fn::TraceShape(os2::fn::EngineTrace, &ray, &vecStart, &vecEnd, &filter,
                        &trace);

    return trace.HitEntity &&
           trace.HitEntity->GetRefEHandle().GetEntryIndex() == iEntryIndex;
  } catch (const std::exception& e) {
    LOG("%s\n", e.what());
    return false;
  }
}

void AimBot::OnRender() noexcept {
  RenderUI();
  RenderAimFOV();
}

void AimBot::OnLevelInit() noexcept {
  const std::lock_guard<std::mutex> guard{this->entities_mutex};

  this->entities_.clear();
}

void AimBot::OnRemoveEntity(os2::sdk::CEntityInstance* _pInstance,
                            os2::sdk::CHandle hHandle) noexcept {
  const std::lock_guard<std::mutex> guard{this->entities_mutex};

  this->entities_.erase(hHandle.GetEntryIndex());
}

inline bool IsAlive(PlayerEntity_t entity) noexcept {
  return entity.base.pController->m_bPawnIsAlive() &&
         entity.base.pController->m_iPawnHealth() > 0;
}

constexpr auto deg2rad(float degrees) noexcept {
  return degrees * (std::numbers::pi_v<float> / 180.0f);
}

void AimBot::RenderAimFOV() noexcept {
  if (!State::LocalPawn) return;

  if (!DrawAimFOV()) return;

  if (!os2::iface::pEngine->IsInGame()) return;

  const auto screen_size = os2::iface::pEngine->GetScreenSize();

  float radius =
      glm::tan(glm::radians(FovRange()) / 2.f) /
      glm::tan(glm::radians(this->pFeatures->fov_changer->InGameFOV() + 55.f) / 2.f) *
      screen_size.x;

  ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

  draw_list->AddCircleFilled(
      {glm::floor(screen_size.x / 2), glm::floor(screen_size.y / 2)}, radius,
      ImColor(0, 0, 0, 30));

  draw_list->AddCircle(
      {glm::floor(screen_size.x / 2), glm::floor(screen_size.y / 2)}, radius,
      ImColor(0, 0, 0, 60));

  for (const auto& [_name, entity] : entities_) {
    if (entity.iTeam == State::LocalPawn->m_iTeamNum() || !IsAlive(entity))
      continue;

    if (entity.bIsInFov) {
      pFeatures->drawing->RenderArrowToAngle(
          {glm::floor(screen_size.x / 2), glm::floor(screen_size.y / 2)},
          entity.fDegAngle, 10.f);
    }
  }
}

void AimBot::RenderUI() noexcept {
  if (!DrawGUI()) return;

  ImGui::PushFont(os2::menu::fonts::cartograph);

  ImGui::Begin(this->Name(), &DrawGUI(), ImGuiWindowFlags_None);

  ImGui::SetWindowSize({888.f, 0});

  ImGui::Checkbox("Enable Aimbot", &Enabled());

  ImGui::Checkbox("Draw FOV Range", &DrawAimFOV());

  ImGui::SliderFloat("FOV Range", &FovRange(), 0.f, 180.f, "%.2f", ImGuiSliderFlags_Logarithmic);

  if (os2::iface::pEngine->IsInGame() && State::LocalPawn) {
    const std::vector<std::string> headers = {
        "Name", "Team", "Place", "Distance", "FOV", "Is Visible", "Angle"};

    if (ImGui::BeginTable("Players", static_cast<int>(headers.size()),
                          ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
                              ImGuiTableFlags_SizingStretchSame |
                              ImGuiTableFlags_Resizable)) {
      for (const auto& header : headers) {
        ImGui::TableSetupColumn(header.c_str());
      }

      ImGui::TableHeadersRow();

      const std::lock_guard<std::mutex> guard{this->entities_mutex};

      const auto renderEntity = [](const std::vector<std::string>& headers,
                                   const PlayerEntity_t& entity) -> void {
        ImGui::TableNextRow();

        for (int i = 0; i < headers.size(); ++i) {
          const char* szHeader = headers[i].c_str();

          ImGui::TableNextColumn();

          switch (i) {
            case 0:
              ImGui::Text(entity.szPlayerName.c_str());
              break;
            case 1:
              ImGui::Text(entity.iTeam == 2 ? "T" : "CT");
              break;
            case 2:
              ImGui::Text(entity.szPlaceName.c_str());
              break;
            case 3:
              ImGui::Text("%.2f", entity.fDistance);
              break;
            case 4: {
              const ImVec4 color = entity.bIsInFov ? ImVec4(0, 255, 0, 255)
                                                   : ImVec4(255, 0, 0, 255);
              ImGui::TextColored(color, "%.2f", entity.fFovDistance);
              break;
            }
            case 5: {
              const ImVec4 color = entity.bIsVisible ? ImVec4(0, 255, 0, 255)
                                                     : ImVec4(255, 0, 0, 255);
              ImGui::TextColored(color, entity.bIsVisible ? "Yes" : "No");
              break;
            }
            case 6:
              ImGui::Text("%.2f deg", glm::degrees(entity.fDegAngle));
              break;
          }
        }
      };

      for (const auto& entity_ : entities_) {
        if (entity_.second.iTeam == State::LocalPawn->m_iTeamNum() ||
            !IsAlive(entity_.second))
          continue;

        renderEntity(headers, entity_.second);
      }

      ImGui::EndTable();
    }
  } else {
    ImGui::Text("Player list will render here once in game");
  }

  ImGui::PopFont();

  ImGui::End();
}

void AimBot::OnCreateMove(CCSGOInput* pCsgoInput, CUserCmd* pUserCmd,
                          glm::vec3& view_angles) noexcept {
  if (!Enabled()) return;

  if (!os2::iface::pEngine->IsInGame()) return;

  if (!State::LocalPawn) return;

  const auto& pLocalPawn = State::LocalPawn;

  if (!State::LocalController->m_bPawnIsAlive()) return;

  using namespace Game::Data;

  const std::lock_guard<std::mutex> guard{g_pGameData->GetEntitiesMutex()};

  const auto& entities = g_pGameData->GetEntities();

  const auto aimPunch = pLocalPawn->m_aimPunchCache();
  glm::vec3 correction_vector{};

  if (aimPunch.m_size > 0 && aimPunch.m_size < 0xFFFF) {
    correction_vector = aimPunch.m_data[aimPunch.m_size - 1];
  }

  for (auto it = entities.begin(); it != entities.end(); it++) {
    C_BaseEntity* pEntity = (*it).handle.Get();
    if (!pEntity) continue;

    // Additional sanity check.
    CHandle hEntity = pEntity->GetRefEHandle();
    if (hEntity != (*it).handle) continue;

    if ((*it).type != EEntityType::PLAYER_CONTROLLER) continue;

    CCSPlayerController* pController =
        static_cast<CCSPlayerController*>(pEntity);

    if (!pController->m_bPawnIsAlive()) continue;

    C_CSPlayerPawn* pPawn = pController->m_hPawn().Get<C_CSPlayerPawn>();
    if (!pPawn) continue;

    CGameSceneNode* game_scene_node = pPawn->m_pGameSceneNode();

    if (game_scene_node == nullptr) continue;

    glm::vec3 headPosition{};
    [[maybe_unused]] glm::vec4 headRotation{};

    if (!game_scene_node->GetBonePositionAndRotation(
            HEAD_BONE_INDEX, headPosition, headRotation))
      continue;

    const uint8_t iTeam = pPawn->m_iTeamNum();

    if (iTeam == pLocalPawn->m_iTeamNum()) continue;

    __int64 index = std::distance(entities.begin(), it);

    const std::string szPlayerName{pController->m_sSanitizedPlayerName()};

    const std::string szLastPlaceName{pPawn->m_szLastPlaceName()};

    const float fDistance = pPawn->DistanceToSquared(pLocalPawn);

    const glm::vec3 extrapolated_bone =
        headPosition + pPawn->m_vecVelocity().Length2D() *
                           State::GlobalVars->interval_per_tick;

    const glm::vec3 vecRelativeAngle = os2::math::CalculateRelativeAngle(
        pLocalPawn->GetEyePosition(), extrapolated_bone,
        view_angles + correction_vector);

    const float fFOV = std::hypot(vecRelativeAngle.x, vecRelativeAngle.y);

    const bool bIsInRange = fFOV <= this->FovRange();

    const bool bIsVisible = IsVisibleByTraceLine(pLocalPawn, extrapolated_bone,
                                                 hEntity.GetEntryIndex());

    const float fDegAngle =
        os2::math::CalculateAngleRadians(view_angles, headPosition);

    PlayerEntityBase_t entity_base{index, pPawn, pController};
    PlayerEntity_t entity{entity_base, szPlayerName, szLastPlaceName,
                          iTeam,       fDistance,    fFOV,
                          bIsInRange,  bIsVisible,   fDegAngle};

    const std::lock_guard<std::mutex> guard{this->entities_mutex};

    this->entities_.insert_or_assign(hEntity.GetEntryIndex(), entity);
  }
}
