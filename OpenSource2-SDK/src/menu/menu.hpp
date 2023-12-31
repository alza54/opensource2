#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>

#include "state.hpp"

class ImStyleCache {
 public:
  ImStyleCache(ImGuiStyle& style) {
    this->FramePadding = style.FramePadding;
    this->ItemSpacing = style.ItemSpacing;
  }

  void restore(ImGuiStyle& style) {
    style.FramePadding = this->FramePadding;
    style.ItemSpacing = this->ItemSpacing;
  }

 private:
  ImVec2 FramePadding;
  ImVec2 ItemSpacing;
};

namespace os2::menu {
  inline namespace fonts {
    inline ImFont* red_hat_display = nullptr;
    inline ImFont* red_hat_display_large = nullptr;
    inline ImFont* libertad_mono = nullptr;
    inline ImFont* cartograph = nullptr;
    inline ImFont* mdi = nullptr;
  }  // namespace fonts

  bool ShowUnloadPopup();
  void Render();

  void Toggle(bool bState);
  bool IsOpen();

  namespace TopBar {
    void MenuItemEx(const char* name, const char icon[], bool& stateVariable,
                    const char* shortcut = "", bool enabled = true) noexcept;

    void Render() noexcept;
  };  // namespace TopBar
};  // namespace os2::menu
