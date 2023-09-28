#pragma once

//class ShaderProgram {
// public:
//  ~ShaderProgram();
//
//  void use(float uniform, int location) const noexcept;
//  void create(const unsigned char fileBytes[]) noexcept;
//
// private:
//  bool bIsInitialised = false;
//};
//
//class BlurEffect {
// public:
//  static void draw(ImDrawList* drawList, float alpha) noexcept {
//    instance()._draw(drawList, alpha);
//  }
//
//  static void clear_textures() noexcept {
//    if (instance().blur_texture_1) {
//      instance().blur_texture_1->Release();
//      instance().blur_texture_1 = nullptr;
//    }
//    if (instance().blur_texture_2) {
//      instance().blur_texture_2->Release();
//      instance().blur_texture_2 = nullptr;
//    }
//  }
// private:
//  ID3D11Texture2D* blur_texture_1 = nullptr;
//  ID3D11Texture2D* blur_texture_2 = nullptr;
//
//  ID3D11RenderTargetView* m_offscreenTexture = nullptr;
//  ID3D11RenderTargetView* m_renderTargetView = nullptr;
//  ID3D11Texture2D* m_renderTarget1 = nullptr;
//  ID3D11Texture2D* m_renderTarget2 = nullptr;
//
//  int backbuffer_width;
//  int backbuffer_height;
//
//  static constexpr auto blur_downsample = 2;
//
//  BlurEffect() = default;
//  BlurEffect(const BlurEffect&) = delete;
//
//  ~BlurEffect() {
//    // if (rtBackup) rtBackup->Release();
//    if (blur_texture_1) blur_texture_1->Release();
//    if (blur_texture_2) blur_texture_2->Release();
//  }
//
//  static BlurEffect& instance() noexcept {
//    static BlurEffect blurEffect;
//    return blurEffect;
//  }
//
//  static void begin(const ImDrawList*, const ImDrawCmd*) noexcept {
//    instance()._begin();
//  }
//
//  static void first_pass(const ImDrawList*, const ImDrawCmd*) noexcept {
//    instance()._first_pass();
//  }
//
//  static void second_pass(const ImDrawList*, const ImDrawCmd*) noexcept {
//    instance()._second_pass();
//  }
//
//  static void end(const ImDrawList*, const ImDrawCmd*) noexcept {
//    instance()._end();
//  }
//
//  void create_textures() noexcept;
//  void create_shaders() noexcept;
//  void _begin(IDXGISwapChain*) noexcept;
//  void _first_pass() noexcept;
//  void _second_pass() noexcept;
//  void _end() noexcept;
//  void _draw(ImDrawList* draw_list, float) noexcept;
//};
//
//namespace os2 {
//  namespace gui {
//    ImFont* add_font_from_vfont(const std::string& path, float size,
//                            const ImWchar* glyphRanges, bool merge) noexcept;
//    ImWchar* get_font_glyph_ranges() noexcept;
//  };
//}; // namespace os2
