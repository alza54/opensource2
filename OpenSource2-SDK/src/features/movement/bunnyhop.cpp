#include "bunnyhop.hpp"

#include "../../game/state.hpp"
#include "../../menu/menu.hpp"

using namespace Game;
using namespace os2::sdk;

void BunnyHop::OnRender() noexcept { BunnyHop::RenderUI(); }

void BunnyHop::RenderUI() noexcept {
  if (DrawWatermark()) {
    ImGui::PushFont(os2::menu::fonts::libertad_mono);

    ImGuiWindowFlags windowFlags =
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize;
    if (!os2::menu::IsOpen()) windowFlags |= ImGuiWindowFlags_NoInputs;

    ImGui::SetNextWindowBgAlpha(0.3f);
    ImGui::SetNextWindowPos({ImGui::GetIO().DisplaySize.x - 300.f, 24.f},
                            ImGuiCond_Once);
    ImGui::Begin("Watermark", nullptr, windowFlags);

    if (os2::iface::pEngine->IsInGame()) {
      ImGui::Text("OpenSource2 | speed %.2f | map %s",
                  State::LocalPawn != nullptr
                      ? State::LocalPawn->m_vecVelocity().Length2D()
                      : -1.f,
                  State::GlobalVars->current_map_name);
    } else {
      ImGui::Text("OpenSource2 | Not In Game");
    }

    ImGui::PopFont();

    ImGui::End();
  }

  if (!DrawGUI() || !os2::menu::IsOpen()) return;

  ImGui::PushFont(os2::menu::fonts::libertad_mono);

  ImGui::Begin(this->Name(), &DrawGUI(), ImGuiWindowFlags_AlwaysAutoResize);

  ImGui::Checkbox("Enable Auto BunnyHop", &Enabled());
  ImGui::Checkbox("Draw Watermark", &DrawWatermark());
  ImGui::SliderInt("BunnyHop Perfection", &Perfection(), 1, 100, "%i/100");

  ImGui::PopFont();

  ImGui::End();
}

void BunnyHop::OnCreateMove(CCSGOInput* pCsgoInput, CUserCmd* pUserCmd,
                            glm::vec3& view_angles) noexcept {
  if (!State::LocalPawn) return;

  if (!Enabled()) return;

  this->bWasLastTimeOnGround = State::LocalPawn->OnGround();

  const float velocity = State::LocalPawn->m_vecVelocity().Length2D();

  bool bShouldJump = true;

  if (velocity > 300.f && Perfection() < 0.8) bShouldJump = false;

  if (!State::LocalPawn->OnGround() && !bWasLastTimeOnGround)
    pUserCmd->m_buttons &= ~os2::sdk::IN_JUMP;

  this->bWasLastTimeOnGround = State::LocalPawn->OnGround();
}
