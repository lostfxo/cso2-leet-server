#include "activesessions.hpp"

#include "clientsession.hpp"
#include "cso2/user.hpp"

ActiveSessions g_Sessions;

ActiveSessions::ActiveSessions()
{
    this->m_Sessions.reserve(256);
}

void ActiveSessions::AddSession(ClientSessionPtr s)
{
    this->m_Sessions.push_back(s);
}

void ActiveSessions::RemoveSession(ClientSessionPtr targetSession)
{
    this->m_Sessions.erase(std::remove_if(
        this->m_Sessions.begin(), this->m_Sessions.end(),
        [targetSession](ClientSessionPtr s) { return s == targetSession; }));
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
