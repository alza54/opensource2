#include "directx11_hook.hpp"
#include "../wndproc/wndproc_hook.hpp"

#include "../hooks.hpp"

#include "../../../features/helpers/post_processing.hpp"

void OpenSource2_PrepareGUI();

bool DirectXHook::CreateDeviceD3D11(HWND hWnd) {
  // Create the D3DDevice
  DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
  swapChainDesc.Windowed = TRUE;
  swapChainDesc.BufferCount = 2;
  swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapChainDesc.OutputWindow = hWnd;
  swapChainDesc.SampleDesc.Count = 1;

  const D3D_FEATURE_LEVEL featureLevels[] = {
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_0,
  };

  HRESULT hr = D3D11CreateDeviceAndSwapChain(
      NULL, D3D_DRIVER_TYPE_NULL, NULL, 0, featureLevels, 2, D3D11_SDK_VERSION,
      &swapChainDesc, &g_pSwapChain, &g_pd3dDevice, nullptr, nullptr);

  if (hr != S_OK) {
    LOG(log_d3d11_create_device_and_swap_chain_failed.c_str(), hr);
    return false;
  }

  return true;
}

int DirectXHook::GetCorrectDXGIFormat(int eCurrentFormat) {
  switch (eCurrentFormat) {
    case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
      return DXGI_FORMAT_R8G8B8A8_UNORM;
  }

  return eCurrentFormat;
}

void DirectXHook::CreateRenderTarget(IDXGISwapChain* pSwapChain) {
  ID3D11Texture2D* pBackBuffer = NULL;

  pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

  if (pBackBuffer) {
    DXGI_SWAP_CHAIN_DESC sd;
    pSwapChain->GetDesc(&sd);

    D3D11_RENDER_TARGET_VIEW_DESC desc = {};
    desc.Format =
        static_cast<DXGI_FORMAT>(GetCorrectDXGIFormat(sd.BufferDesc.Format));

    desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;

    if (FAILED(g_pd3dDevice->CreateRenderTargetView(pBackBuffer, &desc,
                                                    &g_pd3dRenderTarget))) {
      LOG(log_create_render_target_failed.c_str());

      desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

      if (FAILED(g_pd3dDevice->CreateRenderTargetView(pBackBuffer, &desc,
                                                      &g_pd3dRenderTarget))) {
        if (FAILED(g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL,
                                                        &g_pd3dRenderTarget))) {
          LOG(log_create_render_target_severe_error.c_str());
        }
      }
    }

    pBackBuffer->Release();
  }
}

// static Present_t CHooks::Present;
static HRESULT WINAPI Hooks::Present(IDXGISwapChain* pSwapChain, UINT SyncInterval,
                                UINT Flags) {
  DirectXHook::RenderImGui_DX11(pSwapChain);

  return CHooks::Present.m_pOriginalFn(pSwapChain, SyncInterval, Flags);
}

// static Present1_t CHooks::Present1;
static HRESULT WINAPI Hooks::Present1(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT PresentFlags,
           const DXGI_PRESENT_PARAMETERS* pPresentParameters) {
  DirectXHook::RenderImGui_DX11(pSwapChain);

  return CHooks::Present1.m_pOriginalFn(pSwapChain, SyncInterval, PresentFlags,
                                  pPresentParameters);
}

// static ResizeBuffers_t CHooks::ResizeBuffers;
static HRESULT WINAPI Hooks::ResizeBuffers(IDXGISwapChain* pSwapChain,
                                      UINT BufferCount, UINT Width, UINT Height,
                                      DXGI_FORMAT NewFormat,
                                      UINT SwapChainFlags) {
  DirectXHook::CleanupRenderTarget();

  return CHooks::ResizeBuffers.m_pOriginalFn(pSwapChain, BufferCount, Width, Height,
                                       NewFormat, SwapChainFlags);
}

