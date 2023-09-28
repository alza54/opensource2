#include "ceconitem.hpp"

#include "../../../memory/memory.hpp"
#include "../../../interfaces/interfaces.hpp"

using namespace os2::sdk;

void CEconItem::SetDynamicAttributeValue(int index, void* value) {
  CEconItemSchema* pItemSchema =
      os2::iface::pClient->GetEconItemSystem()->GetEconItemSchema();
  if (!pItemSchema) return;

  void* pAttributeDefinitionInterface =
      pItemSchema->GetAttributeDefinitionInterface(index);
  if (!pAttributeDefinitionInterface) return;

  if (!os2::fn::SetDynamicAttributeValueUint) return;
  os2::fn::SetDynamicAttributeValueUint(
      this, pAttributeDefinitionInterface,
                                         value);
}

void CEconItem::SetDynamicAttributeValueString(int index, const char* value) {
  // CS2FIXME: Function got inlined and cannot be sigscanned.
}

CEconItem* CEconItem::CreateInstance() {
  if (!os2::fn::CreateSharedObjectSubclassEconItem) return nullptr;
  return os2::fn::CreateSharedObjectSubclassEconItem();
}
