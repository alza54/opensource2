#pragma once

namespace os2::sdk {
  class CGlobalVarsBase {
   public:
    float real_time;
    int frame_count;
    float unknown1;
    float unknown2;
    int max_clients;
    float interval_per_tick;
    int unknown3;
    int unknown4;
    void* m_unkfunc;
    float unknown5;
    float current_time;
    float current_time2;
    PAD_CLASS(0xC)
    int tick_count;
    float interval_per_tick2;
    void* current_netchan;
    PAD_CLASS(0x130)
    char* current_map;
    char* current_map_name;
  };
};  // namespace os2::sdk

static_assert(sizeof(os2::sdk::CGlobalVarsBase) == 0x0190);
