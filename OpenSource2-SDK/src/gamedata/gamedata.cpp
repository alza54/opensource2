#include "gamedata.hpp"

using namespace Game::Data;
using namespace os2::sdk;

void GameData::CacheCurrentEntities() {
  // This function runs ONCE on injection and caches all the entities if
  // you happen to inject connected on a server.
  if (!os2::iface::pEngine->IsInGame()) return;

  CGameEntitySystem* pEntitySystem = CGameEntitySystem::GetInstance();
  if (!pEntitySystem) return;

  int highestIndex = pEntitySystem->GetHighestEntityIndex();
  for (int i = 1; i <= highestIndex; ++i) {
    C_BaseEntity* pEntity = pEntitySystem->GetBaseEntity(i);

    if (!pEntity) continue;

    OnAddEntity(pEntity, pEntity->GetRefEHandle());
  }
}

void GameData::OnAddEntity(CEntityInstance* pInst, CHandle handle) {
  C_BaseEntity* pEntity = reinterpret_cast<C_BaseEntity*>(pInst);

  if (!pEntity) return;

  // Cache only networked entities.
  // https://developer.valvesoftware.com/wiki/Entity_limit#Source_2_limits
  if (handle.GetEntryIndex() >= 16384) return;

  auto it = std::find_if(g_cachedEntities.begin(), g_cachedEntities.end(),
                         [handle](const Entity_t& i) {
                           return i.handle.GetEntryIndex() ==
                                  handle.GetEntryIndex();
                         });

  if (it == g_cachedEntities.end()) {
    Entity_t cachedEntity{};
    cachedEntity.handle = handle;
    cachedEntity.type = GameData::GetEntityType(pEntity);
    if (cachedEntity.type != EEntityType::UNKNOWN)
      g_cachedEntities.emplace_back(cachedEntity);
  } else {
    it->handle = handle;
    it->type = GameData::GetEntityType(pEntity);
  }
}

void GameData::OnRemoveEntity(CEntityInstance* pInst, CHandle handle) {
  C_BaseEntity* pEntity = reinterpret_cast<C_BaseEntity*>(pInst);

  if (!pEntity) return;

  auto it = std::find_if(
      g_cachedEntities.begin(), g_cachedEntities.end(),
      [handle](const Entity_t& i) { return i.handle == handle; });
  if (it == g_cachedEntities.end()) return;

  it->shouldDrawEsp = false;
  it->type = EEntityType::UNKNOWN;
}

EEntityType GameData::GetEntityType(C_BaseEntity* pEntity) {
  if (pEntity->IsBasePlayerController())
    return EEntityType::PLAYER_CONTROLLER;
  else if (pEntity->IsBasePlayerWeapon())
    return EEntityType::BASE_WEAPON;
  else if (pEntity->IsChicken())
    return EEntityType::CHICKEN;
  else if (pEntity->IsPlantedC4())
    return EEntityType::PLANTED_C4;

  return EEntityType::UNKNOWN;
}

void GameData::SetDrawList(ImDrawList* drawList) {
  g_pBackgroundDrawList = drawList;
}
