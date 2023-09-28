#include "../../../../memory/memory.hpp"

#include "cbonesystem.hpp"

using namespace os2::sdk;

BitFlag CModel::GetBoneFlags(const std::int32_t index) {
  using Fn = std::int32_t(__fastcall*)(CModel*, std::int32_t);

  static auto fn = os2::fn::GetBoneFlags;

  if (fn == nullptr) return {};

  return BitFlag(fn(this, index));
}

std::int32_t CModel::GetBoneParent(const std::int32_t index) {
  using Fn = std::int32_t(__fastcall*)(CModel*, std::int32_t);

  static auto fn = os2::fn::GetBoneParent;

  if (fn == nullptr) return {};

  return fn(this, index);
}
