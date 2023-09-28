#pragma once

#include <mutex>
#include <vector>
#include <math.h>
#include <optional>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <implot/implot.h>

#include "../sdk/interfaces/interfaces.hpp"

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

  /* void Plot() {
    ImGui::TextWrapped("Move your mouse to change the data!");
    ImGui::TextWrapped(
        "This example assumes 60 FPS. Higher FPS requires larger buffer size.");
    static ScrollingBuffer sdata1, sdata2;
    static RollingBuffer rdata1, rdata2;
    ImVec2 mouse = ImGui::GetMousePos();
    static float t = 0;
    t += ImGui::GetIO().DeltaTime;
    sdata1.AddPoint(t, mouse.x * 0.0005f);
    rdata1.AddPoint(t, mouse.x * 0.0005f);
    sdata2.AddPoint(t, mouse.y * 0.0005f);
    rdata2.AddPoint(t, mouse.y * 0.0005f);

    static float history = 10.0f;
    ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");
    rdata1.Span = history;
    rdata2.Span = history;

    static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;

    if (ImPlot::BeginPlot("##Scrolling", ImVec2(-1, 150))) {
      ImPlot::SetupAxes(nullptr, nullptr, flags, flags);
      ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
      ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1);
      ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
      ImPlot::PlotShaded("Mouse X", &sdata1.Data[0].x, &sdata1.Data[0].y,
                         sdata1.Data.size(), -INFINITY, 0, sdata1.Offset,
                         2 * sizeof(float));
      ImPlot::PlotLine("Mouse Y", &sdata2.Data[0].x, &sdata2.Data[0].y,
                       sdata2.Data.size(), 0, sdata2.Offset, 2 * sizeof(float));
      ImPlot::EndPlot();
    }
    if (ImPlot::BeginPlot("##Rolling", ImVec2(-1, 150))) {
      ImPlot::SetupAxes(nullptr, nullptr, flags, flags);
      ImPlot::SetupAxisLimits(ImAxis_X1, 0, history, ImGuiCond_Always);
      ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1);
      ImPlot::PlotLine("Mouse X", &rdata1.Data[0].x, &rdata1.Data[0].y,
                       rdata1.Data.size(), 0, 0, 2 * sizeof(float));
      ImPlot::PlotLine("Mouse Y", &rdata2.Data[0].x, &rdata2.Data[0].y,
                       rdata2.Data.size(), 0, 0, 2 * sizeof(float));
      ImPlot::EndPlot();
    }
  }*/

  enum EEntityType {
    UNKNOWN = -1,
    PLAYER_CONTROLLER,
    BASE_WEAPON,
    CHICKEN,
    PLANTED_C4
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
