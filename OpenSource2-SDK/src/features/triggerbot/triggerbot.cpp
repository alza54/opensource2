#include "triggerbot.hpp"

#include <numbers>

#include "../../game/state.hpp"
#include "../../menu/menu.hpp"
#include "../../sdk/source-sdk/classes/entity/c_csweaponbase.hpp"
#include "../../sdk/source-sdk/classes/types/c_trace.hpp"

// Extract RGBA components from a packed color
#define IM_COL32_EXTRACT_RED(col) ((col >> 0) & 0xFF)
#define IM_COL32_EXTRACT_GREEN(col) ((col >> 8) & 0xFF)
#define IM_COL32_EXTRACT_BLUE(col) ((col >> 16) & 0xFF)
#define IM_COL32_EXTRACT_ALPHA(col) ((col >> 24) & 0xFF)

using namespace Game;
using namespace os2::sdk;

void TriggerBot::OnRender() noexcept {
  this->lock_vector = true;

  for (auto it = this->displayed_info.begin(); it != this->displayed_info.end(); it++) {
    __int64 index = std::distance(this->displayed_info.begin(), it);
    DrawInfo((*it).first, index, (*it).second);
  }

  this->lock_vector = false;

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

template <typename T>
constexpr auto deg2rad(T degrees) noexcept {
  return degrees * (std::numbers::pi_v<T> / static_cast<T>(180));
}

static glm::vec3 fromAngle(const glm::vec3& angle) noexcept {
  return glm::vec3{std::cos(deg2rad(angle.x)) * std::cos(deg2rad(angle.y)),
                   std::cos(deg2rad(angle.x)) * std::sin(deg2rad(angle.y)),
                   -std::sin(deg2rad(angle.x))};
}

void TriggerBot::DrawInfo(const std::string& input, __int64 index,
                          unsigned int text_color) noexcept {
  ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

  ImGui::PushFont(os2::menu::fonts::red_hat_display_large);

  ImVec2 screen_size = ImGui::GetIO().DisplaySize;
  ImVec2 screen_center = ImVec2(screen_size.x * 0.5f, screen_size.y * 0.5f);
  ImVec2 text_size = ImGui::CalcTextSize(input.c_str());
  ImVec2 text_pos = screen_center;

  text_pos.x -= text_size.x * 0.5f;
  text_pos.y += (screen_size.y * 0.05f) + 14.0f * index;

  // Calculate the shadow color (material design style shadow is often
  // semi-transparent black)
  unsigned int shadow_color = IM_COL32(0, 0, 0, 128);

  // Shadow offset (typically to the bottom right for a material design effect)
  ImVec2 shadow_offset = ImVec2(2.0f, 2.0f);

  // Draw shadow
  draw_list->AddText(
      ImVec2(text_pos.x + shadow_offset.x, text_pos.y + shadow_offset.y),
      shadow_color, input.c_str());

  unsigned int border_color = IM_COL32(
      255 - IM_COL32_EXTRACT_RED(text_color),
      255 - IM_COL32_EXTRACT_GREEN(text_color),
      255 - IM_COL32_EXTRACT_BLUE(text_color),
               IM_COL32_EXTRACT_ALPHA(text_color)  // preserve original alpha
  );

  float border_thickness = 1.0f;
  // Draw contrasting border
  for (float y = -border_thickness; y <= border_thickness; y++) {
    for (float x = -border_thickness; x <= border_thickness; x++) {
      if (x != 0.0f || y != 0.0f)  // Avoid drawing the main text position here
        draw_list->AddText(ImVec2(text_pos.x + x, text_pos.y + y), border_color,
                           input.c_str());
    }
  }

  // Draw main text
  draw_list->AddText(text_pos, text_color, input.c_str());

  ImGui::PopFont();
}

void TriggerBot::OnCreateMove(CCSGOInput* pCsgoInput, CUserCmd* pUserCmd,
                              glm::vec3& view_angles) noexcept {
  // If cleared while iterating, bad things happen
  if (!this->lock_vector) this->displayed_info.clear();

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
      Vector{fromAngle(view_angles + correction_vector) * weapon_info->range()};

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

  if (fFlashDuration > MaxFlashIntensity()) {
    std::string _flash = std::format("Flash: {}%", std::floor(fFlashDuration * 100));
    this->displayed_info.emplace_back(std::make_pair(
        _flash, ImGui::ColorConvertFloat4ToU32(ImVec4(255, 0, 0, 255))));
    return;
  }

  float fDensity = os2::fn::TraceSmoke(&start_pos, &end_pos, nullptr);

  if (fDensity > MaxSmokeDensity()) {
    std::string _density = std::format("Smoke: {}%", std::floor(fDensity * 100));
    this->displayed_info.emplace_back(
        std::make_pair(_density, ImGui::ColorConvertFloat4ToU32(ImVec4(255, 255, 0, 255))));
    return;
  };

  pUserCmd->m_buttons |= IN_ATTACK;
}
