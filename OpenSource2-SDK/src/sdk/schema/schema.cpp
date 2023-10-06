#include <unordered_map>

#include "schema.hpp"

#include "../interfaces/interfaces.hpp"

using SchemaKeyValueMap_t = std::unordered_map<uint32_t, int16_t>;
using SchemaTableMap_t = std::unordered_map<uint32_t, SchemaKeyValueMap_t>;

static bool InitSchemaFieldsForClass(const char* moduleName,
                                     SchemaTableMap_t& tableMap,
                                     const char* className, uint32_t classKey) {
  os2::sdk::CSchemaSystemTypeScope* pType =
      os2::iface::pSchemaSystem->FindTypeScopeForModule(moduleName);
  if (!pType) return false;

  os2::sdk::SchemaClassInfoData_t* pClassInfo =
      pType->FindDeclaredClass(className);
  if (!pClassInfo) {
    tableMap.emplace(classKey, SchemaKeyValueMap_t{});

    LOG(os2::schema::strings::log_schema_field_not_found.c_str(), className);
    return false;
  }

  short fieldsSize = pClassInfo->GetFieldsSize();
  os2::sdk::SchemaClassFieldData_t* pFields = pClassInfo->GetFields();

  auto& keyValueMap = tableMap[classKey];
  keyValueMap.reserve(fieldsSize);

  for (int i = 0; i < fieldsSize; ++i) {
    os2::sdk::SchemaClassFieldData_t& field = pFields[i];

#ifdef OPENSOURCE2_ENABLE_LOGGING
    LOG(os2::schema::strings::log_offset_class.c_str(), className, field.m_name,
        field.m_offset);
#endif

    keyValueMap.emplace(hash_32_fnv1a_const(field.m_name), field.m_offset);
  }

  LOG(os2::schema::strings::log_schematablemap_fields_count.c_str(), className,
      keyValueMap.size());
  return true;
}

int16_t os2::schema::GetOffset(const char* moduleName, const char* className,
                               uint32_t classKey, const char* memberName,
                               uint32_t memberKey) {
  static SchemaTableMap_t schemaTableMap;
  const auto& tableMapIt = schemaTableMap.find(classKey);
  if (tableMapIt == schemaTableMap.cend()) {
    if (InitSchemaFieldsForClass(moduleName, schemaTableMap, className,
                                 classKey))
      return GetOffset(moduleName, className, classKey, memberName, memberKey);

    return 0;
  }

  const SchemaKeyValueMap_t& tableMap = tableMapIt->second;
  if (tableMap.find(memberKey) == tableMap.cend()) {
    LOG(strings::log_offset_not_found.c_str(), memberName, className);
    return 0;
  }

  return tableMap.at(memberKey);
}
