#include "c_econitemview.hpp"

#include "../../../memory/memory.hpp"

os2::sdk::CEconItem* os2::sdk::C_EconItemView::GetSOCData() {
  /**
  * @TODO
  * ... later =]
  *
  * CCSPlayerInventory* pInventory = CCSPlayerInventory::GetInstance();
    if (!pInventory) return nullptr;

    return pInventory->GetSOCDataForItem(m_iItemID());
  *
  */
  return nullptr;
}

os2::sdk::CWeaponCSBaseVData* os2::sdk::C_EconItemView::GetWeaponInfo() {
  if (!os2::fn::GetWeaponData) return nullptr;
  return os2::fn::GetWeaponData(this);
}
