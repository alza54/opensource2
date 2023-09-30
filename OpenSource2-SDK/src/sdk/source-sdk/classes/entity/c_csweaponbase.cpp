#include "c_csweaponbase.hpp"

#include "../../../memory/memory.hpp"

void os2::sdk::C_CSWeaponBase::AddStattrakEntity() {
  if (!os2::fn::AddStattrakEntity) return;
  return os2::fn::AddStattrakEntity(m_hStattrakAttachment());
}

void os2::sdk::C_CSWeaponBase::AddNametagEntity() {
  if (!os2::fn::AddNametagEntity) return;
  return os2::fn::AddNametagEntity(m_hNametagAttachment());
}
