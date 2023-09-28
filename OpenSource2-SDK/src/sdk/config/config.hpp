#pragma once

#include "../../features/features.hpp"

inline std::optional<Features> gameFeatures;

struct RecoilControlConfig {
  bool Enabled = false;
  bool DrawGUI = false;
  bool DrawPreview = false;
  float PerfectionX = 100.f;
  float PerfectionY = 100.f;
};

struct BunnyhopConfig {
  bool Enabled = false;
  bool DrawGUI = false;
  bool DrawWatermark = false;
  int Perfection = 100;
};
