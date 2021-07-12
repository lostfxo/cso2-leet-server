#ifndef __CSO2_CLIENTMESSAGES_H_
#define __CSO2_CLIENTMESSAGES_H_

#include <string_view>

#include "clientsession.hpp"

namespace cso2
{
inline void SendUserDialogBox(std::string_view msg, ClientSession* session) {}
}  // namespace cso2

#endif  // __CSO2_CLIENTMESSAGES_H_
