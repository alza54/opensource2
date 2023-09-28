#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>

#include <d3d11.h>
#include <dxgi1_2.h>
#include <exception>
#include <memory>
#include <algorithm>
#include <DirectXMath.h>

#include "../../sdk/shaders/build/gaussian_blur.hpp"

class DrawUtilities {
 public:
  static DrawUtilities& instance() noexcept {
    static DrawUtilities drawUtilities;
    return drawUtilities;
  }

  static void initialise(ID3D11Device* pD3D11Device,
    ID3D11DeviceContext* pD3D11DeviceContext) {
    instance().Initialise(pD3D11Device, pD3D11DeviceContext);
  }

  static void destroy() {
    instance().Destroy();
  }

  static void draw(ImDrawList* drawList, const ImVec2& p_min,
    const ImVec2& p_max, float alpha) noexcept {
    instance().draw(drawList, p_min, p_max, alpha);
  }

  static void begin_blur(const ImDrawList*, const ImDrawCmd*) noexcept {
    instance().BeginBlur();
  }

  static void perform_horizontal_blur(const ImDrawList*, const ImDrawCmd*) noexcept {
    instance().PerformHorizontalBlur();
  }

  static void perform_vertical_blur(const ImDrawList*, const ImDrawCmd*) noexcept {
    instance().PerformVerticalBlur();
  }

  static void end_blur(const ImDrawList*, const ImDrawCmd*) noexcept {
    instance().EndBlur();
  }

  void Initialise(ID3D11Device* pD3D11Device,
                  ID3D11DeviceContext* pD3D11DeviceContext);

  void SetDevice(ID3D11Device* pD3D11Device) { this->pDevice = pD3D11Device; }

  void SetContext(ID3D11DeviceContext* pD3D11DeviceContext);

  void ClearTextures();

  bool CreateShaders();

  ID3D11Texture2D* CreateTexture(int width, int height);

  bool CreateTextures();

  bool CreateOffscreenResources();

  bool CreateSampler();

  bool CreateBackbufferSRV();

  bool InitialiseVSConstantBuffer();

  void SetOffscreenRenderTarget() noexcept;

  ID3D11ShaderResourceView* GetOffscreenShaderResourceView() noexcept;

  void DefineFullscrenQuad() noexcept;

  void RenderFullscrenQuad() noexcept;

  void CopyBackbufferToTexture(ID3D11Texture2D* pDstTexture,
                               D3D11_FILTER filter) noexcept;

  void SetVertexShaderConstants(const DirectX::XMMATRIX& matrix);

  void BeginBlur() noexcept;

  void PerformHorizontalBlur() noexcept;

  void PerformVerticalBlur() noexcept;

  void EndBlur() noexcept;

  void DrawBlur(ImDrawList* drawList, const ImVec2& p_min, const ImVec2& p_max,
                float alpha) noexcept;

  bool IsReady() { return bIsReady; }

  void Destroy();

 private:
  DrawUtilities() = default;
  DrawUtilities(const DrawUtilities&) = delete;

  static constexpr inline int BlurDownsample = 2;

  struct Vertex {
    float x, y, z;
    float u, v;
  };

  struct VSConstants {
    DirectX::XMMATRIX projection;
  };

  ~DrawUtilities() { Destroy(); }

  int backbuffer_width = 0;
  int backbuffer_height = 0;

  bool bIsReady = false;

  template <typename T>
  void ReleaseResource(T*& resource);

  // DirectX
  ID3D11Device* pDevice = nullptr;
  ID3D11DeviceContext* pContext = nullptr;

  // Pixel shaders
  ID3D11PixelShader* pGaussianBlurX = nullptr;
  ID3D11PixelShader* pGaussianBlurY = nullptr;
  ID3D11PixelShader* pCopyPSShader = nullptr;

  // Gaussian blur textures
  ID3D11Texture2D* pGaussianBlurTextureX = nullptr;
  ID3D11Texture2D* pGaussianBlurTextureY = nullptr;

  // Backbuffer holding
  ID3D11RenderTargetView* pOffscreenRenderTargetView = nullptr;
  ID3D11ShaderResourceView* pOffscreenShaderResourceView = nullptr;

  // Fullscreen quad
  ID3D11Buffer* pVertexBuffer = nullptr;
  ID3D11SamplerState* pSampler = nullptr;

  // Backbuffer
  ID3D11Texture2D* pBackBufferTexture = nullptr;
  ID3D11ShaderResourceView* pBackBufferSRV = nullptr;

  // Raster
  ID3D11RasterizerState* pRasterState = nullptr;

  // Constant buffer
  ID3D11Buffer* pVSConstantBuffer = nullptr;
};

inline DrawUtilities* g_pDrawUtilities;
