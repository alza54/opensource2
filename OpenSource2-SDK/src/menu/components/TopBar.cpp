#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui_internal.h>
#include <imgui/imgui.h>

#include <fonts/icons/IconsMaterialDesignIcons.h>

#include "../menu.hpp"
#include "../../features/features.hpp"

using namespace os2::menu;

void TopBar::MenuItemEx(const char* name, const char icon[], bool& stateVariable,
                      const char* shortcut, bool enabled) noexcept {
  if (ImGui::MenuItemEx(name, icon, shortcut, stateVariable, enabled)) {
    stateVariable = !stateVariable;

    if (stateVariable) {
      ImGui::SetWindowFocus(name);
      ImGui::SetWindowPos(name, {100.0f, 100.0f});
    }
  }
}

void TopBar::Render() noexcept {
  if (!IsOpen()) return;

  ImGuiStyle& style = ImGui::GetStyle();

  ImStyleCache* style_cache = new ImStyleCache(style);

  style.FramePadding.y = 16;
  style.FramePadding.x = 16;
  style.ItemSpacing.x = 12;
  style.ItemSpacing.y = 4;

  style.IndentSpacing = 25;
  style.ScrollbarSize = 15;
  style.GrabMinSize = 10;
  style.WindowBorderSize = 1;
  style.ChildBorderSize = 1;
  style.PopupBorderSize = 1;
  style.FrameBorderSize = 1;
  style.TabBorderSize = 1;
  style.WindowRounding = 7;
  style.ChildRounding = 4;
  style.FrameRounding = 3;
  style.PopupRounding = 4;
  style.ScrollbarRounding = 9;
  style.GrabRounding = 3;
  style.LogSliderDeadzone = 4;
  style.TabRounding = 4;

  if (!ImGui::BeginMainMenuBar()) {
    style_cache->restore(style);
    return;
  }

  ImGui::PushFont(os2::menu::fonts::cartograph);

  ImGui::TextColored(ImVec4(BRAND_COLOR, 1.f), ICON_MDI_CROWN_OUTLINE);

  ImGui::PopFont();

  ImGui::PushFont(os2::menu::fonts::libertad_mono);
  ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 0.96f), "OpenSource2");
  ImGui::TextColored(ImVec4(BRAND_COLOR, 0.87f), "SDK");

  ImGui::PopFont();
  ImGui::PushFont(os2::menu::fonts::cartograph);

  if (ImGui::BeginMenu("Aim")) {
    TopBar::MenuItemEx("Aim Assistance", ICON_MDI_ROBOT,
                       gameFeatures->aimBot->DrawGUI(),
                       "Aimbot");
    TopBar::MenuItemEx("Auto Shoot", ICON_MDI_PISTOL,
                       gameFeatures->triggerBot->DrawGUI(),
                     "Triggerbot");
    TopBar::MenuItemEx("Recoil Control", ICON_MDI_LESS_THAN,
                       gameFeatures->recoilControl->DrawGUI(),
                     "NoRecoil", true);
    TopBar::MenuItemEx("Spread Control", ICON_MDI_SHAKER,
                       gameFeatures->bunnyHop->DrawGUI(),
                       "NoSpread", false);
    TopBar::MenuItemEx("Prediction", ICON_MDI_MATRIX,
                       gameFeatures->bunnyHop->DrawGUI(),
                       "",
                       false);
    TopBar::MenuItemEx("Self-learning progress", ICON_MDI_LAYERS_TRIPLE,
                       gameFeatures->bunnyHop->DrawGUI(), "", false);

    TopBar::MenuItemEx("General Statistics", ICON_MDI_STAR_SHOOTING,
                       gameFeatures->bunnyHop->DrawGUI(), "", false);

    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Visuals")) {
    TopBar::MenuItemEx("Players ESP", ICON_MDI_ACCOUNT_EYE_OUTLINE,
                     gameFeatures->esp->DrawGUI());
    TopBar::MenuItemEx("Bomb ESP", ICON_MDI_BOMB, gameFeatures->esp->DrawGUI());
    TopBar::MenuItemEx("Chicken ESP", ICON_MDI_TURKEY,
                       gameFeatures->esp->DrawGUI());
    TopBar::MenuItemEx("FOV Changer", ICON_MDI_EYE,
                       gameFeatures->fov_changer->DrawGUI());
    ImGui::MenuItem("Custom Chams", ICON_MDI_PALETTE_ADVANCED);
    ImGui::MenuItem("Player Radar", ICON_MDI_RADAR);
    ImGui::MenuItem("World 3D Helpers", ICON_MDI_PRINTER_3D);
    ImGui::MenuItem("Grenade ESP", ICON_MDI_SCAN_HELPER);
    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Movement")) {
    TopBar::MenuItemEx("BunnyHop", ICON_MDI_RABBIT, gameFeatures->bunnyHop->DrawGUI());
    ImGui::MenuItem("Auto Strafe");
    ImGui::MenuItem("Edge Jump");
    ImGui::MenuItem("Auto OP Jumps");
    ImGui::MenuItem("Auto Stop");
    ImGui::MenuItem("Auto Slow");
    ImGui::MenuItem("Auto Crouch");
    ImGui::MenuItem("Auto Defuse");
    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("General")) {
    ImGui::MenuItem("3D Sound ESP");
    ImGui::MenuItem("Custom Sky");
    ImGui::MenuItem("Night Mode");
    ImGui::MenuItem("Custom Models");
    ImGui::MenuItem("Web ESP");
    ImGui::MenuItem("LED Physical Integration");
    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Skin Changer")) {
    ImGui::MenuItem("Skin Changer");
    ImGui::MenuItem("Inventory Changer");
    ImGui::MenuItem("Fake Openings");
    ImGui::MenuItem("Intercept Skins from Inspect");
    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Config")) {
    ImGui::MenuItem("Instant New Save");
    ImGui::MenuItem("Open Config Manager");
    ImGui::MenuItem("Unload Cheat");
    ImGui::EndMenu();
  }

  ImGui::PopFont();
  // ImGui::PopFont();

  // End the main menu bar
  ImGui::EndMainMenuBar();

  style_cache->restore(style);

  delete style_cache;
}
