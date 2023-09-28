#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <implot/implot.h>

#include <stdint.h>

#include <fonts/cartograph_font.hpp>
#include <fonts/libertad_font.hpp>

#include <fonts/icons/IconsMaterialDesignIcons.h>
#include <fonts/icons/mdi_font.hpp>

#include "../menu/menu.hpp"
#include "../features/features.hpp"

#include "../sdk/config/config.hpp"
#include "../sdk/hooks/hooks.hpp"

#include "../gamedata/gamedata.hpp"

void OpenSource2_StyleGUI();
void OpenSource2_PrepareFonts();

bool showAreYouSurePopup = false;

void OpenSource2_PrepareGUI() {
  ImPlot::SetImGuiContext(ImGui::GetCurrentContext());

  OpenSource2_PrepareFonts();
  OpenSource2_StyleGUI();
}

void OpenSource2_Render() {
  if (os2::hooks::g_isShuttingDown) return;

  if (ImGui::IsKeyPressed(ImGuiKey_End, false) && !showAreYouSurePopup) {
    os2::menu::ShowUnloadPopup(showAreYouSurePopup);
  }

  Game::Data::g_pGameData->SetDrawList(ImGui::GetBackgroundDrawList());

  os2::menu::Render();
  os2::menu::TopBar::Render();

  gameFeatures->OnRender();
}

void OpenSource2_PrepareFonts() {
  ImGuiIO& IO = ImGui::GetIO();

  ImFontConfig cfg{};
  cfg.MergeMode = true;
  cfg.FontBuilderFlags = 0;
  cfg.SizePixels = 26.f;

  ImFontConfig icons_config{};
  icons_config.MergeMode = true;
  icons_config.PixelSnapH = true;
  icons_config.GlyphMinAdvanceX = 26.f;

  static const ImWchar icons_ranges[] = {ICON_MIN_MDI, ICON_MAX_MDI, 0};

  IO.Fonts->Clear();
  os2::menu::fonts::libertad_mono =
      IO.Fonts->AddFontFromMemoryCompressedBase85TTF(
          LibertadMono_compressed_data_base85, 20.f);
  os2::menu::fonts::cartograph = IO.Fonts->AddFontFromMemoryCompressedBase85TTF(
      Cartograph_compressed_data_base85, 26.f, &cfg,
      IO.Fonts->GetGlyphRangesDefault());
  os2::menu::fonts::mdi = IO.Fonts->AddFontFromMemoryCompressedBase85TTF(
      mdi_compressed_data_base85, 26.f, &icons_config, icons_ranges);
  IO.Fonts->Build();
}

