#include <numbers>

#include "triggerbot.hpp"

#include "../../game/state.hpp"
#include "../../menu/menu.hpp"

#include "../../sdk/source-sdk/classes/entity/c_csweaponbase.hpp"
#include "../../sdk/source-sdk/classes/types/c_trace.hpp"

using namespace Game;
using namespace os2::sdk;

void TriggerBot::OnRender() noexcept { TriggerBot::RenderUI(); }

void TriggerBot::RenderUI() noexcept {
  if (!DrawGUI() || !os2::menu::IsOpen()) return;

  ImGui::PushFont(os2::menu::fonts::libertad_mono);

  ImGui::SetNextWindowSize({700, 0});

  ImGui::Begin(this->Name(), &DrawGUI(), ImGuiWindowFlags_AlwaysAutoResize);

  ImGui::Checkbox("Enable TriggerBot", &Enabled());
  ImGui::Checkbox("Enable Team Fire", &EnableTeamFire());

  ImGui::PopFont();

  ImGui::End();
}

template <typename T>
constexpr auto deg2rad(T degrees) noexcept {
  return degrees * (std::numbers::pi_v<T> / static_cast<T>(180));
}

static glm::vec3 fromAngle(const glm::vec3& angle) noexcept {
  return glm::vec3{
      std::cos(deg2rad(angle.x)) * std::cos(deg2rad(angle.y)),
      std::cos(deg2rad(angle.x)) * std::sin(deg2rad(angle.y)),
      -std::sin(deg2rad(angle.x))};
}

void TriggerBot::OnCreateMove(CCSGOInput* pCsgoInput, CUserCmd* pUserCmd,
                              glm::vec3& view_angles) noexcept {
  if (!Enabled()) return;

  if (!State::LocalController) return;

  if (!State::LocalPawn) return;

  if (!os2::iface::pEngine->IsInGame()) return;

  CPlayer_WeaponServices* pWeaponServices =
      State::LocalPawn->m_pWeaponServices();
  if (!pWeaponServices) return;

  C_CSWeaponBase* pWeapon =
      pWeaponServices->m_hActiveWeapon().Get<C_CSWeaponBase>();
  if (!pWeapon) return;

  if (pWeapon->m_nNextPrimaryAttackTick() > State::GlobalVars->tick_count) {
    return;
  }

  C_AttributeContainer* pAttributeContainer = pWeapon->m_AttributeManager();
  if (!pAttributeContainer) return;

  C_EconItemView* pItemView = pAttributeContainer->m_Item();
  if (!pItemView) return;

  CCSWeaponBaseVData* weapon_info = pItemView->GetWeaponInfo();
  if (!weapon_info) return;

  if (weapon_info->type() == CSWeaponType::Knife ||
      weapon_info->type() == CSWeaponType::C4 ||
      weapon_info->type() == CSWeaponType::Grenade ||
      static_cast<size_t>(weapon_info->type()) > 7)
    return;

  if (weapon_info->bulletsInClip() == 0) {
    return;
  }

  Vector start_pos = Vector{State::LocalPawn->GetEyePosition()};
  Vector end_pos =
      start_pos + Vector{fromAngle(view_angles) * weapon_info->range()};

  C_TraceFilter filter(0x1C3003, State::LocalPawn, nullptr, 4);
  C_Ray ray;
  C_GameTrace trace;

  os2::fn::TraceShape(os2::fn::EngineTrace, &ray, &start_pos, &end_pos, &filter,
                      &trace);

  if (!trace.HitEntity) return;

  if (trace.HitEntity->m_iTeamNum() == 0) return;

  if (!EnableTeamFire() &&
      trace.HitEntity->m_iTeamNum() == State::LocalPawn->m_iTeamNum())
    return;

  pUserCmd->m_buttons |= IN_ATTACK;
}
