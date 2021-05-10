#include "activesessions.hpp"

#include <algorithm>

#include "clientsession.hpp"
#include "cso2/user.hpp"

ActiveSessions g_Sessions;

void ActiveSessions::AddSession(ClientSessionPtr s)
{
    this->m_Sessions.push_back(s);
}

ClientSessionPtr ActiveSessions::FindSessionByUserId(std::uint32_t userId)
{
    return *std::find_if(this->m_Sessions.begin(), this->m_Sessions.end(),
                         [userId](ClientSessionPtr s) {
                             auto user = s->GetUser();
                             return user && user->GetId() == userId;
                         });
}

ClientSessionPtr ActiveSessions::FindSessionByUserName(
    std::string_view userName)
{
    return *std::find_if(this->m_Sessions.begin(), this->m_Sessions.end(),
                         [userName](ClientSessionPtr s) {
                             auto user = s->GetUser();
                             return user && user->GetUserName() == userName;
                         });
}
