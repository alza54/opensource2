#pragma once

namespace os2::sdk {
  using CUtlSymLargeId = std::int32_t*;

  class CUtlSymbolLarge {
    inline static const CUtlSymLargeId UTL_INVALID_SYMBOL_LARGE =
        reinterpret_cast<CUtlSymLargeId>(-1);

   public:
    CUtlSymbolLarge() { id_ = UTL_INVALID_SYMBOL_LARGE; }

    explicit CUtlSymbolLarge(CUtlSymLargeId id) { id_ = id; }

    CUtlSymbolLarge(CUtlSymbolLarge const& sym) { id_ = sym.id_; }

    CUtlSymbolLarge& operator=(CUtlSymbolLarge const& src) {
      id_ = src.id_;

      return *this;
    }

    bool operator==(CUtlSymbolLarge const& src) const { return id_ == src.id_; }

    bool operator==(CUtlSymLargeId const& src) const { return id_ == src; }

    bool operator!=(CUtlSymbolLarge const& src) const { return id_ != src.id_; }

    bool operator!=(CUtlSymLargeId const& src) const { return id_ != src; }

    explicit operator CUtlSymLargeId() const { return id_; }

    [[nodiscard]] const char* get_string() const {
      if (id_ == UTL_INVALID_SYMBOL_LARGE) return "";

      return string_;
    }

    [[nodiscard]] bool is_valid() const {
      return id_ != UTL_INVALID_SYMBOL_LARGE;
    }

   private:
    union {
      CUtlSymLargeId id_;
      const char* string_;
    };
  };
};  // namespace os2::sdk
