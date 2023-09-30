#include "game_hooks.hpp"

#include "../../game/state.hpp"
#include "../../menu/menu.hpp"
#include "../../gamedata/gamedata.hpp"
#include "../features.hpp"

using namespace Game;
using namespace os2::sdk;

enum class MoveType { NOCLIP = 8, LADDER = 9 };

template <class T>
void ResolveFromAddress(std::uint64_t address, T& out,
                        bool dereference = false) {
  if (!address) {
    if (std::is_pointer_v<T>) out = nullptr;
    return;
  }

  if (dereference) address = *reinterpret_cast<uint64_t*>(address);

  out = reinterpret_cast<T>(address);

  LOG(os2::memory::strings::log_globalvars_address.c_str(), address);
}

static bool __fastcall Hooks::MouseInputEnabled(void* rcx) {
  if (os2::menu::IsOpen()) return false;
  return CHooks::MouseInputEnabled(rcx);
}

static void __fastcall Hooks::GetMatricesForView(void* rcx, void* view,
                                                 VMatrix* pWorldToView,
                                                 VMatrix* pViewToProjection,
                                                 VMatrix* pWorldToProjection,
                                                 VMatrix* pWorldToPixels) {
  CHooks::GetMatricesForView(rcx, view, pWorldToView,
                                           pViewToProjection,
                                           pWorldToProjection, pWorldToPixels);

  // Provide data for the math.
  os2::math::UpdateViewMatrix(pWorldToProjection);

  // Update ESP bounding boxes.
  gameFeatures->esp->CalculateBoundingBoxes();
}

static void* __fastcall Hooks::OnAddEntity(void* rcx, CEntityInstance* pInstance, CHandle hHandle) {
  Data::g_pGameData->OnAddEntity(pInstance, hHandle);
  return CHooks::OnAddEntity(rcx, pInstance, hHandle);
}

static void* __fastcall Hooks::OnRemoveEntity(void* rcx, CEntityInstance* pInstance, CHandle hHandle) {
  Data::g_pGameData->OnRemoveEntity(pInstance, hHandle);
  return CHooks::OnRemoveEntity(rcx, pInstance, hHandle);
}

static void __fastcall Hooks::CreateMove(CCSGOInput* input, const std::int32_t slot, const bool active) {
  CHooks::CreateMove(input, slot, active);

  CUserCmd* cmd = input->get_user_cmd();

  if (cmd == nullptr) {
    return;
  }

  auto base_cmd = *cmd->m_base_cmd;

  if (base_cmd.m_viewangles == NULL) {
    return;
  }

  auto view_angles =
      *reinterpret_cast<glm::vec3*>(os2::fn::GetViewAngles(input, 0));

  gameFeatures->OnCreateMove(input, cmd, view_angles);
}

static void __fastcall Hooks::FrameStageNotify(void* rcx, const std::int32_t stage) {
  State::LocalController = CGameEntitySystem::GetLocalPlayerController();
  State::LocalPawn = State::LocalController != nullptr
                         ? State::LocalController->m_hPawn().Get<C_CSPlayerPawn>()
                         : nullptr;

  CHooks::FrameStageNotify(rcx, stage);
}

static __int64 __fastcall Hooks::LevelInit(void* rcx) {
  os2::fn::GlobalVarsAddress = os2::module::client->FindPattern(GLOBAL_VARS).Abs().Address();

  ResolveFromAddress(os2::fn::GlobalVarsAddress, State::GlobalVars, true);

  return CHooks::LevelInit(rcx);
}

void os2::game::initialise() {
  ResolveFromAddress(os2::fn::GlobalVarsAddress, State::GlobalVars, true);

  Data::g_pGameData.emplace();

  CHooks::MouseInputEnabled.Hook(os2::fn::MouseInputEnabled,
                                 HOOK_FUNCTION(Hooks::MouseInputEnabled));
  CHooks::GetMatricesForView.Hook(os2::fn::GetMatricesForView,
                                  HOOK_FUNCTION(Hooks::GetMatricesForView));
  CHooks::OnAddEntity.HookVirtual(CGameEntitySystem::GetInstance(),
                                  ON_ADD_ENTITY,
                                  HOOK_FUNCTION(Hooks::OnAddEntity));
  CHooks::OnRemoveEntity.HookVirtual(CGameEntitySystem::GetInstance(),
                                     ON_REMOVE_ENTITY,
                                     HOOK_FUNCTION(Hooks::OnRemoveEntity));
  CHooks::CreateMove.HookVirtual(os2::fn::pCSGOInput, CREATE_MOVE,
                                 HOOK_FUNCTION(Hooks::CreateMove));
  CHooks::FrameStageNotify.HookVirtual(os2::iface::pClient, FRAME_STAGE_NOTIFY,
                                       HOOK_FUNCTION(Hooks::FrameStageNotify));
  CHooks::LevelInit.Hook(os2::fn::LevelInit, HOOK_FUNCTION(Hooks::LevelInit));

  Data::g_pGameData->CacheCurrentEntities();
}

void os2::game::unhook() {
  os2::menu::Toggle(false);
  Game::Data::g_pGameData.reset();
}
