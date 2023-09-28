#include "wndproc_hook.hpp"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_win32.h>
#include <implot/implot.h>

#include <thread>

#include "../../../menu/menu.hpp"

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg,
                                                      WPARAM wParam,
                                                      LPARAM lParam);

LRESULT WndProcHook::hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam,
  LPARAM lParam) {
  if (!ImGui::GetCurrentContext()) {
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGui_ImplWin32_Init(hWnd);
    ImGuiIO &io = ImGui::GetIO();
    io.IniFilename = io.LogFilename = nullptr;
  }

  ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

  if (os2::menu::IsOpen()) {
    // Messages handled by 'ImGui_ImplWin32_WndProcHandler'.
    switch (uMsg) {
      case WM_MOUSEMOVE:
      case WM_NCMOUSEMOVE:
      case WM_MOUSELEAVE:
      case WM_NCMOUSELEAVE:
      case WM_LBUTTONDOWN:
      case WM_LBUTTONDBLCLK:
      case WM_RBUTTONDOWN:
      case WM_RBUTTONDBLCLK:
      case WM_MBUTTONDOWN:
      case WM_MBUTTONDBLCLK:
      case WM_XBUTTONDOWN:
      case WM_XBUTTONDBLCLK:
      case WM_LBUTTONUP:
      case WM_RBUTTONUP:
      case WM_MBUTTONUP:
      case WM_XBUTTONUP:
      case WM_MOUSEWHEEL:
      case WM_MOUSEHWHEEL:
      case WM_KEYDOWN:
      case WM_KEYUP:
      case WM_SYSKEYDOWN:
      case WM_SYSKEYUP:
      case WM_SETFOCUS:
      case WM_KILLFOCUS:
      case WM_CHAR:
      case WM_SETCURSOR:
      case WM_DEVICECHANGE:
        return 1;
    }
  }

  return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK WndProcHook::EnumWindowsCallback(HWND handle, LPARAM lParam) {
  const auto isMainWindow = [handle]() {
    return GetWindow(handle, GW_OWNER) == nullptr && IsWindowVisible(handle) &&
           handle != GetConsoleWindow();
  };

  DWORD pID = 0;
  GetWindowThreadProcessId(handle, &pID);

  if (GetCurrentProcessId() != pID || !isMainWindow()) return TRUE;

  *reinterpret_cast<HWND *>(lParam) = handle;
  return FALSE;
}

void WndProcHook::Enter() {
  while (!hWindow) {
    EnumWindows(WndProcHook::EnumWindowsCallback, reinterpret_cast<LPARAM>(&hWindow));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  oWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(
      hWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hkWndProc)));

  LOG(WndProcHook::szAfterHookLog.c_str());
}

void WndProcHook::Leave() {
  if (oWndProc) {
    SetWindowLongPtr(hWindow, GWLP_WNDPROC,
                     reinterpret_cast<LONG_PTR>(oWndProc));

    LOG(WndProcHook::szAfterLeaveLog.c_str());
  }
}
