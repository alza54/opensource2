#include "interfaces.hpp"

#include "../module/module.hpp"

#define FIND_INTERFACE(dst, module, version) \
  module->FindInterface(version.c_str()).Get(dst, "os2::iface::" #dst);

void os2::iface::initialise() {
  using namespace os2::sdk;
  using namespace os2::memory::strings;

  os2::module::client = CModule::Create(client_dll);
  os2::module::engine = CModule::Create(engine2_dll);
  os2::module::schemasystem = CModule::Create(schemasystem_dll);
  os2::module::inputsystem = CModule::Create(inputsystem_dll);
  os2::module::tier0 = CModule::Create(tier0_dll);
  os2::module::localize = CModule::Create(localize_dll);
  os2::module::materialsystem = CModule::Create(materialsystem2_dll);
  os2::module::scenesystem = CModule::Create(scenesystem_dll);

  FIND_INTERFACE(pGameResourceService, os2::module::engine,
                 game_resource_service_client);
  FIND_INTERFACE(pClient, os2::module::client, source2_client);
  FIND_INTERFACE(pSchemaSystem, os2::module::schemasystem, schema_system);
  FIND_INTERFACE(pInputSystem, os2::module::inputsystem, input_system_version);
  FIND_INTERFACE(pEngine, os2::module::engine, source2_engine);
  FIND_INTERFACE(pCvar, os2::module::tier0, engine_cvar);
  FIND_INTERFACE(pLocalize, os2::module::localize, localize);
  FIND_INTERFACE(pMaterialSystem, os2::module::materialsystem, material_system2);
}

void os2::iface::shutdown() {}
