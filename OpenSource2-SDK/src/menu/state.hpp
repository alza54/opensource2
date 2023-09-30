#pragma once

#include <iostream>

#define BRAND_COLOR 0.79607844f, 0.6313726f, 0.20784314f

namespace os2::menu {
  inline bool bIsOpen = false;

  struct Windows {
    bool AimAssist;
    bool AutoShoot;
    bool RecoilControl;
    bool SpreadControl;
    bool AimPrediction;
    bool NeuralAimStatus;
    bool AimStatistics;

    bool BunnyHop;
    bool ESP;
  };
};  // namespace os2::menu
