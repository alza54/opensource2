#pragma once

#include "../../dependencies/stb/include/stb.hh"

#if OPENSOURCE2_ENABLE_LOGGING
#ifndef DEBUG_PREFIX
#define LOCAL_DEBUG_PREFIX (const char*)"[+] "
#else
#define LOCAL_DEBUG_PREFIX DEBUG_PREFIX.c_str()
#endif
#define LOG(...)                                         \
  printf(LOCAL_DEBUG_PREFIX); \
  printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

#define SIGNATURE(sig) \
  stb::simple_conversion::build<stb::fixed_string{sig}>::value

// Modules
#define CLIENT_DLL os2_string("client.dll")
#define ENGINE2_DLL os2_string("engine2.dll")
#define SCHEMASYSTEM_DLL os2_string("schemasystem.dll")
#define INPUTSYSTEM_DLL os2_string("inputsystem.dll")
#define SDL3_DLL os2_string("SDL3.dll")
#define TIER0_DLL os2_string("tier0.dll")
#define NAVSYSTEM_DLL os2_string("navsystem.dll")
#define RENDERSYSTEMVULKAN_DLL os2_string("rendersystemvulkan.dll")
#define LOCALIZE_DLL os2_string("localize.dll")
#define SCENESYSTEM_DLL os2_string("scenesystem.dll")
#define MATERIALSYSTEM2_DLL os2_string("materialsystem2.dll")
#define ANIMATIONSYSTEM_DLL os2_string("animationsystem.dll")

// Interfaces
#define GAME_RESOURCE_SERVICE_CLIENT os2_string("GameResourceServiceClientV001")
#define SOURCE2_CLIENT os2_string("Source2Client002")
#define SCHEMA_SYSTEM os2_string("SchemaSystem_001")
#define INPUT_SYSTEM_VERSION os2_string("InputSystemVersion001")
#define SOURCE2_ENGINE_TO_CLIENT os2_string("Source2EngineToClient001")
#define ENGINE_CVAR os2_string("VEngineCvar007")
#define LOCALIZE os2_string("Localize_001")
#define MATERIAL_SYSTEM2 os2_string("VMaterialSystem2_001")

// Signatures
#define GET_HIGHEST_ENTITY_INDEX SIGNATURE("33 DB E8 ? ? ? ? 8B 08")

#define GET_BASE_ENTITY SIGNATURE("8B D3 E8 ? ? ? ? 48 8B F8 48 85 C0 74 76")

#define PRINT_SCHEMA_DETAILED_CLASS_LAYOUT \
  SIGNATURE("48 89 5C 24 ? 48 89 6C 24 ? 48 89 4C 24 ?")

#define MOUSE_INPUT_ENABLED \
  SIGNATURE("40 53 48 83 EC 20 80 B9 ? ? ? ? ? 48 8B D9 75 78")

#define SET_MESH_GROUP_MASK \
  SIGNATURE("E8 ? ? ? ? 48 8B 5C 24 ? 4C 8B 7C 24 ? 4C 8B 74 24 ?")

#define CREATE_SHARED_OBJECT_SUBCLASS_ECON_ITEM                                \
  SIGNATURE("48 83 EC 28 B9 ? ? ? ? E8 ? ? ? ? 48 85 C0 74 3D 48 8D 0D ? ? ? " \
            "?")

#define GET_GC_CLIENT_SYSTEM SIGNATURE("E8 ? ? ? ? 48 8B 4F 10 8B 1D ? ? ? ?")

#define FIND_SO_CACHE SIGNATURE("E8 ? ? ? ? 48 8B F0 48 85 C0 74 0E 4C 8B C3")

#define GET_LOCAL_PLAYER_CONTROLLER SIGNATURE("E8 ? ? ? ? 49 89 47 08")

#define SET_DYNAMIC_ATTRIBUTE_VALUE_UINT                                     \
  SIGNATURE(                                                                 \
      "E9 ? ? ? ? CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC 49 8B C0 48 " \
      "8B CA 48 8B D0")

#define SET_MODEL SIGNATURE("E8 ? ? ? ? F3 0F 10 4C 3B ?")

#define COMPUTE_HITBOX_SURROUNDING_BOX SIGNATURE("E9 ? ? ? ? F6 43 5B FD")

#define GET_MATRICES_FOR_VIEW SIGNATURE("40 53 48 81 EC ? ? ? ? 49 8B C1")

