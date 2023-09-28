#include "cgcclient.hpp"

#include "../../../memory/memory.hpp"

os2::sdk::CGCClientSharedObjectCache* os2::sdk::CGCClient::FindSOCache(
    SOID_t ID, bool bCreateIfMissing) {
  if (!os2::fn::FindSOCache) return nullptr;
  return os2::fn::FindSOCache(this, ID, bCreateIfMissing);
}
