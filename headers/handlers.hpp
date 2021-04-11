#ifndef __HANDLERS_H_
#define __HANDLERS_H_

#include <boost/asio/awaitable.hpp>

#include "clientsession_shared.hpp"

using boost::asio::awaitable;

class PacketView;

void OnAchievementPacketRequest(PacketView& packet, ClientSessionPtr session);
void OnChannelListPacketRequest(PacketView& packet, ClientSessionPtr session);
void OnChatPacketRequest(PacketView& packet, ClientSessionPtr session);
awaitable<void> OnFavoritePacketRequest(PacketView& packet,
                                        ClientSessionPtr session);
awaitable<void> OnHostPacketRequest(PacketView& packet,
                                    ClientSessionPtr session);
awaitable<void> OnLoginPacketRequest(PacketView& packet,
                                     ClientSessionPtr session);
awaitable<void> OnOptionPacketRequest(PacketView& packet,
                                      ClientSessionPtr session);
void OnRoomPacketRequest(PacketView& packet, ClientSessionPtr session);
void OnRoomListPacketRequest(PacketView& packet, ClientSessionPtr session);
awaitable<void> OnUserProfilePacketRequest(PacketView& packet,
                                           ClientSessionPtr session);
void OnVersionPacketRequest(PacketView& packet, ClientSessionPtr session);

#endif  // __HANDLERS_H_
