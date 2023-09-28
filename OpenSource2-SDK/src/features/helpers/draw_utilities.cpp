#include "draw_utilities.hpp"

void DrawUtilities::Destroy() {
  // Reset the readiness flag
  bIsReady = false;

  // Cleanup DirectX resources
  ReleaseResource(pOffscreenRenderTargetView);
  ReleaseResource(pOffscreenShaderResourceView);
  ReleaseResource(pGaussianBlurX);
  ReleaseResource(pGaussianBlurY);
  ReleaseResource(pCopyPSShader);
  ReleaseResource(pSampler);
  ReleaseResource(pVertexBuffer);
  ReleaseResource(pBackBufferTexture);
  ReleaseResource(pBackBufferSRV);
  ReleaseResource(pRasterState);
  ReleaseResource(pVSConstantBuffer);

  ClearTextures();
}

template <typename T>
void DrawUtilities::ReleaseResource(T*& resource) {
  if (resource) {
    resource->Release();
    resource = nullptr;
  }
}

void DrawUtilities::Initialise(ID3D11Device* pD3D11Device,
                               ID3D11DeviceContext* pD3D11DeviceContext) {
  if (IsReady()) return;

  this->SetDevice(pD3D11Device);
  this->SetContext(pD3D11DeviceContext);

  if (!CreateShaders()) {
    throw std::exception("Could not initialise shaders.");
  }

  if (!CreateTextures()) {
    throw std::exception("Could not initialise textures.");
  }

  if (!CreateOffscreenResources()) {
    throw std::exception("Could not initialise offscreen resources.");
  }

  if (!CreateSampler()) {
    throw std::exception("Could not initialise the sampler.");
  }

  if (!InitialiseVSConstantBuffer()) {
    throw std::exception("Could not initialise the constant buffer.");
  }

  bIsReady = true;
}

void DrawUtilities::SetContext(ID3D11DeviceContext* pD3D11DeviceContext) {
  this->pContext = pD3D11DeviceContext;
}

void DrawUtilities::ClearTextures() {
  if (pGaussianBlurTextureX) pGaussianBlurTextureX->Release();
  if (pGaussianBlurTextureY) pGaussianBlurTextureY->Release();

  pGaussianBlurTextureX = nullptr;
  pGaussianBlurTextureY = nullptr;
}

bool DrawUtilities::CreateShaders() {
  // Initialize shaders from byte arrays
  HRESULT hr = pDevice->CreatePixelShader(
      gaussian_blur_x, sizeof(gaussian_blur_x), nullptr, &pGaussianBlurX);

  if (FAILED(hr)) {
    return false;
  }

  hr = pDevice->CreatePixelShader(gaussian_blur_y, sizeof(gaussian_blur_y),
                                  nullptr, &pGaussianBlurY);
  if (FAILED(hr)) {
    return false;
  }

  hr = pDevice->CreatePixelShader(CopyPS, sizeof(CopyPS), nullptr,
                                  &pCopyPSShader);

  if (FAILED(hr)) {
    return false;
  }

  return true;
}

ID3D11Texture2D* DrawUtilities::CreateTexture(int width, int height) {
  ID3D11Texture2D* pTexture = nullptr;

  // Step 1: Create the render texture.
  D3D11_TEXTURE2D_DESC textureDesc = {};
  textureDesc.Width = width;
  textureDesc.Height = height;
  textureDesc.MipLevels = 1;
  textureDesc.ArraySize = 1;
  textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  textureDesc.SampleDesc.Count = 1;
  textureDesc.Usage = D3D11_USAGE_DEFAULT;
  textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

  HRESULT hr = pDevice->CreateTexture2D(&textureDesc, nullptr, &pTexture);

  if (FAILED(hr)) {
    return nullptr;
  }

  return pTexture;
}

bool DrawUtilities::CreateTextures() {
  if (const auto [width, height] = ImGui::GetIO().DisplaySize;
      backbuffer_width != static_cast<int>(width) ||
      backbuffer_height != static_cast<int>(height)) {
    ClearTextures();
    backbuffer_width = static_cast<int>(width);
    backbuffer_height = static_cast<int>(height);
  }

  if (!pGaussianBlurTextureX)
    pGaussianBlurTextureX = CreateTexture(backbuffer_width / BlurDownsample,
                                          backbuffer_height / BlurDownsample);
  if (!pGaussianBlurTextureY)
    pGaussianBlurTextureY = CreateTexture(backbuffer_width / BlurDownsample,
                                          backbuffer_height / BlurDownsample);

  return pGaussianBlurTextureX != nullptr && pGaussianBlurTextureY != nullptr;
}

