#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>

#include "menu.hpp"

#include "../entrypoint/OpenSource2SDK.hpp"

#include "../sdk/memory/memory.hpp"
#include "../sdk/interfaces/interfaces.hpp"
#include "../features/helpers/post_processing.hpp"

inline bool g_showTable = false;

#define MENU_WINDOW_WIDTH (const unsigned int)512

bool os2::menu::ShowUnloadPopup() {
  ImGui::OpenPopup("Are You Sure?");

  if (ImGui::BeginPopupModal("Are You Sure?", NULL,
                             ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text("Are you sure you want to continue?");
    ImGui::Spacing();

    if (ImGui::Button("Yes")) {
      os2::entrypoint::BeginUnloadProcedure();
    }

    ImGui::SameLine();

    if (ImGui::Button("No")) {
      return false;
    }

    ImGui::EndPopup();
  }

  return true;
}

void os2::menu::Render() {
  static char classInputText[64];

  ImGuiIO& IO = ImGui::GetIO();
  IO.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

  if (ImGui::IsKeyPressed(ImGuiKey_Insert, false)) Toggle(!IsOpen());

  if (!IsOpen()) return;

  IO.ConfigFlags = ImGuiConfigFlags_None;

  os2::fn::SDL_SetRelativeMouseMode(false);
  os2::fn::SDL_SetWindowGrab(os2::iface::pInputSystem->GetSDLWindow(), false);

  ImGui::SetNextWindowPos(IO.DisplaySize / 2.f, ImGuiCond_Once, {0.5f, 0.5f});

  ImGui::PushFont(fonts::libertad_mono);

  ImGui::Begin("Analytics", &bIsOpen, ImGuiWindowFlags_AlwaysAutoResize);

  // PostProcessing::performFullscreenBlur(ImGui::GetWindowDrawList(), 1.f);

  ImGui::Checkbox("Analytics Table", &g_showTable);

  if (g_showTable &&
      ImGui::BeginTable("StandardData", 2,
                        ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
                            ImGuiTableFlags_SizingStretchSame |
                            ImGuiTableFlags_Resizable)) {
    ImGui::TableSetupColumn("Param");
    ImGui::TableSetupColumn("Value");
    ImGui::TableHeadersRow();

    glm::vec2 screen = os2::iface::pEngine->GetScreenSize();

    char width[16];
    char height[16];

    sprintf_s(width, "%.0f px", screen.x);
    sprintf_s(height, "%.0f px", screen.y);

    const std::array<std::pair<const char*, const char*>, 4> data = {
      std::make_pair("Is Connected",
                      os2::iface::pEngine->IsConnected() ? "Yes" : "No"),
      std::make_pair("Is In Game",
                      os2::iface::pEngine->IsInGame() ? "Yes" : "No"),
      std::make_pair("Screen Width", width),
      std::make_pair("Screen Height", height)
    };

    for (auto i : data) {
      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      ImGui::Text(i.first);
      ImGui::TableNextColumn();
      ImGui::Text(i.second);
    }

    ImGui::EndTable();
  }

  ImGui::SeparatorText("Other settings");
  ImGui::SetNextItemWidth(MENU_WINDOW_WIDTH);
  ImGui::InputTextWithHint("##l0", "Class", classInputText,
                           IM_ARRAYSIZE(classInputText));

  if (ImGui::Button("Print class layout in game console",
                    {MENU_WINDOW_WIDTH, 0}))
    os2::fn::SchemaDetailedClassLayout(nullptr, classInputText);

  if (ImGui::IsItemHovered())
    ImGui::SetTooltip("Type 'C_BaseEntity' to understand what this does.");

  if (ImGui::Button("Unload", {MENU_WINDOW_WIDTH, 0})) {
    os2::entrypoint::BeginUnloadProcedure();
  }

  ImGui::PopFont();

  ImGui::End();
}

void os2::menu::Toggle(bool bState) {
  if (!ImGui::GetCurrentContext() || !os2::iface::pInputSystem) return;

  bIsOpen = bState;

  if (os2::iface::pInputSystem->IsRelativeMouseMode()) {
    const ImVec2 screenCenter = ImGui::GetIO().DisplaySize / 2.f;

    os2::fn::SDL_SetRelativeMouseMode(!bIsOpen);
    os2::fn::SDL_SetWindowGrab(os2::iface::pInputSystem->GetSDLWindow(), !bIsOpen);
    os2::fn::SDL_WarpMouseInWindow(nullptr, screenCenter.x, screenCenter.y);
  }
}

bool os2::menu::IsOpen() { return bIsOpen; }
