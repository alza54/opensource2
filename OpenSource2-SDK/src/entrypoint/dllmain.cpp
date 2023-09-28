// dllmain.cpp : Defines the entry point for the DLL application.
#define LOAD_SDK_FILES
#include "OpenSource2SDK.hpp"
#include "../sdk/framework.hpp"
#include "../sdk/config/config.hpp"

#include <locale>
#include <cstdio>

// static Config* g_Config = Config::GetInstance("CONFIG");

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

#if OPENSOURCE2_ENABLE_LOGGING
void InitialiseConsole() {
  AllocConsole();

  SetConsoleTitleA(os2::entrypoint::debug_console_title.c_str());

  // Enable I/O
  freopen_s(reinterpret_cast<FILE**> stdin, "CONIN$", "r", stdin);
  freopen_s(reinterpret_cast<FILE**> stdout, "CONOUT$", "w", stdout);
}
#endif

void InitialiseSDK() {
  std::setlocale(LC_ALL, ".utf8");

#if OPENSOURCE2_ENABLE_LOGGING
  InitialiseConsole();
#endif

  [[maybe_unused]] const auto moduleInfo = os2::entrypoint::WaitUntilModulesLoaded();

  LOG(os2::entrypoint::debug::modules_loaded.c_str(), moduleInfo.first.c_str(),
      moduleInfo.second.count());

  gameFeatures.emplace();

  os2::iface::initialise();
  os2::memory::initialise();
  os2::hooks::initialise();
}

void UnloadSDK() {
  os2::hooks::shutdown();
  os2::memory::shutdown();
  os2::iface::shutdown();

  if (gameFeatures.has_value()) gameFeatures.reset();

#if OPENSOURCE2_ENABLE_LOGGING
  fclose(stdin);
  fclose(stdout);
  FreeConsole();
#endif
}

BOOL WINAPI DllMain( HMODULE hModule, DWORD fdwReason, LPVOID lpvReserved)
{
  if (fdwReason == DLL_PROCESS_ATTACH) {
    DisableThreadLibraryCalls(hModule);

    HANDLE hThread = CreateThread(
        nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(InitialiseSDK),
        NULL, NULL, nullptr);

    if (hThread != NULL) {
      CloseHandle(hThread);
    }
  } else if (fdwReason == DLL_PROCESS_DETACH && !lpvReserved) {
    UnloadSDK();
  }

  return TRUE;
}

