#pragma once

#include "c_baseentity.hpp"

namespace os2::sdk {
  class C_PointCamera : C_BaseEntity {
   public:
    SCHEMA_FIELD(m_FOV, "C_PointCamera", "m_FOV",
                 float);  // Offset: 1344
    SCHEMA_FIELD(m_Resolution, "C_PointCamera", "m_Resolution",
                 float);  // Offset: 1348
    SCHEMA_FIELD(m_bFogEnable, "C_PointCamera", "m_bFogEnable",
                 bool);  // Offset: 1352
    SCHEMA_FIELD(m_flFogStart, "C_PointCamera", "m_flFogStart",
                 float);  // Offset: 1360
    SCHEMA_FIELD(m_flFogEnd, "C_PointCamera", "m_flFogEnd",
                 float);  // Offset: 1364
    SCHEMA_FIELD(m_flFogMaxDensity, "C_PointCamera", "m_flFogMaxDensity",
                 float);                                          // Offset: 1368
    SCHEMA_FIELD(m_bActive, "C_PointCamera", "m_bActive", bool);  // Offset: 1372
    SCHEMA_FIELD(m_bUseScreenAspectRatio, "C_PointCamera",
                 "m_bUseScreenAspectRatio", bool);  // Offset: 1373
    SCHEMA_FIELD(m_flAspectRatio, "C_PointCamera", "m_flAspectRatio",
                 float);                                        // Offset: 1376
    SCHEMA_FIELD(m_bNoSky, "C_PointCamera", "m_bNoSky", bool);  // Offset: 1380
    SCHEMA_FIELD(m_fBrightness, "C_PointCamera", "m_fBrightness",
                 float);                                           // Offset: 1384
    SCHEMA_FIELD(m_flZFar, "C_PointCamera", "m_flZFar", float);    // Offset: 1388
    SCHEMA_FIELD(m_flZNear, "C_PointCamera", "m_flZNear", float);  // Offset: 1392
    SCHEMA_FIELD(m_bCanHLTVUse, "C_PointCamera", "m_bCanHLTVUse",
                 bool);  // Offset: 1396
    SCHEMA_FIELD(m_bDofEnabled, "C_PointCamera", "m_bDofEnabled",
                 bool);  // Offset: 1397
    SCHEMA_FIELD(m_flDofNearBlurry, "C_PointCamera", "m_flDofNearBlurry",
                 float);  // Offset: 1400
    SCHEMA_FIELD(m_flDofNearCrisp, "C_PointCamera", "m_flDofNearCrisp",
                 float);  // Offset: 1404
    SCHEMA_FIELD(m_flDofFarCrisp, "C_PointCamera", "m_flDofFarCrisp",
                 float);  // Offset: 1408
    SCHEMA_FIELD(m_flDofFarBlurry, "C_PointCamera", "m_flDofFarBlurry",
                 float);  // Offset: 1412
    SCHEMA_FIELD(m_flDofTiltToGround, "C_PointCamera", "m_flDofTiltToGround",
                 float);  // Offset: 1416
    SCHEMA_FIELD(m_TargetFOV, "C_PointCamera", "m_TargetFOV",
                 float);  // Offset: 1420
  };
};  // namespace os2::sdk  // namespace os2::sdk
