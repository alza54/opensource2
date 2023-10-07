#pragma once

#include <glm/vec3.hpp>

class C_SoundData_Origin {
 public:
  glm::vec3 origin;
};

class C_SoundData {
 public:
  char pad_0000[8];
  char* sound_name;
  char pad_0010[16];
  class C_SoundData_Origin* get_origin;
};
