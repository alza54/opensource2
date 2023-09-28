#include "c_basemodelentity.hpp"

#include "../../../memory/memory.hpp"

void os2::sdk::C_BaseModelEntity::SetModel(const char* name) {
  if (!os2::fn::SetModel) return;
  os2::fn::SetModel(this, name);
}
