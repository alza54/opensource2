#include "../../../memory/memory.hpp"

#include "ceconitemschema.hpp"

bool os2::sdk::CPaintKit::UsesLegacyModel() {
  if (!this || !os2::fn::IsPaintKitUsingLegacyModel) return false;
  return os2::fn::IsPaintKitUsingLegacyModel(this->sName);
}
