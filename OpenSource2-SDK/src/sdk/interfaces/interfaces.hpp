#pragma once

#include "../source-sdk/interfaces/cgameresourceserviceclient.hpp"
#include "../source-sdk/classes/cgameentitysystem.hpp"
#include "../source-sdk/interfaces/csource2client.hpp"
#include "../source-sdk/interfaces/cschemasystem.hpp"
#include "../source-sdk/interfaces/cengineclient.hpp"
#include "../source-sdk/interfaces/cinputsystem.hpp"
#include "../source-sdk/classes/cgameevent.hpp"
#include "../source-sdk/interfaces/clocalize.hpp"
#include "../source-sdk/interfaces/ccvar.hpp"
#include "../source-sdk/classes/cmaterial2.hpp"
#include "../source-sdk/interfaces/cmaterialsystem2.hpp"

namespace os2 {
  namespace module {
    inline std::shared_ptr<sdk::CModule> client;
    inline std::shared_ptr<sdk::CModule> engine;
    inline std::shared_ptr<sdk::CModule> schemasystem;
    inline std::shared_ptr<sdk::CModule> inputsystem;
    inline std::shared_ptr<sdk::CModule> tier0;
    inline std::shared_ptr<sdk::CModule> localize;
    inline std::shared_ptr<sdk::CModule> materialsystem;
  };  // namespace module

  namespace iface {
    void initialise();
    void shutdown();

    inline os2::sdk::CGameResourceService* pGameResourceService;
    inline os2::sdk::CSource2Client* pClient;
    inline os2::sdk::CSchemaSystem* pSchemaSystem;
    inline os2::sdk::CInputSystem* pInputSystem;
    inline os2::sdk::CEngineClient* pEngine;
    inline os2::sdk::CCvar* pCvar;
    inline os2::sdk::CLocalize* pLocalize;
    inline os2::sdk::CMaterialSystem2* pMaterialSystem;
  }; // namespace iface
};  // namespace os2
