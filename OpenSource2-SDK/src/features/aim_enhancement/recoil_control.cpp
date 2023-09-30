#include "recoil_control.hpp"

#include <imgui/imgui.h>

#include "../../game/state.hpp"
#include "../../menu/menu.hpp"

using namespace Game;
using namespace os2::sdk;

void RecoilControl::OnRender() noexcept { RenderUI(); }

void RecoilControl::RenderUI() noexcept {
  if (!DrawGUI() || !os2::menu::IsOpen()) return;

  ImGui::PushFont(os2::menu::fonts::libertad_mono);

  ImGui::SetNextWindowSize({500, 0});

  ImGui::Begin(this->Name(), &DrawGUI(), ImGuiWindowFlags_AlwaysAutoResize);

  ImGui::Checkbox("Enable Recoil Control", &Enabled());
  ImGui::SliderFloat("Perfection in X-axis", &PerfectionX(), 0.f, 100.f,
                     "%.3f");
  ImGui::SliderFloat("Perfection in Y-axis", &PerfectionY(), 0.f, 100.f,
                     "%.3f");

  ImGui::PopFont();

  ImGui::End();
}

void RecoilControl::OnCreateMove(CCSGOInput* pCsgoInput, CUserCmd* pUserCmd,
                                 glm::vec3& view_angles) noexcept {
  if (!Enabled()) return;

  if (!os2::iface::pEngine->IsInGame()) return;

  if (!State::LocalPawn) return;

  auto aimPunch = State::LocalPawn->m_aimPunchCache();

  if (aimPunch.m_size > 0 && aimPunch.m_size < 0xFFFF) {
    auto recoil = aimPunch.m_data[aimPunch.m_size - 1];

    static glm::vec3 prev = glm::vec3(0.f, 0.f, 0.f);

    auto delta = prev - recoil;

    delta.x *= PerfectionX() / 100.f;
    delta.y *= PerfectionY() / 100.f;

    this->bulletsPre.push_back(delta);

    view_angles += delta;

    os2::math::ClampAngle(view_angles);

    os2::fn::SetViewAngles(pCsgoInput, 0, view_angles);

    this->bulletsPost.push_back(view_angles);

    prev = recoil;
  } else {
    this->bulletsPre.clear();
    this->bulletsPost.clear();
  }
}
