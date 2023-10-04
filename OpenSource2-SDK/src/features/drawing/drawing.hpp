#pragma once

#include "../feature.hpp"

class Drawing : public Feature {
 public:
  CONSTRUCT_FEATURE(Drawing) {}

  void OnRender() noexcept override;

  void RenderOnScreen(const std::string& input, unsigned int text_color = 0xFFFFFFFF) noexcept;
  void RenderOnScreen(const std::string& input, ImVec4 text_color) noexcept;
  void RenderOnScreen(const char* input, unsigned int text_color = 0xFFFFFFFF) noexcept;
  void RenderOnScreen(const char* input, ImVec4 text_color) noexcept;

  void ClearScreenText() noexcept;
  
  void RenderShadow(ImVec2 position, float width, float height,
                    float rounding = 0.0f, int shadowOffset = 2,
                    int shadowLayers = 3,
                    unsigned int color = 0x20000000) noexcept;
  void RenderProgressBar(ImVec2 position, float width, unsigned int color,
                         float progress, bool shadow = true) noexcept;
  void RenderCenteredTextWithOutline(ImVec2 position, float width, float height,
                                     std::string text,
                                     unsigned int textColor = 0xFF000000,
                                     unsigned int textBorderColor = 0xFFFFFFFF,
                                     float offset = 1.0f) noexcept;

  void RenderArrowToAngle(const ImVec2& center, float angleRadians,
                          float offset = 0.f) noexcept;

  unsigned int BombColorFade(float x, float y) noexcept;

  void OnCreateMove(os2::sdk::CCSGOInput* pCsgoInput,
                    os2::sdk::CUserCmd* pUserCmd,
                    glm::vec3& view_angles) noexcept override {}

  nlohmann::json ToJson() const override { return {}; }

  void FromJson(const nlohmann::json& j) override {}

 private:
  std::vector<std::pair<std::string, unsigned int>> displayed_info;
  std::mutex displayed_info_mutex;

  void DrawInfo(const std::string& input, __int64 index,
                unsigned int text_color) noexcept;
};
