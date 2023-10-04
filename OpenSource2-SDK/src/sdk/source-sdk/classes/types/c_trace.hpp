#pragma once

#include "../../source-sdk.hpp"

#include "../../../math/classes/vector.hpp"

#include "../entity/c_csplayerpawn.hpp"

#define COMBINE(x, y) x##y
#define COMBINE2(x, y) COMBINE(x, y)

#define PAD_CLASS_DEBUG(sz) int COMBINE2(pad_, __COUNTER__)[sz];

namespace os2::sdk {
  struct C_Ray {
    Vector Start;
    Vector End;
    Vector Mins;
    Vector Maxs;
    PAD_CLASS(0x4);
    std::uint8_t UnkType;
  };

  struct C_SurfaceData {
    PAD_CLASS(0x8);
    float PenetrationModifier;
    float DamageModifier;
    PAD_CLASS(0x4);
    int Material;
  };

  struct C_TraceHitboxData {
    PAD_CLASS_DEBUG(0x38 / 0x4);
    int Hitgroup;
    PAD_CLASS_DEBUG(0x2);
    PAD_CLASS(0x1);
    int HitboxId;
    PAD_CLASS_DEBUG(0x24 / 0x4);
  };

  class C_GameTrace {
   public:
    C_SurfaceData* GetSurfaceData();

    int GetHitboxId() {
      if (HitboxData) return HitboxData->HitboxId;
      return 0;
    }

    int GetHitgroup() {
      if (HitboxData) return HitboxData->Hitgroup;
      return 0;
    }

    void* Surface;
    C_CSPlayerPawn* HitEntity;
    C_TraceHitboxData* HitboxData;
    PAD_CLASS(0x10);
    std::uint32_t Contents;
    PAD_CLASS(0x58);
    Vector EndPos;
    PAD_CLASS(0x1C);
    float Fraction;
    PAD_CLASS(0x10);
  };

  class C_TraceFilter {
   public:
    std::uint64_t TraceMask;
    std::uint64_t V1[2];
    std::uint32_t SkipHandles[4];
    std::uint16_t Collisions[2];
    std::uint16_t V2;
    std::uint8_t V3;
    std::uint8_t V4;
    std::uint8_t V5;

    virtual ~C_TraceFilter() {}
    virtual bool function() { return true; }

    C_TraceFilter(std::uint32_t Mask, C_CSPlayerPawn* Skip1,
                  C_CSPlayerPawn* Skip2, int Layer);
  };
};  // namespace os2::sdk

static_assert(sizeof(os2::sdk::C_TraceHitboxData) == 0x70);