#define ADD_STATTRAK_ENTITY SIGNATURE("40 55 41 55 48 8D 6C 24 ?")

#define ADD_NAMETAG_ENTITY                                                    \
  SIGNATURE("E8 ? ? ? ? 48 8B CB E8 ? ? ? ? 48 8B C8 E8 ? ? ? ? 88 83 ? ? ? " \
            "?")

#define IS_PAINT_KIT_USING_LEGACY_MODEL \
  SIGNATURE("48 8B C4 48 89 70 20 55 48 8D 68 A1")

#define BONE_FLAGS SIGNATURE("85 D2 78 16 3B 91")

#define BONE_PARENT SIGNATURE("85 D2 78 17 3B 91 70")

#define BONE_NAME SIGNATURE("85 D2 78 25 3B 91")

#define BONE_INDEX SIGNATURE("E8 ? ? ? ? 85 C0 78 ? 4C 8D 4D")

#define VIEW_MATRIX SIGNATURE("48 8D 0D ? ? ? ? 48 C1 E0 06")

#define CSGO_INPUT SIGNATURE("48 8D 0D ? ? ? ? E8 ? ? ? ? 66 0F 6F 05")

#define SET_VIEW_ANGLES SIGNATURE("F2 41 0F 10 00 4C 63 CA")

#define GET_VIEW_ANGLES SIGNATURE("4C 63 C2 4B 8D 04 40 4C 8D 0C C1")

#define GET_PLAYER_CONTROLLER \
  SIGNATURE("E8 ? ? ? ? 41 83 BF ? ? ? ? ?")

#define GET_PLAYER_PAWN SIGNATURE("E8 ? ? ? ? 48 8B 88 ? ? ? ? 48 8B 01 FF 90 ? ? ? ? 48 8B C8")

#define GET_ENTITY_INDEX SIGNATURE("E8 ? ? ? ? 48 3B F8 0F 85")

#define GET_WEAPON_DATA \
  SIGNATURE("48 81 EC ? ? ? ? 48 85 C9 75 ? 33 C0 48 81 C4 ? ? ? ? C3 48 89 9C 24")

#define GLOBAL_VARS SIGNATURE("48 8B 05 ? ? ? ? 48 8B D8 80 78 3D 00 75 1D 80 78 3C 00 75")

#define LEVEL_INIT SIGNATURE("48 89 5C 24 ? 56 48 83 EC ? 48 8B 0D ? ? ? ? 48 8B F2")

#define GET_SURFACE_DATA SIGNATURE("E8 ? ? ? ? 48 85 C0 74 ? 44 38 60")

#define GET_HANDLE_FROM_ENTITY                                                \
  SIGNATURE(                                                                  \
      "48 85 C9 74 ? 48 8B 41 ? 48 85 C0 74 ? 44 8B 40 ? BA ? ? ? ? 8B 48 ? " \
      "41 8B C0 83 E1")

#define TRACE_SHAPE SIGNATURE("E8 ? ? ? ? 80 7D ? ? 75 ? F3 0F 10 45")

#define ENGINE_TRACE SIGNATURE("4C 8B 3D ? ? ? ? 24 C9 0C 49 66 0F 7F 45 ?")

#define GET_SCENE_CAMERA SIGNATURE("48 83 EC ? 90 80 3D ? ? ? ? ? 74 ? 48 83 C4")

#define GET_SCENE_CAMERA_FOV SIGNATURE("F3 0F 10 81 ? ? ? ? C3 CC CC CC CC CC CC CC 0F B6 41")

// You can also do "45 0F 57 C9 44 0F 2F 08" + 0x16
#define GET_IN_GAME_FOV_PTR                                                  \
  SIGNATURE(                                                                 \
      "E8 ? ? ? ? 48 85 C0 75 ? 48 8B 05 ? ? ? ? 48 8B 40 ? F3 0F 10 00 44 " \
      "0F 28 4C 24")

#define SET_IN_GAME_FOV \
  SIGNATURE("40 53 48 81 EC ? ? ? ? 48 8B D9 E8 ? ? ? ? 48 85 C0")

#define TRACE_SMOKE SIGNATURE("45 8B 00 48 83 C1")

#define GET_HITBOX_SET \
  SIGNATURE("E8 ? ? ? ? 48 85 C0 0F 85 ? ? ? ? 44 8D 48 07")

#define HITBOX_TO_WORLD_TRANSFORMS SIGNATURE("E8 ? ? ? ? 45 33 F6 4C 63 E0")
