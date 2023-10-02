#include "features.hpp"

#include "../menu/menu.hpp"
#include "../sdk/hooks/hooks.hpp"

#define IS_UNLOADING (os2::hooks::g_isShuttingDown)

void Features::OnCreateMove(os2::sdk::CCSGOInput* pCsgoInput, os2::sdk::CUserCmd* cmd,
                  glm::vec3& view_angles) noexcept {
  if (IS_UNLOADING) return;

  bunnyHop->OnCreateMove(pCsgoInput, cmd, view_angles);
  recoilControl->OnCreateMove(pCsgoInput, cmd, view_angles);
  triggerBot->OnCreateMove(pCsgoInput, cmd, view_angles);
}

[[maybe_unused]] void Features::OnFrameStageNotify() noexcept {}

void Features::OnRender() noexcept {
  if (IS_UNLOADING) return;

  ImGui::PushFont(os2::menu::fonts::red_hat_display);

  esp->OnRender();
  bunnyHop->OnRender();
  recoilControl->OnRender();
  triggerBot->OnRender();

  ImGui::PopFont();
}
