#include <Windows.h>
#include <psapi.h>
#include <set>

#include "OpenSource2SDK.hpp"

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

std::chrono::duration<float> time_since(std::chrono::system_clock::time_point time) {
    return std::chrono::system_clock::now().time_since_epoch() -
           time.time_since_epoch();
}

WaitUntilModulesLoaded_t os2::entrypoint::WaitUntilModulesLoaded(
  std::chrono::duration<float> timeout,
  std::chrono::duration<float> pool_rate) {
  const auto start_timestamp = std::chrono::system_clock::now();

  HANDLE hProcess = GetCurrentProcess();

  if (hProcess == NULL) {
    std::this_thread::sleep_for(timeout);
    return std::make_pair(std::string(), timeout);
  }

  HMODULE hModules[1024];
  DWORD cbNeeded;

  std::set<std::string> modulesSet = {};

  auto load_timestamp = std::chrono::system_clock::now();

  std::string last_module_name = "";

  while (true) {
    const auto time_since_last_module = time_since(load_timestamp);

    if (time_since_last_module >= timeout) {
      return std::make_pair(last_module_name, time_since(start_timestamp));
    }

    if (EnumProcessModules(hProcess, hModules, sizeof(hModules), &cbNeeded)) {
      unsigned int i;
      for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
        char szModName[MAX_PATH];

        if (GetModuleFileNameExA(hProcess, hModules[i], szModName,
                                sizeof(szModName) / sizeof(TCHAR))) {
          std::string moduleName = std::string(szModName);

          if (moduleName.find(os2::entrypoint::game_path_part) != std::string::npos) {
            auto insertStatus = modulesSet.insert(moduleName);

            const bool didInsert = insertStatus.second;

            // Actually inserted value into the set (it is a new value)
            if (didInsert) {
              load_timestamp = std::chrono::system_clock::now();
              last_module_name = moduleName.substr(moduleName.rfind('\\') + 1);
            }
          }
        }
      }
    }

    std::this_thread::sleep_for(pool_rate);
  }
}

void os2::entrypoint::BeginUnloadProcedure() noexcept {
  HANDLE hThread = CreateThread(
      nullptr, NULL,
      [](LPVOID pData) -> DWORD {
        FreeLibraryAndExitThread((HMODULE)(pData), EXIT_SUCCESS);
      },
      HINST_THISCOMPONENT, NULL, nullptr);

  if (hThread != NULL) {
    CloseHandle(hThread);
  }
}
