#ifndef __ACTIVESESSIONS_H_
#define __ACTIVESESSIONS_H_

#include <string_view>
#include <vector>

#include "clientsession_shared.hpp"

class ActiveSessions
{
public:
    ActiveSessions() { this->m_Sessions.reserve(256); }

    void AddSession(ClientSessionPtr s);
    [[nodiscard]] ClientSessionPtr FindSessionByUserId(std::uint32_t userId);
    [[nodiscard]] ClientSessionPtr FindSessionByUserName(
        std::string_view userName);

private:
    std::vector<ClientSessionPtr> m_Sessions;
};

extern ActiveSessions g_Sessions;

#endif  // __ACTIVESESSIONS_H_
