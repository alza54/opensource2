#pragma once

namespace os2::sdk {
  class IGameEvent {
   public:
    const char* get_name() { return CALL_VIRTUAL(const char*, 1, this); }

    bool is_empty(const char* key_name) {
      return CALL_VIRTUAL(bool, 5, this, key_name);
    }

    bool get_bool(const char* key_name, const bool default_value = false) {
      return CALL_VIRTUAL(bool, 6, this, key_name, default_value);
    }

    std::int32_t get_int(const char* key_name,
                         const std::int32_t default_value = 0) {
      return CALL_VIRTUAL(std::int32_t, 7, this, key_name, default_value);
    }

    std::uint64_t get_uint64(const char* key_name,
                             const std::uint64_t default_value = 0) {
      return CALL_VIRTUAL(std::uint64_t, 8, this, key_name, default_value);
    }

    float get_float(const char* key_name, const float default_value = 0.f) {
      return CALL_VIRTUAL(float, 9, this, key_name, default_value);
    }

    const char* get_string(const char* key_name, const char* default_value = "") {
      return CALL_VIRTUAL(const char*, 10, this, key_name, default_value);
    }

    const wchar_t* get_wstring(const char* key_name,
                               const wchar_t* default_value = L"") {
      return CALL_VIRTUAL(const wchar_t*, 11, this, key_name, default_value);
    }
  };

  class IGameEventListener2 {
   public:
    virtual ~IGameEventListener2() = default;
    virtual void fire_game_event(IGameEvent* event) = 0;
    virtual int get_event_debug_id() = 0;
  };
};  // namespace os2::sdk
