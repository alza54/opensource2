#pragma once

#include "../fnv1a/hash_fnv1a_constexpr.hpp"

#include <string>
#include <memory>

#include "../utilities/static_string.hpp"

#define SCHEMA_FIELD_OFFSET(varName, datatable, propName, extra_offset, type) \
  std::add_lvalue_reference_t<type> varName() {                               \
    static constexpr auto datatable_hash = hash_32_fnv1a_const(datatable);    \
    static constexpr auto prop_hash = hash_32_fnv1a_const(propName);          \
                                                                              \
    static const auto m_offset =                                              \
        os2::schema::GetOffset(datatable, datatable_hash, propName, prop_hash);    \
                                                                              \
    return *reinterpret_cast<std::add_pointer_t<type>>(                       \
        (uintptr_t)(this) + m_offset + extra_offset);                         \
  }

#define SCHEMA_FIELD(varName, datatable, propName, type) \
  SCHEMA_FIELD_OFFSET(varName, datatable, propName, 0, type)

#define PSCHEMA_FIELD_OFFSET(varName, datatable, propName, extra_offset, type) \
  auto varName() {                                                             \
    static constexpr auto datatable_hash = hash_32_fnv1a_const(datatable);     \
    static constexpr auto prop_hash = hash_32_fnv1a_const(propName);           \
                                                                               \
    static const auto m_offset =                                               \
        os2::schema::GetOffset(datatable, datatable_hash, propName, prop_hash);     \
                                                                               \
    return reinterpret_cast<std::add_pointer_t<type>>(                         \
        (uintptr_t)(this) + m_offset + extra_offset);                          \
  }

#define PSCHEMA_FIELD(varName, datatable, propName, type) \
  PSCHEMA_FIELD_OFFSET(varName, datatable, propName, 0, type)

namespace os2::schema {
  namespace strings {
    inline const std::string log_schema_field_not_found = os2_string(
        "os2::schema::InitSchemaFieldsForClass(): '%s' was not found!\n");
    inline const std::string log_offset_not_found =
        os2_string("os2::schema::GetOffset(): '%s' was not found in '%s'!\n");
    inline const std::string log_offset_class =
        os2_string("%s::%s found at -> 0x%X\n");
    inline const std::string log_schematablemap_fields_count =
        os2_string("schemaTableMap[%s] has %llu fields.\n");
  };  // namespace strings

  int16_t GetOffset(const char* className, uint32_t classKey,
                    const char* memberName, uint32_t memberKey);
};  // namespace os2::schema