bool DrawUtilities::CreateOffscreenResources() {
  // Assuming CreateTexture() has been defined earlier in the class.
  ID3D11Texture2D* pOffscreenTexture =
      CreateTexture(backbuffer_width, backbuffer_height);
  if (!pOffscreenTexture) return false;

  // Create the render target view for the offscreen texture.
  HRESULT hr = pDevice->CreateRenderTargetView(pOffscreenTexture, NULL,
                                               &pOffscreenRenderTargetView);
  if (FAILED(hr)) {
    pOffscreenTexture->Release();
    return false;
  }

  // Create the shader resource view for the offscreen texture.
  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
  srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  srvDesc.Texture2D.MipLevels = 1;

  hr = pDevice->CreateShaderResourceView(pOffscreenTexture, &srvDesc,
                                         &pOffscreenShaderResourceView);
  pOffscreenTexture->Release();

  if (FAILED(hr)) {
    return false;
  }

  return true;
}

bool DrawUtilities::CreateSampler() {
  D3D11_SAMPLER_DESC samplerDesc = {};
  samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
  samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
  samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
  samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
  samplerDesc.MinLOD = 0;
  samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

  return !FAILED(pDevice->CreateSamplerState(&samplerDesc, &pSampler));
}

bool DrawUtilities::CreateBackbufferSRV() {
  if (pBackBufferSRV) return true;

  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
  srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  srvDesc.Texture2D.MipLevels = 1;

  HRESULT hr = pDevice->CreateShaderResourceView(pBackBufferTexture, &srvDesc,
                                                 &pBackBufferSRV);

  return !FAILED(hr);
}

bool DrawUtilities::InitialiseVSConstantBuffer() {
  D3D11_BUFFER_DESC bufferDesc = {};

  bufferDesc.Usage =
      D3D11_USAGE_DEFAULT;  // We'll be updating this buffer quite frequently
  bufferDesc.ByteWidth = sizeof(VSConstants);  // Size of the struct
  bufferDesc.BindFlags =
      D3D11_BIND_CONSTANT_BUFFER;  // Use as a constant buffer

  HRESULT hr = pDevice->CreateBuffer(&bufferDesc, nullptr, &pVSConstantBuffer);

  if (FAILED(hr)) {
    return false;  // Failed to create the buffer
  }

  return true;  // Successfully created the buffer
}

void DrawUtilities::SetOffscreenRenderTarget() noexcept {
  // 
}

ID3D11ShaderResourceView*
DrawUtilities::GetOffscreenShaderResourceView() noexcept {
  return pOffscreenShaderResourceView;
}

void DrawUtilities::DefineFullscrenQuad() noexcept {
  if (pVertexBuffer) return;

  Vertex vertices[] = {
      {-1.0f, 1.0f, 0.0f, 0.0f, 0.0f},
      {1.0f, 1.0f, 0.0f, 1.0f, 0.0f},
      {-1.0f, -1.0f, 0.0f, 0.0f, 1.0f},
      {1.0f, -1.0f, 0.0f, 1.0f, 1.0f},
  };

  // Create vertex buffer
  D3D11_BUFFER_DESC bufferDesc = {};
  bufferDesc.Usage = D3D11_USAGE_DEFAULT;
  bufferDesc.ByteWidth = sizeof(vertices);
  bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

  D3D11_SUBRESOURCE_DATA initData = {};
  initData.pSysMem = vertices;

  pDevice->CreateBuffer(&bufferDesc, &initData, &pVertexBuffer);
}

void DrawUtilities::RenderFullscrenQuad() noexcept {
  pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
  UINT stride = sizeof(Vertex), offset = 0;
  pContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
  pContext->PSSetShader(pCopyPSShader, nullptr, 0);
  pContext->PSSetSamplers(0, 1, &pSampler);
  // pContext->PSSetShaderResources(0, 1, &pBackBufferSRV);
  // pContext->Draw(4, 0);
}

void DrawUtilities::CopyBackbufferToTexture(ID3D11Texture2D* pDstTexture,
                                            D3D11_FILTER filter) noexcept {
  /*ID3D11Resource* pBackBufferResource;
  pOffscreenRenderTargetView->GetResource(&pBackBufferResource);
  pContext->CopyResource(pDstTexture, pBackBufferResource);
  pBackBufferResource->Release();*/

  ID3D11RenderTargetView* rtv = nullptr;
  pContext->OMGetRenderTargets(1, &rtv, nullptr);

  ID3D11Resource* srcResource = nullptr;
  rtv->GetResource(&srcResource);

  pContext->CopyResource(pDstTexture, srcResource);

  srcResource->Release();
  rtv->Release();
}

void DrawUtilities::SetVertexShaderConstants(const DirectX::XMMATRIX& matrix) {
  VSConstants constants;
  constants.projection = matrix;
  pContext->UpdateSubresource(pVSConstantBuffer, 0, NULL, &constants, 0, 0);
  pContext->VSSetConstantBuffers(0, 1, &pVSConstantBuffer);
}

