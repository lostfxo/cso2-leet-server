#include "activesessions.hpp"

#include <algorithm>

#include "clientsession.hpp"
#include "cso2/user.hpp"

namespace ranges = std::ranges;

ActiveSessions g_Sessions;

void ActiveSessions::AddSession(ClientSessionPtr s)
{
    this->m_Sessions.push_back(s);
}

ClientSessionPtr ActiveSessions::FindSessionByUserId(std::uint32_t userId)
{
    return *ranges::find_if(this->m_Sessions, [userId](ClientSessionPtr s) {
        auto user = s->GetUser();
        return user && user->GetId() == userId;
    });
}

ClientSessionPtr ActiveSessions::FindSessionByUserName(
    std::string_view userName)
{
    return *ranges::find_if(this->m_Sessions, [userName](ClientSessionPtr s) {
        auto user = s->GetUser();
        return user && user->GetUserName() == userName;
    });
}
