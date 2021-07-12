#ifndef __CHANNEL_SHARED_H_
#define __CHANNEL_SHARED_H_

#include <vector>

#include "clientsession_shared.hpp"

class Channel;
class ChannelGroup;
class ClientSession;

using ChannelPtr = std::shared_ptr<Channel>;
using ChannelsArray = std::vector<ChannelPtr>;
using ChannelGroupPtr = std::shared_ptr<ChannelGroup>;
using ChannelGroupsArray = std::vector<ChannelGroupPtr>;

void SendChannelRoomList(ClientSessionPtr session, ChannelPtr channel);

#endif  // __CHANNEL_SHARED_H_
