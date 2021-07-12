#ifndef __SYSTEM_DIALOGBOX_H_
#define __SYSTEM_DIALOGBOX_H_

#include <string_view>

#include "clientsession_shared.hpp"

class ClientSession;

void SendDialogBox(ClientSessionPtr session, std::string_view msg);
void SendSystemMessage(ClientSessionPtr session, std::string_view msg);

#endif  // __SYSTEM_DIALOGBOX_H_
