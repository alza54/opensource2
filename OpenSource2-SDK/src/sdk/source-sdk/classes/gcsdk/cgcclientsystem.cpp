#include "cgcclientsystem.hpp"

#include "../../../memory/memory.hpp"

os2::sdk::CGCClientSystem* os2::sdk::CGCClientSystem::GetInstance() {
  if (!os2::fn::GetClientSystem) return nullptr;
  return os2::fn::GetClientSystem();
}
