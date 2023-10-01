#include "c_trace.hpp"

#include <stdint.h>

#include "../../../memory/memory.hpp"

os2::sdk::C_TraceFilter::C_TraceFilter(std::uint32_t Mask,
                                       C_CSPlayerPawn* Skip1,
                                       C_CSPlayerPawn* Skip2, int Layer) {
  TraceMask = Mask;
  V1[0] = V1[1] = 0;
  V2 = 7;
  V3 = Layer;
  V4 = 0x49;
  V5 = 0;

  SkipHandles[0] = os2::fn::GetEntityHandle(Skip1);
  SkipHandles[1] = Skip1->GetOwnerHandle();
  SkipHandles[2] = 0;
  SkipHandles[3] = 0;

  Collisions[0] = Skip1->GetCollisionMask();
  Collisions[1] = 0;
}

os2::sdk::C_SurfaceData* os2::sdk::C_GameTrace::GetSurfaceData() {
  if (!os2::fn::GetSurfaceData) return nullptr;
  return reinterpret_cast<C_SurfaceData*>(os2::fn::GetSurfaceData(Surface));
}