// static ResizeBuffers1_t CHooks::ResizeBuffers1;
static HRESULT WINAPI Hooks::ResizeBuffers1(IDXGISwapChain* pSwapChain,
                                       UINT BufferCount, UINT Width,
                                       UINT Height, DXGI_FORMAT NewFormat,
                                       UINT SwapChainFlags,
                                       const UINT* pCreationNodeMask,
                                       IUnknown* const* ppPresentQueue) {
  DirectXHook::CleanupRenderTarget();

  return CHooks::ResizeBuffers1.m_pOriginalFn(pSwapChain, BufferCount, Width, Height,
                                        NewFormat, SwapChainFlags,
                                        pCreationNodeMask, ppPresentQueue);
}

// static CreateSwapChain_t CHooks::CreateSwapChain;
static HRESULT WINAPI Hooks::CreateSwapChain(IDXGIFactory* pFactory,
                                        IUnknown* pDevice,
                                        DXGI_SWAP_CHAIN_DESC* pDesc,
                                        IDXGISwapChain** ppSwapChain) {
  DirectXHook::CleanupRenderTarget();

  return CHooks::CreateSwapChain.m_pOriginalFn(pFactory, pDevice, pDesc, ppSwapChain);
}

// static CreateSwapChainForHwnd_t CHooks::CreateSwapChainForHwnd;
static HRESULT WINAPI Hooks::CreateSwapChainForHwnd(
    IDXGIFactory* pFactory, IUnknown* pDevice, HWND hWnd,
    const DXGI_SWAP_CHAIN_DESC1* pDesc,
    const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* pFullscreenDesc,
    IDXGIOutput* pRestrictToOutput, IDXGISwapChain1** ppSwapChain) {
  DirectXHook::CleanupRenderTarget();

  return CHooks::CreateSwapChainForHwnd.m_pOriginalFn(
      pFactory, pDevice, hWnd, pDesc, pFullscreenDesc, pRestrictToOutput,
      ppSwapChain);
}

// static CreateSwapChainForCoreWindow_t CHooks::CreateSwapChainForCoreWindow;
static HRESULT WINAPI Hooks::CreateSwapChainForCoreWindow(
    IDXGIFactory* pFactory, IUnknown* pDevice, IUnknown* pWindow,
    const DXGI_SWAP_CHAIN_DESC1* pDesc, IDXGIOutput* pRestrictToOutput,
    IDXGISwapChain1** ppSwapChain) {
  DirectXHook::CleanupRenderTarget();

  return CHooks::CreateSwapChainForCoreWindow.m_pOriginalFn(
      pFactory, pDevice, pWindow, pDesc, pRestrictToOutput, ppSwapChain);
}

// static CreateSwapChainForComposition_t CHooks::CreateSwapChainForComposition;
static HRESULT WINAPI Hooks::CreateSwapChainForComposition(
    IDXGIFactory* pFactory, IUnknown* pDevice,
    const DXGI_SWAP_CHAIN_DESC1* pDesc, IDXGIOutput* pRestrictToOutput,
    IDXGISwapChain1** ppSwapChain) {
  DirectXHook::CleanupRenderTarget();

  return CHooks::CreateSwapChainForComposition.m_pOriginalFn(
      pFactory, pDevice, pDesc, pRestrictToOutput, ppSwapChain);
}

