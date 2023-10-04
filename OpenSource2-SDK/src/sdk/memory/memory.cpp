#include "memory.hpp"

#include <stb/include/stb.hh>

#include "../interfaces/interfaces.hpp"

#include "../utilities/static_string.hpp"

#define FUNCTION_VARIABLE(var) var, "os2::fn::" #var

void os2::memory::initialise() {
  using namespace os2::fn;

  os2::module::client->FindPattern(GET_BASE_ENTITY)
      .ToAbsolute(3, 0)
      .Get(FUNCTION_VARIABLE(GetBaseEntity));

  os2::module::client->FindPattern(GET_HIGHEST_ENTITY_INDEX)
      .ToAbsolute(3, 0)
      .Get(FUNCTION_VARIABLE(GetHighestEntityIndex));

  os2::module::client
      ->FindPattern(MOUSE_INPUT_ENABLED)
      .Get(FUNCTION_VARIABLE(MouseInputEnabled));

  os2::module::schemasystem
      ->FindPattern(PRINT_SCHEMA_DETAILED_CLASS_LAYOUT)
      .Get(FUNCTION_VARIABLE(SchemaDetailedClassLayout));

  os2::module::client
      ->FindPattern(SET_MESH_GROUP_MASK)
      .ToAbsolute(1, 0)
      .Get(FUNCTION_VARIABLE(SetMeshGroupMask));

  os2::module::client
       ->FindPattern(GET_LOCAL_PLAYER_CONTROLLER)
      .ToAbsolute(1, 0)
      .Get(FUNCTION_VARIABLE(GetLocalPlayerController));

  os2::module::client
      ->FindPattern(CREATE_SHARED_OBJECT_SUBCLASS_ECON_ITEM)
      .Get(FUNCTION_VARIABLE(CreateSharedObjectSubclassEconItem));

  os2::module::client
      ->FindPattern(GET_GC_CLIENT_SYSTEM)
      .ToAbsolute(1, 0)
      .Get(FUNCTION_VARIABLE(GetClientSystem));

  os2::module::client
      ->FindPattern(FIND_SO_CACHE)
      .ToAbsolute(1, 0)
      .Get(FUNCTION_VARIABLE(FindSOCache));

  os2::module::client
      ->FindPattern(SET_DYNAMIC_ATTRIBUTE_VALUE_UINT)
      .ToAbsolute(1, 0)
      .Get(FUNCTION_VARIABLE(SetDynamicAttributeValueUint));

  os2::module::client
      ->FindPattern(SET_MODEL)
      .ToAbsolute(1, 0)
      .Get(FUNCTION_VARIABLE(SetModel));

  os2::module::client
      ->FindPattern(COMPUTE_HITBOX_SURROUNDING_BOX)
      .ToAbsolute(1, 0)
      .Get(FUNCTION_VARIABLE(ComputeHitboxSurroundingBox));

  os2::module::client
      ->FindPattern(GET_MATRICES_FOR_VIEW)
      .Get(FUNCTION_VARIABLE(GetMatricesForView));

  os2::module::client
      ->FindPattern(ADD_STATTRAK_ENTITY)
      .Get(FUNCTION_VARIABLE(AddStattrakEntity));

  os2::module::client
      ->FindPattern(ADD_NAMETAG_ENTITY)
      .ToAbsolute(1, 0)
      .Get(FUNCTION_VARIABLE(AddNametagEntity));

  os2::module::client
       ->FindPattern(IS_PAINT_KIT_USING_LEGACY_MODEL)
       .Get(FUNCTION_VARIABLE(IsPaintKitUsingLegacyModel));

  os2::module::client
       ->FindPattern(BONE_FLAGS)
      .Get(FUNCTION_VARIABLE(GetBoneFlags));

  os2::module::client
      ->FindPattern(BONE_PARENT)
      .Get(FUNCTION_VARIABLE(GetBoneParent));

  os2::module::client->FindPattern(BONE_NAME).Get(
      FUNCTION_VARIABLE(GetBoneName));

  os2::fn::ViewMatrixAddress =
      os2::module::client->FindPattern(VIEW_MATRIX).Abs().Address();
  LOG(strings::log_viewmatrix_address.c_str(), os2::fn::ViewMatrixAddress);

  os2::fn::GlobalVarsAddress =
      os2::module::client->FindPattern(GLOBAL_VARS).Abs().Address();
  LOG(strings::log_globalvars_address.c_str(), os2::fn::GlobalVarsAddress);

  os2::module::client->FindPattern(LEVEL_INIT)
      .Get(FUNCTION_VARIABLE(LevelInit));

  os2::module::client
       ->FindPattern(CSGO_INPUT)
      .Abs()
      .Get(FUNCTION_VARIABLE(pCSGOInput));

  os2::module::client
    ->FindPattern(SET_VIEW_ANGLES)
      .Get(FUNCTION_VARIABLE(SetViewAngles));

  os2::module::client
    ->FindPattern(GET_VIEW_ANGLES)
      .Get(FUNCTION_VARIABLE(GetViewAngles));

  os2::module::client
       ->FindPattern(GET_PLAYER_CONTROLLER)
      .ToAbsolute(1, 0)
      .Get(FUNCTION_VARIABLE(GetPlayerController));

  os2::module::client
       ->FindPattern(GET_PLAYER_PAWN)
      .Jmp()
      .Get(FUNCTION_VARIABLE(GetPlayerPawn));

  os2::module::client
       ->FindPattern(GET_ENTITY_INDEX)
      .Jmp()
      .Get(FUNCTION_VARIABLE(GetEntityByIndex));

  os2::module::client
       ->FindPattern(GET_WEAPON_DATA)
      .Get(FUNCTION_VARIABLE(GetWeaponData));

  // Tracing:
  os2::module::client->FindPattern(GET_SURFACE_DATA)
      .ToAbsolute(1, 0)
      .Get(FUNCTION_VARIABLE(GetSurfaceData));

  os2::module::client->FindPattern(GET_HANDLE_FROM_ENTITY)
      .Get(FUNCTION_VARIABLE(GetEntityHandle));

  os2::module::client->FindPattern(TRACE_SHAPE)
      .Jmp()
      .Get(FUNCTION_VARIABLE(TraceShape));

  os2::module::client->FindPattern(ENGINE_TRACE)
      .ToAbsolute(3, 0)
      .Dereference(1)
      .Get(FUNCTION_VARIABLE(EngineTrace));

  os2::module::client->FindPattern(GET_SCENE_CAMERA)
      .Get(FUNCTION_VARIABLE(GetSceneCamera));

  os2::module::client->FindPattern(GET_SCENE_CAMERA_FOV)
      .Get(FUNCTION_VARIABLE(GetSceneCameraFov));

  os2::module::client->FindPattern(SET_IN_GAME_FOV)
      .Get(FUNCTION_VARIABLE(SetInGameFov));

  os2::module::client->FindPattern(GET_IN_GAME_FOV_PTR)
      .ToAbsolute(1, 0)
      .Get(FUNCTION_VARIABLE(GetInGameFovPtr));

  os2::module::client->FindPattern(TRACE_SMOKE)
      .Get(FUNCTION_VARIABLE(TraceSmoke));

  os2::sdk::CModule sdl3(os2::memory::strings::sdl3_dll);

  // SDL Functions:
  sdl3.GetProcAddress(strings::sdl_set_relative_mouse_mode.c_str())
      .Get(SDL_SetRelativeMouseMode);
  sdl3.GetProcAddress(strings::sdl_set_window_grab.c_str())
      .Get(SDL_SetWindowGrab);
  sdl3.GetProcAddress(strings::sdl_warp_mouse_in_window.c_str())
      .Get(SDL_WarpMouseInWindow);
}

void os2::memory::shutdown() {}
