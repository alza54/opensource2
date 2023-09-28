#pragma once

#include <Windows.h>

#include <string>
#include <iostream>

#include "../../utilities/static_string.hpp"
#include "../../framework.hpp"

class WndProcHook {
 private:
  static inline HWND hWindow = nullptr;
  static inline WNDPROC oWndProc = NULL;

  static inline std::string szAfterHookLog = os2_string("Hooked Input API!\n");
  static inline std::string szAfterLeaveLog =
      os2_string("Restored original Input API!\n");

  static LRESULT hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

  static BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam);

 public:
  void Enter();
  void Leave();

  constexpr static HWND GetGameWindow() { return hWindow; }
};
