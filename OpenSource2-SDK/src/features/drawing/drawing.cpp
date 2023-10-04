#include <imgui/imgui.h>

#include "drawing.hpp"

#include "../../game/state.hpp"
#include "../../gamedata/gamedata.hpp"
#include "../../menu/menu.hpp"

using namespace Game::Data;

// Extract RGBA components from a packed color
#define IM_COL32_EXTRACT_RED(col) ((col >> 0) & 0xFF)
#define IM_COL32_EXTRACT_GREEN(col) ((col >> 8) & 0xFF)
#define IM_COL32_EXTRACT_BLUE(col) ((col >> 16) & 0xFF)
#define IM_COL32_EXTRACT_ALPHA(col) ((col >> 24) & 0xFF)

void Drawing::OnRender() noexcept {
  const std::lock_guard<std::mutex> guard{this->displayed_info_mutex};

  for (auto it = this->displayed_info.begin(); it != this->displayed_info.end();
       it++) {
    __int64 index = std::distance(this->displayed_info.begin(), it);
    DrawInfo((*it).first, index, (*it).second);
  }
}

void Drawing::ClearScreenText() noexcept {
  const std::lock_guard<std::mutex> guard{this->displayed_info_mutex};

  this->displayed_info.clear();
}

void Drawing::RenderOnScreen(const std::string& input,
                             unsigned int text_color) noexcept {
  const std::lock_guard<std::mutex> guard{this->displayed_info_mutex};

  this->displayed_info.emplace_back(std::make_pair(input, text_color));
}

void Drawing::RenderOnScreen(const std::string& input,
                             ImVec4 text_color) noexcept {
  return RenderOnScreen(input, ImGui::ColorConvertFloat4ToU32(text_color));
}

void Drawing::RenderOnScreen(const char* input,
                             unsigned int text_color) noexcept {
  return RenderOnScreen(std::string{input}, text_color);
}

void Drawing::RenderOnScreen(const char* input, ImVec4 text_color) noexcept {
  return RenderOnScreen(std::string{input},
                        ImGui::ColorConvertFloat4ToU32(text_color));
}

