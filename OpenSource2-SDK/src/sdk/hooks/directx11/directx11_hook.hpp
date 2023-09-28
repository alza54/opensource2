#pragma once

#include <Windows.h>

#include <d3d11.h>
#include <dxgi1_2.h>

#include <string>
#include <iostream>

#include <imgui/backends/imgui_impl_dx11.h>
#include <imgui/backends/imgui_impl_win32.h>
#include <implot/implot.h>

#include "../../utilities/static_string.hpp"
#include "../../framework.hpp"

#include "../hook.hpp"

typedef os2::sdk::CHook<HRESULT WINAPI(IDXGISwapChain*, UINT, UINT)>             Present_t;
typedef os2::sdk::CHook<HRESULT WINAPI(IDXGISwapChain*, UINT, UINT,
                                       const DXGI_PRESENT_PARAMETERS*)>          Present1_t;
typedef os2::sdk::CHook<HRESULT WINAPI(IDXGISwapChain*, UINT, UINT, UINT,
                                       DXGI_FORMAT, UINT)>                       ResizeBuffers_t;
typedef os2::sdk::CHook<HRESULT WINAPI(IDXGISwapChain*, UINT, UINT, UINT,
                                       DXGI_FORMAT, UINT, const UINT*,
                                       IUnknown* const*)>                        ResizeBuffers1_t;
typedef os2::sdk::CHook<HRESULT WINAPI(IDXGIFactory*, IUnknown*,
                                       DXGI_SWAP_CHAIN_DESC*, IDXGISwapChain**)> CreateSwapChain_t;
typedef os2::sdk::CHook<HRESULT WINAPI(
    IDXGIFactory*, IUnknown*, HWND, const DXGI_SWAP_CHAIN_DESC1*,
    const DXGI_SWAP_CHAIN_FULLSCREEN_DESC*, IDXGIOutput*, IDXGISwapChain1**)>    CreateSwapChainForHwnd_t;
typedef os2::sdk::CHook<HRESULT WINAPI(IDXGIFactory*, IUnknown*, IUnknown*,
                                       const DXGI_SWAP_CHAIN_DESC1*,
                                       IDXGIOutput*, IDXGISwapChain1**)>         CreateSwapChainForCoreWindow_t;
typedef os2::sdk::CHook<HRESULT WINAPI(IDXGIFactory*, IUnknown*,
                                       const DXGI_SWAP_CHAIN_DESC1*,
                                       IDXGIOutput*, IDXGISwapChain1**)>         CreateSwapChainForComposition_t;

namespace CHooks {
  static Present_t Present;
  static Present1_t Present1;
  static ResizeBuffers_t ResizeBuffers;
  static ResizeBuffers1_t ResizeBuffers1;
  static CreateSwapChain_t CreateSwapChain;
  static CreateSwapChainForHwnd_t CreateSwapChainForHwnd;
  static CreateSwapChainForCoreWindow_t CreateSwapChainForCoreWindow;
  static CreateSwapChainForComposition_t CreateSwapChainForComposition;
};

namespace Hooks {
  static HRESULT WINAPI Present(IDXGISwapChain* pSwapChain, UINT SyncInterval,
                         UINT Flags);
  static HRESULT WINAPI Present1(IDXGISwapChain* pSwapChain, UINT SyncInterval,
                          UINT PresentFlags,
                          const DXGI_PRESENT_PARAMETERS* pPresentParameters);
  static HRESULT WINAPI ResizeBuffers(IDXGISwapChain* pSwapChain, UINT BufferCount,
                               UINT Width, UINT Height, DXGI_FORMAT NewFormat,
                               UINT SwapChainFlags);
  static HRESULT WINAPI ResizeBuffers1(IDXGISwapChain* pSwapChain, UINT BufferCount,
                                UINT Width, UINT Height, DXGI_FORMAT NewFormat,
                                UINT SwapChainFlags,
                                const UINT* pCreationNodeMask,
                                IUnknown* const* ppPresentQueue);
  static HRESULT WINAPI CreateSwapChain(IDXGIFactory* pFactory, IUnknown* pDevice,
                                 DXGI_SWAP_CHAIN_DESC* pDesc,
                                 IDXGISwapChain** ppSwapChain);
  static HRESULT WINAPI CreateSwapChainForHwnd(
      IDXGIFactory* pFactory, IUnknown* pDevice, HWND hWnd,
      const DXGI_SWAP_CHAIN_DESC1* pDesc,
      const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* pFullscreenDesc,
      IDXGIOutput* pRestrictToOutput, IDXGISwapChain1** ppSwapChain);
  static HRESULT WINAPI CreateSwapChainForCoreWindow(
      IDXGIFactory* pFactory, IUnknown* pDevice, IUnknown* pWindow,
      const DXGI_SWAP_CHAIN_DESC1* pDesc, IDXGIOutput* pRestrictToOutput,
      IDXGISwapChain1** ppSwapChain);
  static HRESULT WINAPI CreateSwapChainForComposition(IDXGIFactory*, IUnknown*,
                                               const DXGI_SWAP_CHAIN_DESC1*,
                                               IDXGIOutput*, IDXGISwapChain1**);
};

class DirectXHook {
 private:
  static inline ID3D11Device* g_pd3dDevice = nullptr;
  static inline ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
  static inline ID3D11RenderTargetView* g_pd3dRenderTarget = nullptr;
  static inline IDXGISwapChain* g_pSwapChain = nullptr;

  static inline const std::string
      log_d3d11_create_device_and_swap_chain_failed = os2_string(
          "[!] D3D11CreateDeviceAndSwapChain() failed. [rv: "
          "%lu\n]\n");
  static inline const std::string log_create_device_d3d11_failed =
      os2_string("[!] CreateDeviceD3D11() failed.\n");
  static inline const std::string log_create_render_target_failed = os2_string(
      "CreateRenderTarget(): Falling back to "
      "'D3D11_RTV_DIMENSION_TEXTURE2D'.\n");
  static inline const std::string log_create_render_target_severe_error =
      os2_string("Severe error in CreateRenderTarget().\n");
  static inline const std::string log_create_draw_utilities_failed =
      os2_string("[!] Could not [!] initialise DrawUtilities, cause: %s\n");
  static inline const std::string log_create_draw_utilities_succeeded =
      os2_string("[+] Successfully initialised the draw utilities instance!\n");

  const enum EHookIndexes_Factory {
    CREATE_SWAP_CHAIN = 10,
    CREATE_SWAP_CHAIN_FOR_HWND = 15,
    CREATE_SWAP_CHAIN_FOR_CORE_WINDOW = 16,
    CREATE_SWAP_CHAIN_FOR_COMPOSITION = 24,
  };

  const enum EHookIndexes_SwapChain {
    PRESENT = 8,
    PRESENT1 = 22,

    RESIZE_BUFFERS = 13,
    RESIZE_BUFFERS_1 = 39
  };

  static bool CreateDeviceD3D11(HWND hWnd);
  static int GetCorrectDXGIFormat(int eCurrentFormat);
  static void CreateRenderTarget(IDXGISwapChain* pSwapChain);

  static void CleanupDeviceD3D11();
 public:
  void Enter();
  void Leave();

  static void RenderImGui_DX11(IDXGISwapChain* pSwapChain);

  static void CleanupRenderTarget();

  static inline ID3D11Device* GetDevice() { return DirectXHook::g_pd3dDevice; }
  static inline ID3D11DeviceContext* GetDeviceContext() { return DirectXHook::g_pd3dDeviceContext; }
};
