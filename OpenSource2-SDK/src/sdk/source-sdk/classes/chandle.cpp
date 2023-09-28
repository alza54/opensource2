#include "chandle.hpp"

#include "../../interfaces/interfaces.hpp"

os2::sdk::C_BaseEntity* os2::sdk::CHandle::GetBaseEntity() const {
  os2::sdk::CGameEntitySystem* pEntitySystem = os2::sdk::CGameEntitySystem::GetInstance();
  if (!pEntitySystem) return nullptr;

  return pEntitySystem->GetBaseEntity(GetEntryIndex());
}