void Drawing::DrawInfo(const std::string& input, __int64 index,
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

  unsigned int border_color =
      IM_COL32(255 - IM_COL32_EXTRACT_RED(text_color),
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

void Drawing::RenderShadow(ImVec2 position, float width, float height,
                       float rounding, int shadowOffset, int shadowLayers,
                       unsigned int shadowColor) noexcept {
  for (int i = 1; i <= shadowLayers; i++) {
    // Compute new color by reducing the alpha component for each layer
    unsigned int currentShadowColor =
        (shadowColor & 0xFFFFFF00) |
        (unsigned int)(((float)(shadowColor & 0xFF) *
                        (1.0f - (i / (float)shadowLayers))));

    // Expand the shadow size for each layer
    float offsetX = shadowOffset * (i / (float)shadowLayers);
    float offsetY = shadowOffset * (i / (float)shadowLayers);

    g_pGameData->DrawList()->AddRectFilled(
        ImVec2(position.x - offsetX, position.y + offsetY),
        ImVec2(position.x + width + offsetX, position.y + height + offsetY),
        currentShadowColor, rounding);
  }
}

void Drawing::RenderCenteredTextWithOutline(ImVec2 position, float width,
                                        float height, std::string text,
                                        unsigned int textColor,
                                        unsigned int textBorderColor,
                                        float offset) noexcept {
  ImDrawList* drawList = g_pGameData->DrawList();

  const char* szText = text.c_str();

  ImGui::PushFont(os2::menu::fonts::cartograph);

  // Calculate text size
  ImVec2 textSizeVec = ImGui::CalcTextSize(szText);

  // Compute position for centered text
  ImVec2 textPos = ImVec2(position.x + (width - textSizeVec.x) * 0.5f,
                          position.y + (height - textSizeVec.y) * 0.5f);

  // Draw text outline/border
  for (float x = -offset; x <= offset; x += offset) {
    for (float y = -offset; y <= offset; y += offset) {
      // Skip rendering at center since that's where the main text will be
      if (x != 0.0f || y != 0.0f)
        drawList->AddText(ImVec2(textPos.x + x, textPos.y + y), textBorderColor,
                          szText);
    }
  }

  // Draw the centered text
  drawList->AddText(textPos, textColor, szText);

  ImGui::PopFont();
}

void Drawing::RenderArrowToAngle(const ImVec2& center, float angleRadians,
                                 float offset) noexcept {
  ImDrawList* drawList = g_pGameData->DrawList();

  // Define arrow properties
  float arrowSize = 28.0f;
  float distanceFromCenter = offset + 16.0f;

  ImVec2 arrowStart(center.x + distanceFromCenter * std::cos(angleRadians),
                    center.y + distanceFromCenter * std::sin(angleRadians));
  ImVec2 arrowEnd(arrowStart.x + arrowSize * std::cos(angleRadians),
                  arrowStart.y + arrowSize * std::sin(angleRadians));

  // Draw line from arrow start to arrow end
  drawList->AddLine(arrowStart, arrowEnd, IM_COL32(255, 255, 255, 255), 2.0f);

  // Draw arrowhead
  ImVec2 arrowHead1(
      arrowEnd.x + arrowSize * 0.4f * std::cos(angleRadians - 3.1416f * 0.75f),
      arrowEnd.y + arrowSize * 0.4f * std::sin(angleRadians - 3.1416f * 0.75f));
  ImVec2 arrowHead2(
      arrowEnd.x + arrowSize * 0.4f * std::cos(angleRadians + 3.1416f * 0.75f),
      arrowEnd.y + arrowSize * 0.4f * std::sin(angleRadians + 3.1416f * 0.75f));

  drawList->AddTriangleFilled(arrowEnd, arrowHead1, arrowHead2,
                               IM_COL32(255, 255, 255, 255));
}

void Drawing::RenderProgressBar(ImVec2 position, float width, unsigned int color,
                            float progress, bool shadow) noexcept {
  // Ensure progress is between 0 and 1
  progress = (progress < 0.0f) ? 0.0f : (progress > 1.0f) ? 1.0f : progress;

  ImDrawList* drawList = g_pGameData->DrawList();

  const int height = 20;        // This can be adjusted based on your preference
  const float rounding = 4.0f;  // Corner rounding
  const int borderWidth = 1;    // This can be adjusted
  const auto borderColor =
      IM_COL32(220, 220, 220, 255);  // Light gray color for border
  const auto bgColor = IM_COL32(50, 50, 50, 255);  // Dark gray for background

  if (shadow) RenderShadow(position, width, height, rounding, 4, 6);

  // Background
  drawList->AddRectFilled(position,
                          ImVec2(position.x + width, position.y + height),
                          bgColor, rounding);

  // Progress
  int progressWidth = static_cast<int>(progress * width);
  drawList->AddRectFilled(
      position, ImVec2(position.x + progressWidth, position.y + height), color,
      rounding, ImDrawFlags_RoundCornersAll);

  // Border
  drawList->AddRect(position, ImVec2(position.x + width, position.y + height),
                    borderColor, rounding, ImDrawFlags_RoundCornersAll,
                    borderWidth);
}

unsigned int Drawing::BombColorFade(float x, float y) noexcept {
  if (y <= 0)
    return IM_COL32(255, 0, 0, 255);  // If max is 0 or less, return full red.

  double z = static_cast<double>(x) / y;

  int r, g;

  if (z <= 0.05) {
    // Very dark red
    r = 255;
    g = 0;
  } else if (z <= 0.10) {
    // Transition between very dark red and very intense red
    double factor = (z - 0.05) * 20;
    r = 255;
    g = static_cast<int>(20 * (1 - factor));
  } else if (z <= 0.5) {
    // Transition between very intense red and regular green-to-red fade
    double factor = (z - 0.10) / 0.40;
    r = static_cast<int>(220 + (255 - 220) * factor);
    g = static_cast<int>(20 * (1 - factor));
  } else {
    // Regular green-to-red fade
    r = static_cast<int>(255.0 * (1 - z));
    g = static_cast<int>(255.0 * z);
  }

  return IM_COL32(r, g, 0, 255);
}