void OpenSource2_StyleGUI() {
  ImGuiStyle& style = ImGui::GetStyle();

  style.Alpha = 1.f;
  style.DisabledAlpha = 0.6f;
  style.WindowPadding = ImVec2(10.f, 10.f);
  style.WindowRounding = 10.f;
  style.WindowBorderSize = 1.f;
  style.WindowMinSize = ImVec2(50.f, 50.f);
  style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
  style.WindowMenuButtonPosition = ImGuiDir_None;
  style.ChildRounding = 0.f;
  style.ChildBorderSize = 1.f;
  style.PopupRounding = 11.f;
  style.PopupBorderSize = 1.f;
  style.FramePadding = ImVec2(5.f, 3.f);
  style.FrameRounding = 3.f;
  style.FrameBorderSize = 0.f;
  style.ItemSpacing = ImVec2(8.6f, 10.f);
  style.ItemInnerSpacing = ImVec2(3.f, 2.f);
  style.CellPadding = ImVec2(6.4f, 6.4f);
  style.IndentSpacing = 6.f;
  style.ColumnsMinSpacing = 6.f;
  style.ScrollbarSize = 10.8f;
  style.ScrollbarRounding = 16.f;
  style.GrabMinSize = 20.f;
  style.GrabRounding = 4.f;
  style.TabRounding = 4.f;
  style.TabBorderSize = 1.f;
  style.TabMinWidthForCloseButton = 0.f;
  style.ColorButtonPosition = ImGuiDir_Right;
  style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
  style.SelectableTextAlign = ImVec2(0.f, 0.f);

  style.AntiAliasedLinesUseTex = true;
  style.AntiAliasedLines = true;
  style.AntiAliasedFill = true;

  style.Colors[ImGuiCol_Text] = ImVec4(0.8588235f, 0.9294118f, 0.8862745f, 1.f);
  style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.5215687f, 0.5490196f, 0.5333334f, 1.f);
  style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09411765f, 0.09411765f, 0.1529412f, 1.f);
  style.Colors[ImGuiCol_ChildBg] = ImVec4(0.1490196f, 0.1568628f, 0.1882353f, 1.f);
  style.Colors[ImGuiCol_PopupBg] = ImVec4(0.2f, 0.2196078f, 0.2666667f, 1.f);
  style.Colors[ImGuiCol_Border] = ImVec4(0.1372549f, 0.1137255f, 0.1333333f, 1.f);
  style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.f, 0.f, 0.f, 1.f);
  style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1686275f, 0.1843137f, 0.2313726f, 1.f);
  style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.454902f, 0.1960784f, 0.2980392f, 1.f);
  style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.454902f, 0.1960784f, 0.2980392f, 1.f);
  style.Colors[ImGuiCol_TitleBg] = ImVec4(0.4206008f, 0.1317762f, 0.2536658f, 0.7639485f);
  style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.5019608f, 0.0745098f, 0.254902f, 1.f);
  style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.2f, 0.2196078f, 0.2666667f, 1.f);
  style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.13115f, 0.1430745f, 0.1716738f, 0.7725322f);
  style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.2392157f, 0.2392157f, 0.2196078f, 1.f);
  style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.3882353f, 0.3882353f, 0.372549f, 1.f);
  style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.6941177f, 0.6941177f, 0.6862745f, 1.f);
  style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.6941177f, 0.6941177f, 0.6862745f, 1.f);
  style.Colors[ImGuiCol_CheckMark] = ImVec4(0.6588235f, 0.1372549f, 0.1764706f, 1.f);
  style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.6509804f, 0.1490196f, 0.345098f, 1.f);
  style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.7098039f, 0.2196078f, 0.2666667f, 1.f);
  style.Colors[ImGuiCol_Button] = ImVec4(0.6509804f, 0.1490196f, 0.345098f, 1.f);
  style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.454902f, 0.1960784f, 0.2980392f, 1.f);
  style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.454902f, 0.1960784f, 0.2980392f, 1.f);
  style.Colors[ImGuiCol_Header] = ImVec4(0.7960784f, 0.6313726f, 0.2078431f, 0.85f);
  style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.7960784f, 0.6313726f, 0.2078431f, 0.7f);
  style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.7960784f, 0.6313726f, 0.2078431f, 0.5f);
  style.Colors[ImGuiCol_Separator] = ImVec4(0.427451f, 0.427451f, 0.4980392f, 1.f);
  style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.09803922f, 0.4f, 0.7490196f, 1.f);
  style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.09803922f, 0.4f, 0.7490196f, 1.f);
  style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.6509804f, 0.1490196f, 0.345098f, 1.f);
  style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.454902f, 0.1960784f, 0.2980392f, 1.f);
  style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.454902f, 0.1960784f, 0.2980392f, 1.f);
  style.Colors[ImGuiCol_Tab] = ImVec4(0.5536481f, 0.1259371f, 0.2774184f, 0.8841202f);
  style.Colors[ImGuiCol_TabHovered] = ImVec4(0.2588235f, 0.5882353f, 0.9764706f, 1.f);
  style.Colors[ImGuiCol_TabActive] = ImVec4(0.888412f, 0.09532315f, 0.3762094f, 0.7553648f);
  style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.06666667f, 0.1019608f, 0.145098f, 1.f);
  style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1333333f, 0.2588235f, 0.4235294f, 1.f);
  style.Colors[ImGuiCol_PlotLines] = ImVec4(0.8588235f, 0.9294118f, 0.8862745f, 1.f);
  style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.7596567f, 0.1271529f, 0.4312412f, 1.f);
  style.Colors[ImGuiCol_PlotHistogram] = ImVec4(1.f, 0.1843137f, 0.5764706f, 1.f);
  style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.7607843f, 0.1254902f, 0.4313726f, 1.f);
  style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882353f, 0.1882353f, 0.2f, 1.f);
  style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039f, 0.3098039f, 0.3490196f, 1.f);
  style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.227451f, 0.227451f, 0.2470588f, 1.f);
  style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.08218976f, 0.08621877f, 0.09871244f, 1.f);
  style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.12017167f, 0.12017046f, 0.12017046f, 1.f);
  style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.3843137f, 0.627451f, 0.9176471f, 1.f);
  style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.f, 1.f, 0.f, 1.f);
  style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2588235f, 0.5882353f, 0.9764706f, 1.f);
  style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.f, 1.f, 1.f, 1.f);
  style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 1.f);
  style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.3f);
}
