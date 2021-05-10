#ifndef __ACTIVESESSIONS_H_
#define __ACTIVESESSIONS_H_

#include <string_view>
#include <vector>

#include "clientsession_shared.hpp"

class ActiveSessions
{
public:
    ActiveSessions();

    void AddSession(ClientSessionPtr s);
    void RemoveSession(ClientSessionPtr targetSession);

    [[nodiscard]] ClientSessionPtr FindSessionByUserId(std::uint32_t userId);
    [[nodiscard]] ClientSessionPtr FindSessionByUserName(
        std::string_view userName);

private:
    std::vector<ClientSessionPtr> m_Sessions;
};

extern ActiveSessions g_Sessions;

#endif  // __ACTIVESESSIONS_H_
