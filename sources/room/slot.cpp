#include "room/slot.hpp"

#include "clientsession.hpp"
#include "cso2/user.hpp"

Slot::Slot(ClientSessionPtr owner, cso2::TeamNum teamNum)
    : m_Owner(owner), m_Team(teamNum), m_Status(cso2::SlotStatus::NotReady),
      m_CachedUserId(0), m_Kills(0), m_Deaths(0), m_Assists(0), m_Headshots(0)
{
    this->m_CachedUserId = this->m_Owner->GetUser()->GetId();
}
