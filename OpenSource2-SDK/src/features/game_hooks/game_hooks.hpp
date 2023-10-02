#pragma once

#include "../../sdk/hooks/hook.hpp"
#include "../../sdk/interfaces/interfaces.hpp"

using namespace os2::sdk;

typedef CHook<bool __fastcall(void*)> MouseInputEnabled_t;
typedef CHook<void __fastcall(void*, void*, VMatrix*, VMatrix*, VMatrix*,
                              VMatrix*)>
    GetMatricesForView_t;
typedef CHook<void* __fastcall(void*, CEntityInstance*, CHandle)> OnAddEntity_t;
typedef CHook<void* __fastcall(void*, CEntityInstance*, CHandle)>
    OnRemoveEntity_t;
typedef CHook<void __fastcall(CCSGOInput*, const std::int32_t, const bool)>
    CreateMove_t;
typedef CHook<void __fastcall(void*, const std::int32_t)> FrameStageNotify_t;
typedef CHook<__int64 __fastcall(void*)> LevelInit_t;

namespace CHooks {
static MouseInputEnabled_t MouseInputEnabled;
static GetMatricesForView_t GetMatricesForView;
static OnAddEntity_t OnAddEntity;
static OnRemoveEntity_t OnRemoveEntity;
static CreateMove_t CreateMove;
static FrameStageNotify_t FrameStageNotify;
static LevelInit_t LevelInit;
};  // namespace CHooks

namespace Hooks {
static bool __fastcall MouseInputEnabled(void* rcx);
static void __fastcall GetMatricesForView(void* rcx, void* view,
                                          VMatrix* pWorldToView,
                                          VMatrix* pViewToProjection,
                                          VMatrix* pWorldToProjection,
                                          VMatrix* pWorldToPixels);
static void* __fastcall OnAddEntity(void* rcx, CEntityInstance* pInstance,
                                    CHandle hHandle);
static void* __fastcall OnRemoveEntity(void* rcx, CEntityInstance* pInstance,
                                       CHandle hHandle);
static void __fastcall CreateMove(CCSGOInput* input, const std::int32_t slot,
                                  const bool active);
static void __fastcall FrameStageNotify(void* rcx, const std::int32_t stage);
static __int64 __fastcall LevelInit(void* rcx);
};  // namespace Hooks

const enum EHookIndexes {
  ON_ADD_ENTITY = 14,       // CGameEntitySystem instance
  ON_REMOVE_ENTITY = 15,    // CGameEntitySystem instance
  CREATE_MOVE = 5,          // CCSGOInput
  FRAME_STAGE_NOTIFY = 31,  // Client interface
};

namespace os2 {
namespace game {
void initialise();
void unhook();
};  // namespace game
};  // namespace os2
