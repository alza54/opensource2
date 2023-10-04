#pragma once

#include <mutex>
#include <vector>
#include <math.h>
#include <optional>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <implot/implot.h>

#include "../sdk/interfaces/interfaces.hpp"

#include "../sdk/source-sdk/classes/entity/c_pointcamera.hpp"

namespace Game::Data {
  struct ScrollingBuffer {
    int MaxSize;
    int Offset;
    ImVector<ImVec2> Data;
    ScrollingBuffer(int max_size = 2000) {
      MaxSize = max_size;
      Offset = 0;
      Data.reserve(MaxSize);
    }
    void AddPoint(float x, float y) {
      if (Data.size() < MaxSize)
        Data.push_back(ImVec2(x, y));
      else {
        Data[Offset] = ImVec2(x, y);
        Offset = (Offset + 1) % MaxSize;
      }
    }
    void Erase() {
      if (Data.size() > 0) {
        Data.shrink(0);
        Offset = 0;
      }
    }
  };

  // utility structure for real-time plot
  struct RollingBuffer {
    float Span;
    ImVector<ImVec2> Data;
    RollingBuffer() {
      Span = 10.0f;
      Data.reserve(2000);
    }
    void AddPoint(float x, float y) {
      float xmod = fmodf(x, Span);
      if (!Data.empty() && xmod < Data.back().x) Data.shrink(0);
      Data.push_back(ImVec2(xmod, y));
    }
  };

  enum EEntityType {
    UNKNOWN = -1,
    PLAYER_CONTROLLER,
    BASE_WEAPON,
    CHICKEN,
    PLANTED_C4,
  };

  struct GamePlayer_t {};

  struct Entity_t {
    os2::sdk::CHandle handle;
    EEntityType type;

    os2::sdk::BBox_t boundingBox;
    bool shouldDrawEsp;
  };

  class GameData {
   public:
    GameData() : g_pBackgroundDrawList(nullptr) {}

    ImDrawList* DrawList() { return g_pBackgroundDrawList; }
    void SetDrawList(ImDrawList* drawList = ImGui::GetBackgroundDrawList());

    std::vector<Entity_t>& GetEntities() { return g_cachedEntities; }
    std::mutex& GetEntitiesMutex() { return g_cachedEntitiesMutex; }

    void CacheCurrentEntities();
    void OnAddEntity(os2::sdk::CEntityInstance* pInst,
                     os2::sdk::CHandle handle);
    void OnRemoveEntity(os2::sdk::CEntityInstance* pInst,
                        os2::sdk::CHandle handle);

    static EEntityType GetEntityType(os2::sdk::C_BaseEntity* pEntity);

   private:
    ImDrawList* g_pBackgroundDrawList;
    std::vector<Entity_t> g_cachedEntities;
    std::mutex g_cachedEntitiesMutex;
  };

  inline std::optional<Game::Data::GameData> g_pGameData;
};  // namespace Game::Data
