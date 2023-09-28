#pragma once

namespace os2 {
  namespace sdk {
    enum {
      IN_ATTACK = 1 << 0,
      IN_JUMP = 1 << 1,
      IN_DUCK = 1 << 2,
      IN_FORWARD = 1 << 3,
      IN_BACK = 1 << 4,
      IN_USE = 1 << 5,
      IN_MOVELEFT = 1 << 9,
      IN_MOVERIGHT = 1 << 10,
      IN_ATTACK2 = 1 << 11,
      IN_SCORE = 1 << 16,
      IN_BULLRUSH = 1 << 22
    };

    class pb_base {
      PAD_CLASS(0x18);
    };

    class CCmdQAngle : public pb_base {
     public:
      glm::vec3 m_angles;
    };

    class CCmdVector : public pb_base {
     public:
      glm::vec4 m_vector;
    };

    class CSubTickContainer {
     public:
      int32_t m_tick_count;
      PAD_CLASS(0x4);
      void* m_input_history;
    };

    class CUserCmdBase {
     public:
      PAD_CLASS(0x40);
      CCmdQAngle* m_viewangles;
      int m_command_number;
      int m_tick_count;
      float m_forwardmove;
      float m_rightmove;
      float m_upmove;
    };

    class CUserCmd {
     public:
      PAD_CLASS(0x20);
      CSubTickContainer m_sub_tick_container;
      CUserCmdBase* m_base_cmd;
      int m_start_history_index_attack1;
      int m_start_history_index_attack2;
      int m_start_history_index_attack3;
      PAD_CLASS(0x8);
      uint64_t m_buttons;
      uint64_t m_buttons_changed;
      uint64_t m_buttons_scroll;

      PAD_CLASS(0x8);
    };

    class CCSGOInput {
     public:
      void* table;
      PAD_CLASS(0x248);
      CUserCmd m_commands[150];
      PAD_CLASS(0x1);
      bool m_in_third_person;
      PAD_CLASS(0x22);
      int32_t m_sequence_number;

      auto get_user_cmd() { return &m_commands[m_sequence_number % 150]; }
    };
  }; // namespace sdk
}; // namespace os2