void DrawUtilities::BeginBlur() noexcept {
  printf("BeginBlur Pre\n");
  // Backup the current render target
  pContext->OMGetRenderTargets(1, &pOffscreenRenderTargetView, nullptr);

  // Copy the backbuffer to our first blur texture with some filtering.
  CopyBackbufferToTexture(pBackBufferTexture, D3D11_FILTER_MIN_MAG_MIP_LINEAR);

  // Set sampler state to use clamping.
  pContext->PSSetSamplers(
      0, 1, &pSampler);  // Assuming clampSampler is previously defined

  // Disable scissor test (assuming you have a similar need)
  D3D11_RASTERIZER_DESC desc;
  ZeroMemory(&desc, sizeof(desc));
  desc.FillMode = D3D11_FILL_SOLID;
  desc.CullMode = D3D11_CULL_NONE;
  desc.ScissorEnable = false;
  pDevice->CreateRasterizerState(&desc, &pRasterState);
  pContext->RSSetState(pRasterState);

  // Set the projection matrix similar to DX9
  // Assuming you have a method to set vertex shader constants.
  DirectX::XMMATRIX projection = DirectX::XMMatrixSet(
      1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
      -1.0f / (backbuffer_width / BlurDownsample),
      1.0f / (backbuffer_height / BlurDownsample), 0.0f, 1.0f);
  SetVertexShaderConstants(projection);

  printf("BeginBlur Post\n");
}

void DrawUtilities::PerformHorizontalBlur() noexcept {
  printf("PerformHorizontalBlur Pre\n");

  // Set the Gaussian blur texture X as a render target
  SetOffscreenRenderTarget();

  // Bind the offscreen texture as a shader resource
  pContext->PSSetShaderResources(0, 1, &pOffscreenShaderResourceView);

  // Use horizontal blur shader
  pContext->PSSetShader(pGaussianBlurX, nullptr, 0);

  // Draw the full screen quad
  // RenderFullscrenQuad();

  printf("PerformHorizontalBlur Post\n");
}

void DrawUtilities::PerformVerticalBlur() noexcept {
  printf("PerformVerticalBlur Pre\n");

  // Switch to another texture for vertical blur so we're not reading/writing
  // the same texture. [NOTE] This implies that SetOffscreenRenderTarget()
  // uses `pGaussianBlurTextureY` now.

  SetOffscreenRenderTarget();

  // Bind the horizontal blurred texture (Gaussian blur X texture) as a shader
  // resource.
  ID3D11ShaderResourceView* tempSRV = nullptr;
  HRESULT hr =
      pDevice->CreateShaderResourceView(pGaussianBlurTextureX, NULL, &tempSRV);
  if (SUCCEEDED(hr) && tempSRV) {
    pContext->PSSetShaderResources(0, 1, &tempSRV);
    tempSRV->Release();
  }

  // Use vertical blur shader
  pContext->PSSetShader(pGaussianBlurY, nullptr, 0);

  // Draw the full screen quad
  RenderFullscrenQuad();

  printf("PerformVerticalBlur Post\n");
}

void DrawUtilities::EndBlur() noexcept {
  printf("EndBlur Pre\n");
  // Restore the original render target.
  pContext->OMSetRenderTargets(1, &pOffscreenRenderTargetView, nullptr);
  pOffscreenRenderTargetView->Release();

  // Reset shaders and restore scissor state if required.
  pContext->PSSetShader(nullptr, nullptr, 0);

  D3D11_RASTERIZER_DESC desc;
  ZeroMemory(&desc, sizeof(desc));
  desc.FillMode = D3D11_FILL_SOLID;
  desc.CullMode = D3D11_CULL_NONE;
  desc.ScissorEnable = true;
  pDevice->CreateRasterizerState(&desc, &pRasterState);
  pContext->RSSetState(pRasterState);
  printf("PerformVerticalBlur Post\n");
}

void DrawUtilities::DrawBlur(ImDrawList* drawList, const ImVec2& p_min, const ImVec2& p_max,
              float alpha) noexcept {
  if (!IsReady()) return;

  drawList->AddCallback(&begin_blur, nullptr);

  for (int i = 0; i < 8; ++i) {
    drawList->AddCallback(&perform_horizontal_blur, nullptr);
    drawList->AddImage(reinterpret_cast<ImTextureID>(pGaussianBlurTextureX),
                       p_min, p_max, {1.0f, 1.0f});
    drawList->AddCallback(&perform_vertical_blur, nullptr);
    drawList->AddImage(reinterpret_cast<ImTextureID>(pGaussianBlurTextureY),
                       p_min, p_max, {1.0f, 1.0f});
  }

  drawList->AddCallback(&end_blur, nullptr);
  drawList->AddCallback(ImDrawCallback_ResetRenderState, nullptr);

  drawList->AddImage(
      reinterpret_cast<ImTextureID>(pGaussianBlurTextureX), p_min, p_max,
      ImVec2((float)backbuffer_width * 1, (float)backbuffer_height * 1),
      ImVec2((float)backbuffer_width * 1, (float)backbuffer_height * 1),
      IM_COL32(255, 255, 255, 255 * alpha));
}
