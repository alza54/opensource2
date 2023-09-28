#include "recoil_control.hpp"

#include <imgui/imgui.h>

#include "../../game/state.hpp"
#include "../../menu/menu.hpp"

using namespace Game;
using namespace os2::sdk;

void RecoilControl::OnRender() noexcept { RenderUI(); }

void RecoilControl::RenderUI() noexcept {
  if (DrawPreview()) RenderRecoilPreview(ImVec2(500, 500));

  if (!DrawGUI() || !os2::menu::IsOpen()) return;

  ImGui::PushFont(os2::menu::fonts::libertad_mono);

  ImGui::SetNextWindowSize({500, 0});

  ImGui::Begin(this->Name(), &DrawPreview(),
               ImGuiWindowFlags_AlwaysAutoResize);

  ImGui::Checkbox("Enable Recoil Control", &Enabled());
  ImGui::Checkbox("Enable Recoil Control Preview", &DrawPreview());
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

static void SquareConstraint(ImGuiSizeCallbackData *data) {
  data->DesiredSize =
      ImVec2(std::max(data->DesiredSize.x, data->DesiredSize.y),
             std::max(data->DesiredSize.x, data->DesiredSize.y));
}

void RecoilControl::RenderRecoilPreview(ImVec2 vecWindowSize) noexcept {
  ImGui::SetNextWindowSize(vecWindowSize, ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX),
                                      SquareConstraint);

  auto windowFlags =
      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
      ImGuiStyleVar_FrameBorderSize | ImGuiWindowFlags_NoTitleBar;

  if (!os2::menu::IsOpen()) windowFlags |= ImGuiWindowFlags_NoInputs;

  if (ImGui::Begin("Radar", &DrawPreview(), windowFlags)) {
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    ImVec2 windowPosition = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();

    drawList->AddLine(
        ImVec2(windowPosition.x + windowPosition.x * 0.5f, windowPosition.y),
        ImVec2(windowPosition.x + windowSize.x * 0.5f,
               windowPosition.y + windowSize.y),
        ImColor(70, 70, 70, 255), 1.f);
    drawList->AddLine(
        ImVec2(windowPosition.x, windowPosition.y + windowSize.y * 0.5f),
        ImVec2(windowPosition.x + windowSize.x,
               windowPosition.y + windowSize.y * 0.5f),
        ImColor(70, 70, 70, 255), 1.f);

    drawList->AddLine(ImVec2(windowPosition.x + windowSize.x * 0.5f,
                             windowPosition.y + windowSize.y * 0.5f),
                      ImVec2(windowPosition.x, windowPosition.y),
                      ImColor(90, 90, 90, 255), 1.f);
    drawList->AddLine(ImVec2(windowPosition.x + windowSize.x * 0.5f,
                             windowPosition.y + windowSize.y * 0.5f),
                      ImVec2(windowPosition.x + windowSize.x, windowPosition.y),
                      ImColor(90, 90, 90, 255), 1.f);

    const auto centerX = windowPosition.x + windowSize.x * 0.5f;
    const auto centerY = windowPosition.y + windowSize.y * 0.5f;

    drawList->AddCircleFilled(ImVec2(centerX, centerY), 5.f, ImColor(255, 30, 30, 255));

    for (auto bullet : bulletsPre) {
      drawList->AddCircle(ImVec2(centerX + bullet.x, centerY + bullet.y), 3.f, ImColor(50, 0, 0, 200));
    }

    for (auto bullet : bulletsPost) {
      drawList->AddCircleFilled(ImVec2(centerX + bullet.x, centerY + bullet.y), 4.f,
                          ImColor(0, 222, 0, 222));
    }

    ImGui::End();
  }
}
