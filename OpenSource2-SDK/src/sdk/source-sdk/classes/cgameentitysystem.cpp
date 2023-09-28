#include "cgameentitysystem.hpp"

#include "../../interfaces/interfaces.hpp"

using namespace os2::sdk;

CGameEntitySystem* CGameEntitySystem::GetInstance() {
  if (!os2::iface::pGameResourceService) return nullptr;

  return os2::iface::pGameResourceService->GetGameEntitySystem();
}

CCSPlayerController* CGameEntitySystem::GetLocalPlayerController() {
  if (!os2::fn::GetLocalPlayerController) return nullptr;

  return os2::fn::GetLocalPlayerController(-1);
}
