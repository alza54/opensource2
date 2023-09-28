#include "c_csplayerpawn.hpp"

#include "../../../interfaces/interfaces.hpp"
#include "../../../memory/memory.hpp"

bool os2::sdk::C_CSPlayerPawn::IsEnemyWithTeam(int team) {
  static ConVar* mp_teammates_are_enemies =
      os2::iface::pCvar->FindVarByName("mp_teammates_are_enemies");

  os2::sdk::CCSPlayerController* pPlayerController =
      m_hController().Get<CCSPlayerController>();

  if (pPlayerController && pPlayerController->m_bIsLocalPlayerController())
    return false;

  return mp_teammates_are_enemies->GetValue<bool>() ? true
                                                    : m_iTeamNum() != team;
}

bool os2::sdk::C_CSPlayerPawn::IsDormant() {
  os2::sdk::CGameSceneNode* game_scene_node = m_pGameSceneNode();

  if (game_scene_node == nullptr) return false;

  return game_scene_node->m_bDormant();
}

std::uint16_t os2::sdk::C_CSPlayerPawn::GetCollisionMask() {
  if (this && m_pCollision())
    return m_pCollision()->CollisionMask();  // Collision + 0x38
  return 0;
}

std::uint32_t os2::sdk::C_CSPlayerPawn::GetOwnerHandle() {
  std::uint32_t Result = -1;
  if (this && m_pCollision() && !(m_pCollision()->m_solidFlags() & 4)) {
    Result = this->m_hOwnerEntity().m_Index;
  }
  return Result;
}
