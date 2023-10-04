#include "fov_changer.hpp"

#include "../../menu/menu.hpp"

#include "../../game/state.hpp"

void FovChanger::OnRender() noexcept { FovChanger::RenderUI(); }

void FovChanger::RenderUI() noexcept {
  if (!DrawGUI() || !os2::menu::IsOpen()) return;

  ImGui::PushFont(os2::menu::fonts::libertad_mono);

  ImGui::Begin(this->Name(), &DrawGUI(), ImGuiWindowFlags_AlwaysAutoResize);

  ImGui::SeparatorText("Scene FOV - this is for fun :D");
  ImGui::Checkbox("Change Scene FOV", &ChangeSceneFOV());
  ImGui::SliderFloat("Scene FOV", &SceneCameraFOV(), 40, 140, "%.f");

  ImGui::SliderFloat("In-Game FOV", &InGameFOV(), 40, 140, "%.f");

  ImGui::PopFont();

  ImGui::End();
}

void FovChanger::SetSceneCameraFOV(float fov) noexcept {
  this->config_.SceneCameraFOV = fov;
}
