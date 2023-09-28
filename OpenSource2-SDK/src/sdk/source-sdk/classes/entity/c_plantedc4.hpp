#pragma once

#include "c_basemodelentity.hpp"

namespace os2::sdk {
  class C_PlantedC4 : public C_BaseModelEntity {
   public:
    SCHEMA_FIELD(m_bBombTicking, "C_PlantedC4", "m_bBombTicking",
                 bool);  // Offset: 3696
    SCHEMA_FIELD(m_nBombSite, "C_PlantedC4", "m_nBombSite",
                 int32_t);  // Offset: 3700
    SCHEMA_FIELD(m_flNextBeep, "C_PlantedC4", "m_flNextBeep",
                 float);                                           // Offset: 3740
    SCHEMA_FIELD(m_flC4Blow, "C_PlantedC4", "m_flC4Blow", float);  // Offset: 3744
    SCHEMA_FIELD(m_flTimerLength, "C_PlantedC4", "m_flTimerLength",
                 float);  // Offset: 3752
    SCHEMA_FIELD(m_flDefuseLength, "C_PlantedC4", "m_flDefuseLength",
                 float);  // Offset: 3776
    SCHEMA_FIELD(m_flDefuseCountDown, "C_PlantedC4", "m_flDefuseCountDown",
                 float);  // Offset: 3780
    SCHEMA_FIELD(m_hBombDefuser, "C_PlantedC4", "m_hBombDefuser",
                 CHandle);  // Offset: 3788
    SCHEMA_FIELD(m_pBombDefuser, "C_PlantedC4", "m_pBombDefuser",
                 CHandle);  // Offset: 3808
  };
};  // namespace os2::sdk
