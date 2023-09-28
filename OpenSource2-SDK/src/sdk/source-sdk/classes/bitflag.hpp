#pragma once

namespace os2::sdk {
  class BitFlag {
   public:
    BitFlag() = default;

    explicit BitFlag(const std::uintptr_t flag) : flag_(flag) {}

    __forceinline void add_flag(const std::uintptr_t flag) { flag_ |= flag; }

    [[nodiscard]] __forceinline bool has_flag(const std::uintptr_t flag) const {
      return flag_ & flag;
    }

    __forceinline void remove_flag(const std::uintptr_t flag) { flag_ &= ~flag; }

    [[nodiscard]] __forceinline std::uintptr_t get_flag() const { return flag_; }

   private:
    std::uintptr_t flag_;
  };
};  // namespace os2::sdk
