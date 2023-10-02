#pragma once

#include <iostream>
#include <string>
#include <array>
#include <string>

// Windows API
#include <Windows.h>
#include <Psapi.h>

#include "../interface/interface.hpp"
#include "../utilities/static_string.hpp"
#include "../framework.hpp"

const std::string log_ptr_get = os2_string("%s found at -> %llX\n");

const std::string log_get_proc_address =
    os2_string("GetProcAddress('%s', '%s') returned -> %p\n");
const std::string log_module_found = os2_string("Module '%s' found at -> %p\n");
const std::string str_create_interface = os2_string("CreateInterface");

namespace os2::sdk {
  // Pointer arithmetic utility class.
  struct UTILPtr {
   public:
    template <typename T>
    UTILPtr(T val) {
      m_val = (uintptr_t)(val);
    }

    template <typename T = void*>
    T Get(const char* variableName = nullptr) {
      if (variableName) {
        LOG(log_ptr_get.c_str(), variableName, m_val);
      }

      return (T)(m_val);
    }

    template <typename T>
    void Get(T& dst, const char* variableName = nullptr) {
      dst = Get<T>(variableName);
    }

    [[nodiscard]] __forceinline UTILPtr Abs(const std::size_t length = 7) const {
      std::uintptr_t base = m_val;

      const auto displacement = *reinterpret_cast<std::int32_t*>(base + 0x3);

      base += displacement;
      base += length;

      return UTILPtr(base);
    }

    [[nodiscard]] __forceinline UTILPtr Jmp(
        const std::ptrdiff_t offset = 0x1) const {
      std::uintptr_t base = m_val + offset;

      const auto displacement = *reinterpret_cast<std::int32_t*>(base);

      base += displacement;
      base += sizeof(std::uint32_t);

      return UTILPtr(base);
    }

    [[nodiscard]] __forceinline std::uintptr_t Address() const { return m_val; }

    UTILPtr& AddOffset(int offset) {
      if (m_val) m_val += offset;
      return *this;
    }

    UTILPtr& ToAbsolute(int preOffset, int postOffset) {
      if (m_val) {
        AddOffset(preOffset);
        m_val = m_val + sizeof(int) + *(int*)(m_val);
        AddOffset(postOffset);
      }
      return *this;
    }

    UTILPtr& Dereference(int dereferences) {
      if (m_val)
        while (dereferences-- != 0) m_val = *(uintptr_t*)(m_val);
      return *this;
    }

    bool IsValid() { return m_val != 0; }

   private:
    uintptr_t m_val;
  };

  class CModule {
   public:
    CModule(CModule&&) = delete;
    CModule(const CModule&) = delete;

    [[nodiscard]] static std::shared_ptr<CModule> Create(
        const std::string& name) {
      return std::make_shared<CModule>(name);
    }

    explicit CModule(const std::string& name) : m_name(name.c_str()) {
      this->Load();
    }

    void Load() {
      this->InitializeHandle();
      this->InitializeBounds();
    }

    UTILPtr GetProcAddress(const char* procName) const {
      UTILPtr rv = 0;
      if (this->IsLoaded()) {
  #ifdef IS_WINDOWS
        rv = ::GetProcAddress(static_cast<HMODULE>(this->m_handle), procName);
  #endif
      }

      LOG(log_get_proc_address.c_str(), this->GetName(), procName,
          rv.Get<void*>());
      return rv;
    }

    UTILPtr FindInterface(const char* version) const {
      UTILPtr rv = 0;
      if (this->IsLoaded()) {
        UTILPtr pCreateInterface =
            this->GetProcAddress(str_create_interface.c_str());
        if (!pCreateInterface.IsValid()) return rv;

        InterfaceReg* s_pInterfaceRegs =
            pCreateInterface.ToAbsolute(3, 0).Dereference(1).Get<InterfaceReg*>();

        for (; s_pInterfaceRegs; s_pInterfaceRegs = s_pInterfaceRegs->m_pNext) {
          if (strcmp(version, s_pInterfaceRegs->m_pName) == 0) {
            rv = s_pInterfaceRegs->m_CreateFn();
            break;
          }
        }
      }
      return rv;
    }

    template <size_t N>
    UTILPtr FindPattern(const std::array<int, N>& signature) const {
      UTILPtr rv = 0;
      if (this->IsLoaded()) {
        const int* pSigData = signature.data();
        uint8_t* pBytes = reinterpret_cast<uint8_t*>(this->m_start);
        for (size_t i = 0; i < this->m_end - N; ++i) {
          bool found = true;
          for (size_t j = 0; j < N; ++j) {
            if (pBytes[i + j] != pSigData[j] && pSigData[j] != -1) {
              found = false;
              break;
            }
          }

          if (found) {
            rv = reinterpret_cast<uintptr_t>(&pBytes[i]);
            break;
          }
        }
      }
      return rv;
    }

    template <typename T>
    T GetOffset(uint64_t offset) {
      return reinterpret_cast<T>(this->m_start + offset);
    }

    const char* GetName() const { return this->m_name; }

    bool IsLoaded() const { return this->m_handle != 0; }

   private:
    CModule() = default;

    void InitializeHandle() {
  #ifdef IS_WINDOWS
      this->m_handle = static_cast<void*>(GetModuleHandleA(this->GetName()));
  #endif

      LOG(log_module_found.c_str(), this->GetName(), this->m_handle);
    }
    void InitializeBounds() {
      if (!this->IsLoaded()) return;

  #ifdef IS_WINDOWS
      MODULEINFO mi;
      BOOL status = GetModuleInformation(GetCurrentProcess(),
                                         static_cast<HMODULE>(this->m_handle),
                                         &mi, sizeof(mi));
      if (status != 0) {
        this->m_start = reinterpret_cast<uintptr_t>(this->m_handle);
        this->m_end = static_cast<uintptr_t>(mi.SizeOfImage);
      }
  #endif
    }

    void* m_handle = nullptr;
    uintptr_t m_start = 0, m_end = 0;
    const char* m_name = "";
  };
}  // namespace os2::sdk
