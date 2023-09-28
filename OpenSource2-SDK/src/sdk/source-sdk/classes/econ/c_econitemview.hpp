#pragma once

#include "../../../schema/schema.hpp"

#include "./ceconitemdefinition.hpp"

namespace os2::sdk {
  enum class CSWeaponType {
    Knife = 0,
    Pistol,
    SubMachinegun,
    Rifle,
    Shotgun,
    SniperRifle,
    Machinegun,
    C4,
    Placeholder,
    Grenade,
    Unknown,
    StackableItem,
    Fists,
    BreachCharge,
    BumpMine,
    Tablet,
    Melee
  };

  class CWeaponCSBaseVData {
   public:
    SCHEMA_FIELD(damage, "CWeaponCSBaseVData", "m_nDamage",
                 int32_t);  // Offset: 3376
    SCHEMA_FIELD(headshotMultiplier, "CWeaponCSBaseVData",
                 "m_flHeadshotMultiplier", float);  // Offset: 3380
    SCHEMA_FIELD(armorRatio, "CWeaponCSBaseVData", "m_flArmorRatio",
                 float);  // Offset: 3384
    SCHEMA_FIELD(penetration, "CWeaponCSBaseVData", "m_flPenetration",
                 float);  // Offset: 3388
    SCHEMA_FIELD(range, "CWeaponCSBaseVData", "m_flRange",
                 float);  // Offset: 3392
    SCHEMA_FIELD(rangeModifier, "CWeaponCSBaseVData", "m_flRangeModifier",
                 float);  // Offset: 3396
    SCHEMA_FIELD(bulletsInClip, "CBasePlayerWeaponVData", "m_nNumBullets",
                 int32_t);  // Offset: 268
    SCHEMA_FIELD(type, "CWeaponCSBaseVData", "m_WeaponType",
                 CSWeaponType);  // Offset: 576
  };

  class CEconItem;

  class C_EconItemView {
   public:
    CEconItem* GetSOCData();
    CWeaponCSBaseVData* GetWeaponInfo();

    auto GetCustomPaintKitIndex() { return CALL_VIRTUAL(int, 2, this); }
    auto GetStaticData() { return CALL_VIRTUAL(CEconItemDefinition*, 13, this); }

    SCHEMA_FIELD(m_iItemDefinitionIndex, "C_EconItemView",
                 "m_iItemDefinitionIndex", uint16_t);
    SCHEMA_FIELD(m_iItemID, "C_EconItemView", "m_iItemID", uint64_t);
    SCHEMA_FIELD(m_iItemIDLow, "C_EconItemView", "m_iItemIDLow", uint32_t);
    SCHEMA_FIELD(m_iItemIDHigh, "C_EconItemView", "m_iItemIDHigh", uint32_t);
    SCHEMA_FIELD(m_iAccountID, "C_EconItemView", "m_iAccountID", uint32_t);
  };
};  // namespace os2::sdk