void DirectXHook::Enter() {
  if (!CreateDeviceD3D11(WndProcHook::GetGameWindow())) {
    LOG(log_create_device_d3d11_failed.c_str());
    return;
  }

  // Hook
  IDXGIDevice* pDXGIDevice = NULL;
  g_pd3dDevice->QueryInterface(IID_PPV_ARGS(&pDXGIDevice));

  IDXGIAdapter* pDXGIAdapter = NULL;
  pDXGIDevice->GetAdapter(&pDXGIAdapter);

  IDXGIFactory* pIDXGIFactory = NULL;
  pDXGIAdapter->GetParent(IID_PPV_ARGS(&pIDXGIFactory));

  pIDXGIFactory->Release();
  pDXGIAdapter->Release();
  pDXGIDevice->Release();

  CHooks::CreateSwapChain.HookVirtual(pIDXGIFactory, EHookIndexes_Factory::CREATE_SWAP_CHAIN,
                                      HOOK_FUNCTION(Hooks::CreateSwapChain));
  CHooks::CreateSwapChainForHwnd.HookVirtual(
      pIDXGIFactory, EHookIndexes_Factory::CREATE_SWAP_CHAIN_FOR_HWND,
      HOOK_FUNCTION(Hooks::CreateSwapChainForHwnd));
  CHooks::CreateSwapChainForCoreWindow.HookVirtual(
      pIDXGIFactory, EHookIndexes_Factory::CREATE_SWAP_CHAIN_FOR_CORE_WINDOW,
      HOOK_FUNCTION(Hooks::CreateSwapChainForCoreWindow));
  CHooks::CreateSwapChainForComposition.HookVirtual(
      pIDXGIFactory, EHookIndexes_Factory::CREATE_SWAP_CHAIN_FOR_COMPOSITION,
      HOOK_FUNCTION(Hooks::CreateSwapChainForComposition));

  CHooks::Present.HookVirtual(g_pSwapChain, EHookIndexes_SwapChain::PRESENT,
                              HOOK_FUNCTION(Hooks::Present));
  CHooks::Present1.HookVirtual(g_pSwapChain, EHookIndexes_SwapChain::PRESENT1,
                               HOOK_FUNCTION(Hooks::Present1));

  CHooks::ResizeBuffers.HookVirtual(g_pSwapChain,
                                    EHookIndexes_SwapChain::RESIZE_BUFFERS,
                                    HOOK_FUNCTION(Hooks::ResizeBuffers));
  CHooks::ResizeBuffers1.HookVirtual(g_pSwapChain,
                                     EHookIndexes_SwapChain::RESIZE_BUFFERS_1,
                                     HOOK_FUNCTION(Hooks::ResizeBuffers1));

  CleanupDeviceD3D11();
}

void DirectXHook::Leave() {
  if (!ImGui::GetCurrentContext()) return;

  // DrawUtilities::destroy();

  ImGuiIO& io = ImGui::GetIO();

  if (io.BackendRendererUserData) {
    ImGui_ImplDX11_Shutdown();
  }

  if (io.BackendPlatformUserData) {
    ImGui_ImplWin32_Shutdown();
  }

  ImGui::DestroyContext();
  ImPlot::DestroyContext();
}

void DirectXHook::CleanupRenderTarget() {
  if (g_pd3dRenderTarget) {
    g_pd3dRenderTarget->Release();
    g_pd3dRenderTarget = NULL;
  }
}

void DirectXHook::CleanupDeviceD3D11() {
  CleanupRenderTarget();

  // PostProcessing::onDeviceReset();

  if (g_pSwapChain) {
    g_pSwapChain->Release();
    g_pSwapChain = NULL;
  }

  if (g_pd3dDevice) {
    g_pd3dDevice->Release();
    g_pd3dDevice = NULL;
  }

  if (g_pd3dDeviceContext) {
    g_pd3dDeviceContext->Release();
    g_pd3dDeviceContext = NULL;
  }
}

void DirectXHook::RenderImGui_DX11(IDXGISwapChain* pSwapChain) {
  if (!ImGui::GetCurrentContext() || os2::hooks::g_isShuttingDown) return;

  if (!ImGui::GetIO().BackendRendererUserData) {
    if (SUCCEEDED(pSwapChain->GetDevice(IID_PPV_ARGS(&g_pd3dDevice)))) {
      g_pd3dDevice->GetImmediateContext(&g_pd3dDeviceContext);
      ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

      OpenSource2_PrepareGUI();
    }
  }

  if (!g_pd3dRenderTarget) {
    CreateRenderTarget(pSwapChain);
  } else {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();

    //PostProcessing::setDevice(g_pd3dDevice, g_pd3dDeviceContext,
    //                          g_pd3dRenderTarget, pSwapChain);

    ImGui::NewFrame();

    /*try {
      g_pDrawUtilities = &DrawUtilities::instance();

      g_pDrawUtilities->Initialise(g_pd3dDevice, g_pd3dDeviceContext);

      LOG(log_create_draw_utilities_succeeded.c_str());
    } catch (std::exception& e) {
      LOG(log_create_draw_utilities_failed.c_str(), e.what());
    }*/

    void OpenSource2_Render();
    OpenSource2_Render();

    ImGui::EndFrame();
    ImGui::Render();

    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_pd3dRenderTarget, NULL);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
  }
}
