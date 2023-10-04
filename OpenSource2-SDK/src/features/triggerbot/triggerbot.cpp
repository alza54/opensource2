#include "triggerbot.hpp"

#include "../../game/state.hpp"
#include "../../menu/menu.hpp"
#include "../../sdk/source-sdk/classes/entity/c_csweaponbase.hpp"
#include "../../sdk/source-sdk/classes/types/c_trace.hpp"

#include "../features.hpp"

using namespace Game;
using namespace os2::sdk;

void TriggerBot::OnRender() noexcept {
  TriggerBot::RenderUI();
}

void TriggerBot::RenderUI() noexcept {
  if (!DrawGUI() || !os2::menu::IsOpen()) return;

  ImGui::PushFont(os2::menu::fonts::libertad_mono);

  ImGui::SetNextWindowSize({700, 0});

  ImGui::Begin(this->Name(), &DrawGUI(), ImGuiWindowFlags_AlwaysAutoResize);

  ImGui::Checkbox("Enable TriggerBot", &Enabled());
  ImGui::Checkbox("Enable Team Fire", &EnableTeamFire());
  ImGui::SliderFloat("Max Flash Intensity", &MaxFlashIntensity(), 0, 1, "%.1f");
  ImGui::SliderFloat("Max Smoke Density", &MaxSmokeDensity(), 0, 1, "%.1f");
  ImGui::Checkbox("Scoped Only", &ScopedOnly());

  ImGui::PopFont();

  ImGui::End();
}

void TriggerBot::OnCreateMove(CCSGOInput* pCsgoInput, CUserCmd* pUserCmd,
                              glm::vec3& view_angles) noexcept {
  if (!Enabled()) return;

  if (!State::LocalController) return;

  if (!State::LocalPawn) return;

  if (!os2::iface::pEngine->IsInGame()) return;

  const auto pPawn = State::LocalPawn;

  CPlayer_WeaponServices* pWeaponServices = pPawn->m_pWeaponServices();
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

  const auto aimPunch = pPawn->m_aimPunchCache();
  glm::vec3 correction_vector{};

  if (aimPunch.m_size > 0 && aimPunch.m_size < 0xFFFF) {
    correction_vector = aimPunch.m_data[aimPunch.m_size - 1];
  }

  Vector start_pos = Vector{pPawn->GetEyePosition()};
  Vector end_pos =
      start_pos +
      Vector{os2::math::FromAngle(view_angles + correction_vector) * weapon_info->range()};

  C_TraceFilter filter(0x1C3003, pPawn, nullptr, 4);
  C_Ray ray;
  C_GameTrace trace;

  os2::fn::TraceShape(os2::fn::EngineTrace, &ray, &start_pos, &end_pos, &filter,
                      &trace);

  if (!trace.HitEntity) return;

  if (trace.HitEntity->m_iTeamNum() == 0) return;

  if (!EnableTeamFire() && trace.HitEntity->m_iTeamNum() == pPawn->m_iTeamNum())
    return;

  if (weapon_info->type() == CSWeaponType::SniperRifle && ScopedOnly() &&
      !State::LocalPawn->m_bIsScoped())
    return;

  float fFlashDuration =
      pPawn->m_flFlashBangTime()
          ? (pPawn->m_flFlashBangTime() - State::GlobalVars->current_time) /
                pPawn->m_flFlashDuration()
          : 0.f;

  const auto pDraw = pFeatures->drawing.get();

  if (fFlashDuration > MaxFlashIntensity()) {
    pDraw->RenderOnScreen(
        std::format("Flash: {}%", std::floor(fFlashDuration * 100)));
    return;
  }

  float fDensity = 0;
  // os2::fn::TraceSmoke(&start_pos, &end_pos, nullptr);

  if (fDensity > MaxSmokeDensity()) {
    pDraw->RenderOnScreen(std::format("Smoke: {}%", std::floor(fDensity * 100)),
                          ImVec4(255, 255, 0, 255));
    return;
  };

   pUserCmd->m_buttons |= IN_ATTACK;
}
