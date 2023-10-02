#pragma once

#define OS2_STRING_LIST inline const std::string

#define COMBINE(x, y) x##y
#define COMBINE2(x, y) COMBINE(x, y)

#define PAD_CLASS(sz)                           \
 private:                                       \
  std::uint8_t COMBINE2(pad_, __COUNTER__)[sz]; \
                                                \
 public:

#include "../framework.hpp"

#include "../utilities/static_string.hpp"

#include "../math/math.hpp"

#include "../schema/schema.hpp"

#include "../module/module.hpp"

#include "../source-sdk/classes/entity/ccsplayercontroller.hpp"
#include "../source-sdk/classes/gcsdk/cgcclientsystem.hpp"

#include "../source-sdk/classes/cstronghandle.hpp"
#include "../source-sdk/classes/cutlsymbollarge.hpp"

#include "../source-sdk/classes/ccsgoinput.hpp"
#include "../source-sdk/classes/entity/cgamescenenode.hpp"
#include "../source-sdk/classes/entity/bones/cbonesystem.hpp"

#include "../source-sdk/classes/ccsgoinput.hpp"

// #include "../source-sdk/classes/c_trace.hpp"

// #include "../source-sdk/interfaces/cgametracemanager.hpp"

#include "../source-sdk/classes/types/cglobalvarsbase.hpp"

#include "../source-sdk/interfaces/cgameeventmanager.hpp"

namespace os2 {
  namespace memory {
    namespace strings {
      OS2_STRING_LIST client_dll = CLIENT_DLL;
      OS2_STRING_LIST engine2_dll = ENGINE2_DLL;
      OS2_STRING_LIST schemasystem_dll = SCHEMASYSTEM_DLL;
      OS2_STRING_LIST inputsystem_dll = INPUTSYSTEM_DLL;
      OS2_STRING_LIST tier0_dll = TIER0_DLL;
      OS2_STRING_LIST localize_dll = LOCALIZE_DLL;
      OS2_STRING_LIST materialsystem2_dll = MATERIALSYSTEM2_DLL;
      OS2_STRING_LIST sdl3_dll = SDL3_DLL;

      OS2_STRING_LIST game_resource_service_client =
          GAME_RESOURCE_SERVICE_CLIENT;
      OS2_STRING_LIST source2_client = SOURCE2_CLIENT;
      OS2_STRING_LIST input_system_version = INPUT_SYSTEM_VERSION;
      OS2_STRING_LIST schema_system = SCHEMA_SYSTEM;
      OS2_STRING_LIST source2_engine = SOURCE2_ENGINE_TO_CLIENT;
      OS2_STRING_LIST engine_cvar = ENGINE_CVAR;
      OS2_STRING_LIST localize = LOCALIZE;
      OS2_STRING_LIST material_system2 = MATERIAL_SYSTEM2;

      OS2_STRING_LIST sdl_set_relative_mouse_mode =
          os2_string("SDL_SetRelativeMouseMode");
      OS2_STRING_LIST sdl_set_window_grab =
          os2_string("SDL_SetWindowGrab");
      OS2_STRING_LIST sdl_warp_mouse_in_window =
          os2_string("SDL_WarpMouseInWindow");

      OS2_STRING_LIST log_viewmatrix_address =
          os2_string("ViewMatrixAddress found at -> %llX\n");
      OS2_STRING_LIST log_globalvars_address =
          os2_string("GlobalVarsAddress found at -> %llX\n");
    }; // namespace strings

    void initialise();
    void shutdown();
  }; // namespace memory

  namespace fn {
    inline void* MouseInputEnabled;
    inline void* GetMatricesForView;
    inline void* LevelInit;
    inline void* EngineTrace;

    inline void*(__thiscall* GetBaseEntity)(void*, int);
    inline int(__thiscall* GetHighestEntityIndex)(void*, int*);
    inline void*(__thiscall* SchemaDetailedClassLayout)(void*, const char*);
    inline void(__thiscall* SetMeshGroupMask)(void*, uint64_t);
    inline os2::sdk::CCSPlayerController*(__fastcall* GetLocalPlayerController)(int);
    inline os2::sdk::CEconItem* (*CreateSharedObjectSubclassEconItem)();
    inline os2::sdk::CGCClientSystem* (*GetClientSystem)();
    inline os2::sdk::CGCClientSharedObjectCache*(__thiscall* FindSOCache)(
        void*, os2::sdk::SOID_t, bool);
    inline void*(__fastcall* SetDynamicAttributeValueUint)(void*, void*,
                                                             void*);
    inline void*(__fastcall* SetModel)(void*, const char*);
    inline bool(__fastcall* ComputeHitboxSurroundingBox)(void*,
                                                         os2::sdk::Vector&,
                                                         os2::sdk::Vector&);
    inline void(__fastcall* AddStattrakEntity)(void*);
    inline void(__fastcall* AddNametagEntity)(void*);
    inline bool(__fastcall* IsPaintKitUsingLegacyModel)(const char*);

    inline std::int32_t(__fastcall* GetBoneFlags)(os2::sdk::CModel*,
                                                  std::int32_t);
    inline std::int32_t(__fastcall* GetBoneParent)(os2::sdk::CModel*,
                                                   std::int32_t);

    inline uintptr_t ViewMatrixAddress;
    inline uintptr_t GlobalVarsAddress;

    inline bool(__fastcall* SetViewAngles)(os2::sdk::CCSGOInput*, std::int32_t,
                                           const glm::vec3&);
    inline std::uint64_t(__fastcall* GetViewAngles)(os2::sdk::CCSGOInput*,
                                                    std::int32_t);

    inline os2::sdk::CCSPlayerController*(__fastcall* GetPlayerController)(
        std::int32_t);
    inline os2::sdk::C_CSPlayerPawn*(__fastcall* GetPlayerPawn)(
        os2::sdk::CCSPlayerController*);
    inline os2::sdk::CCSPlayerController*(__fastcall* GetEntityByIndex)(
        std::int32_t);

    // SDL3 Functions
    inline int(__stdcall* SDL_SetRelativeMouseMode)(int);
    inline int(__stdcall* SDL_SetWindowGrab)(void*, int);
    inline int(__stdcall* SDL_WarpMouseInWindow)(void*, float, float);

    // Tracing
    inline std::uint64_t(__fastcall* GetSurfaceData)(void*);
    inline std::uint32_t(__fastcall* GetEntityHandle)(void*);
    inline bool(__fastcall* TraceShape)(void*, void*, os2::sdk::Vector*,
                                        os2::sdk::Vector*, void*, void*);
    inline float (__fastcall* TraceSmoke)(os2::sdk::Vector*, os2::sdk::Vector*, void* ptr);

    inline os2::sdk::CCSWeaponBaseVData*(__fastcall* GetWeaponData)(
        os2::sdk::C_EconItemView*);

    inline os2::sdk::CCSGOInput* pCSGOInput;
  }; // namespace fn
};  // namespace os2

#undef OS2_STRING_LIST
