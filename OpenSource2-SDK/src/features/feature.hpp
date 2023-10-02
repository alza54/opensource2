#pragma once

#include <iostream>
#include <memory>
#include <nlohmann/single_include/nlohmann/json.hpp>
#include <nlohmann/single_include/nlohmann/json_fwd.hpp>
#include <type_traits>

#include "../sdk/interfaces/interfaces.hpp"
#include "../sdk/memory/memory.hpp"

class Feature {
 private:
  const char* name_;

 public:
  Feature(const char* className) : name_(className) {}

  const char* Name() const { return name_; }

  virtual ~Feature() = default;

  virtual void OnCreateMove(os2::sdk::CCSGOInput* pCsgoInput,
                            os2::sdk::CUserCmd*,
                            glm::vec3& view_angles) noexcept = 0;
  virtual void OnFrameStageNotify(os2::sdk::CSource2Client*,
                                  const std::int32_t) noexcept {}
  virtual void OnRender() noexcept = 0;

  virtual nlohmann::json ToJson() const = 0;
  virtual void FromJson(const nlohmann::json& j) = 0;
};

#define CONFIG_GETTER(TYPE, NAME) \
  TYPE&##NAME() { return config_.NAME; }
