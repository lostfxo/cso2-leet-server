#include "handlers.hpp"

#include "clientsession.hpp"
#include "cso2/definitions.hpp"
#include "cso2/user.hpp"
#include "packets/view.hpp"
#include "services/userservice.hpp"
#include "util/log.hpp"

#include "packets/in/userprofile.hpp"
#include "packets/in/userprofile/setavatar.hpp"
#include "packets/in/userprofile/setsignature.hpp"
#include "packets/in/userprofile/settitle.hpp"
#include "packets/in/userprofile/updatecampaign.hpp"
#include "packets/out/userinfo.hpp"

void OnUserProfileCampaignStarted(ClientSessionPtr session)
{
    Log::Debug(
        "[OnUserProfileCampaignStarted] user {} sent a campaign start packet\n",
        session->GetUser()->GetId());
}

awaitable<void> OnUserProfileCampaignFinished(
    const InUserProfilePacketUpdateCampaign& campaignPkt,
    ClientSessionPtr session)
{
    if (cso2::IsCampaignFlagValid(campaignPkt.CampaignId) == false)
    {
        Log::Warning("[OnUserProfileCampaignFinished] user {} tried to finish "
                     "unknown campaign\n",
                     session->GetUser()->GetId(), campaignPkt.CampaignId);
        co_return;
    }

    auto sessionUser = session->GetUser();
    auto userId = sessionUser->GetId();
    auto newFlags = sessionUser->GetCampaignFlags() | campaignPkt.CampaignId;

    bool flagsUpdated =
        co_await g_UserService->SetUserCampaignFlags(userId, newFlags);

    if (flagsUpdated == false)
    {
        Log::Warning("[OnUserProfileCampaignFinished] failed to update user "
                     "{}'s campaign flags\n",
                     userId);
        co_return;
    }

    session->Send(OutUserInfoPacket::UpdateCampaignFlags(sessionUser));

    Log::Debug(
        "[OnUserProfileCampaignFinished] user {} finished campaign id {}\n",
        userId, campaignPkt.CampaignId);
}

awaitable<void> OnUserProfileUpdateCampaignRequest(PacketView& packet,
                                                   ClientSessionPtr session)
{
    auto campaignPkt = InUserProfilePacketUpdateCampaign(packet);

    switch (campaignPkt.Type)
    {
        case UP_UpdateCampaignType::Started:
            OnUserProfileCampaignStarted(session);
            break;
        case UP_UpdateCampaignType::Finished:
            co_await OnUserProfileCampaignFinished(campaignPkt, session);
            break;
        default:
            Log::Warning("[OnUserProfileUpdateCampaignRequest] user {} sent an "
                         "unknown {} update campaign packet\n",
                         session->GetUser()->GetId(), campaignPkt.Type);
    }
}

awaitable<void> OnUserProfileSetAvatarRequest(PacketView& packet,
                                              ClientSessionPtr session)
{
    auto avatarPkt = InUserProfilePacketSetAvatar(packet);
    auto sessionUser = session->GetUser();
    auto userId = sessionUser->GetId();

    bool avatarUpdated =
        co_await g_UserService->SetUserAvatar(userId, avatarPkt.AvatarId);

    if (avatarUpdated == false)
    {
        Log::Warning("[OnUserProfileSetAvatarRequest] failed to update user "
                     "{}'s avatar\n",
                     userId);
        co_return;
    }

    session->Send(OutUserInfoPacket::UpdateAvatar(sessionUser));

    Log::Debug("[OnUserProfileSetAvatarRequest] user {} updated avatar to {}\n",
               userId, avatarPkt.AvatarId);
}

awaitable<void> OnUserProfileSetSignatureRequest(PacketView& packet,
                                                 ClientSessionPtr session)
{
    auto signaturePkt = InUserProfilePacketSetSignature(packet);
    auto sessionUser = session->GetUser();
    auto userId = sessionUser->GetId();

    // TODO: find the real signature's max length
    if (signaturePkt.Signature.length() > 64)
    {
        Log::Warning("[OnUserProfileSetSignatureRequest] user {} tried to set "
                     "a way too large signature with length {}\n",
                     userId, signaturePkt.Signature.length());
        co_return;
    }

    bool updated = co_await g_UserService->SetUserSignature(
        userId, std::move(signaturePkt.Signature));

    if (updated == false)
    {
        Log::Warning("[OnUserProfileSetSignatureRequest] failed to update user "
                     "{}'s signature\n",
                     userId);
        co_return;
    }

    session->Send(OutUserInfoPacket::UpdateSignature(sessionUser));

    Log::Debug("[OnUserProfileSetSignatureRequest] user {} updated signature\n",
               userId);
}

awaitable<void> OnUserProfileSetTitleRequest(PacketView& packet,
                                             ClientSessionPtr session)
{
    auto titlePkt = InUserProfilePacketSetTitle(packet);
    auto sessionUser = session->GetUser();
    auto userId = sessionUser->GetId();

    bool updated =
        co_await g_UserService->SetUserTitle(userId, titlePkt.TitleId);

    if (updated == false)
    {
        Log::Warning("[OnUserProfileSetTitleRequest] failed to update user "
                     "{}'s title\n",
                     userId);
        co_return;
    }

    session->Send(OutUserInfoPacket::UpdateTitle(sessionUser));

    Log::Debug("[OnUserProfileSetTitleRequest] user {} updated title to {}\n",
               userId, titlePkt.TitleId);
}

awaitable<void> OnUserProfilePacketRequest(PacketView& packet,
                                           ClientSessionPtr session)
{
    if (session->HasUser() == false)
    {
        Log::Warning("[OnUserProfilePacketRequest] conn '{}' sent an user "
                     "profile packet without session\n",
                     session->GetUniqueId());
        co_return;
    }

    auto profPacket = InUserProfilePacket(packet);

    switch (profPacket.Type)
    {
        case UserProfilePacketType::CampaignUpdate:
            co_await OnUserProfileUpdateCampaignRequest(packet, session);
            break;
        case UserProfilePacketType::SetAvatar:
            co_await OnUserProfileSetAvatarRequest(packet, session);
            break;
        case UserProfilePacketType::SetSignature:
            co_await OnUserProfileSetSignatureRequest(packet, session);
            break;
        case UserProfilePacketType::SetTitle:
            co_await OnUserProfileSetTitleRequest(packet, session);
            break;
        default:
            Log::Warning("[OnUserProfilePacketRequest] connection {} sent an "
                         "unknown {} user profile packet\n",
                         session->GetUniqueId(), profPacket.Type);
    }
}
