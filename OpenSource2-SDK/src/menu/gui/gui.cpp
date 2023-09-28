#include <fstream>
#include <vector>

#include "gui.hpp"

std::vector<char> load_binary_file(const std::string& path) noexcept {
  std::vector<char> result;
  std::ifstream in{path, std::ios::binary};

  if (!in) return result;

  in.seekg(0, std::ios_base::end);
  result.resize(static_cast<std::size_t>(in.tellg()));
  in.seekg(0, std::ios_base::beg);
  in.read(result.data(), result.size());

  return result;
}

bool decode_vfont(std::vector<char>& buffer) {
  constexpr std::string_view tag = "VFONT1";
  unsigned char magic = 0xA7;

  // LOG("tag size %i\n", buffer.size());

  if (buffer.size() <= tag.length()) return false;

  const auto tagIndex = buffer.size() - tag.length();
  if (std::memcmp(tag.data(), &buffer[tagIndex], tag.length())) return false;

  unsigned char saltBytes = buffer[tagIndex - 1];
  const auto saltIndex = tagIndex - saltBytes;
  --saltBytes;

  for (std::size_t i = 0; i < saltBytes; ++i)
    magic ^= (buffer[saltIndex + i] + 0xA7) % 0x100;

  for (std::size_t i = 0; i < saltIndex; ++i) {
    unsigned char xored = buffer[i] ^ magic;
    magic = (buffer[i] + 0xA7) % 0x100;
    buffer[i] = xored;
  }

  buffer.resize(saltIndex);
  return true;
}

//ImFont* os2::gui::add_font_from_vfont(const std::string& path, float size,
//                                   const ImWchar* glyphRanges, bool merge) noexcept {
//  std::vector<char> file = load_binary_file(path);
//
//  if (!decode_vfont(file)) return nullptr;
//
//  ImFontConfig cfg;
//  cfg.FontData = file.data();
//  cfg.FontDataSize = file.size();
//  cfg.FontDataOwnedByAtlas = false;
//
//  cfg.MergeMode = merge;
//  cfg.GlyphRanges = glyphRanges;
//  cfg.SizePixels = size;
//
//  return ImGui::GetIO().Fonts->AddFont(&cfg);
//}
//
//ImWchar* os2::gui::get_font_glyph_ranges() noexcept {
//  static ImVector<ImWchar> ranges;
//
//  if (ranges.empty()) {
//    ImFontGlyphRangesBuilder builder;
//    constexpr ImWchar baseRanges[]{
//      0x0100, 0x024F,  // Latin Extended-A + Latin Extended-B
//      0x0300, 0x03FF,  // Combining Diacritical Marks + Greek/Coptic
//      0x0600, 0x06FF,  // Arabic
//      0x0E00, 0x0E7F,  // Thai
//      0
//    };
//
//    builder.AddRanges(baseRanges);
//    builder.AddRanges(ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
//    builder.AddRanges(
//        ImGui::GetIO().Fonts->GetGlyphRangesChineseSimplifiedCommon());
//    // builder.AddText("\u9F8D\u738B\u2122");
//    builder.BuildRanges(&ranges);
//  }
//
//  return ranges.Data;
//}

//static ID3D11Device* pd3dDevice = DirectXHook::GetDevice();
//static ID3D11DeviceContext* pd3dDeviceContext = DirectXHook::GetDeviceContext();
//
//void ShaderProgram::create(const unsigned char fileBytes[]) noexcept {
//  if (bIsInitialised) return;
//
//  bIsInitialised = true;
//
//  pd3dDevice->CreatePixelShader(fileBytes, )
//}
//
//[[nodiscard]] static ID3D11Texture2D* create_texture(int width, int height) noexcept {
//  D3D11_TEXTURE2D_DESC desc;
//  desc.Width = width;
//  desc.Height = height;
//  desc.MipLevels = desc.ArraySize = 1;
//  desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//  desc.SampleDesc.Count = 1;
//  desc.Usage = D3D11_USAGE_DEFAULT;
//  desc.BindFlags = D3D11_BIND_RENDER_TARGET;
//  desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//  desc.MiscFlags = 0;
//
//  ID3D11Texture2D* pTexture = NULL;
//  pd3dDevice->CreateTexture2D(&desc, NULL, &pTexture);
//
//  return pTexture;
//}
//
//static void blur_pass(ID3D11ShaderResourceView* inputTexture,
//                      ID3D11RenderTargetView* outputRTV, int blurDirection) {
//  pd3dDeviceContext->PSSetShaderResources(0, 1, &inputTexture);
//
//  pd3dDeviceContext->OMSetRenderTargets(1, &outputRTV, NULL);
//
//  pd3dDeviceContext->Draw(2.0f / (backbuffer_width / blurDownsample));
//
//  ID3D11ShaderResourceView* nullSRV = nullptr;
//  pd3dDeviceContext->PSSetShaderResources(0, 1, &nullSRV);
//}
//
//void BlurEffect::create_textures() noexcept {
//  if (const auto [width, height] = ImGui::GetIO().DisplaySize;
//      backbuffer_width != static_cast<int>(width) ||
//      backbuffer_height != static_cast<int>(height)) {
//    clear_textures();
//    backbuffer_width = static_cast<int>(width);
//    backbuffer_height = static_cast<int>(height);
//  }
//
//  if (!blur_texture_1)
//    blur_texture_1 = create_texture(backbuffer_width / blur_downsample,
//                                    backbuffer_height / blur_downsample);
//  if (!blur_texture_2)
//    blur_texture_2 = create_texture(backbuffer_width / blur_downsample,
//                                    backbuffer_height / blur_downsample);
//}
//
//void BlurEffect::_begin(IDXGISwapChain* pSwapChain) noexcept {
//  ID3D11Texture2D* pBackBuffer = NULL;
//
//  pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
//
//  if (pBackBuffer) {
//    pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_offscreenTexture);
//
//    DXGI_SWAP_CHAIN_DESC sd;
//    pSwapChain->GetDesc(&sd);
//
//    D3D11_RENDER_TARGET_VIEW_DESC desc = {};
//    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//    desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
//    desc.Texture2D.MipSlice = 0;
//
//    pd3dDevice->CreateRenderTargetView(pBackBuffer, &desc, &m_renderTargetView);
//  }
//
//  pBackBuffer->Release();
//}
//
//void BlurEffect::_first_pass() noexcept {
//  pd3dDeviceContext->OMSetRenderTargets(1, &m_renderTargetView, NULL);
//
//  pd3dDeviceContext->PSSetShaderResources(0, 1, &inputTexture);
//}
