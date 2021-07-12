#ifndef __SYSTEM_L10N_H_
#define __SYSTEM_L10N_H_

#include <string_view>

using namespace std::string_view_literals;

namespace l10n
{
static std::string_view LOGIN_BAD_USERNAME =
    "#CSO2_LoginAuth_Certify_NoPassport"sv;
static std::string_view LOGIN_BAD_PASSWORD = "#CSO2_LoginAuth_WrongPassword"sv;
static std::string_view LOGIN_INVALID_USERINFO =
    "#CSO2_ServerMessage_INVALID_USERINFO"sv;
static std::string_view POPUP_ROOM_JOIN_FAILED_CLOSED =
    "#CSO2_POPUP_ROOM_JOIN_FAILED_CLOSED"sv;
static std::string_view POPUP_ROOM_JOIN_FAILED_FULL =
    "#CSO2_POPUP_ROOM_JOIN_FAILED_FULL"sv;
static std::string_view POPUP_ROOM_JOIN_FAILED_INVALID_PASSWD =
    "#CSO2_POPUP_ROOM_JOIN_FAILED_INVALID_PASSWD"sv;
static std::string_view POPUP_ROOM_CHANGETEAM_FAILED =
    "#CSO2_POPUP_ROOM_CHANGETEAM_FAILED"sv;
static std::string_view UI_ROOM_COUNTDOWN_FAILED_NOENEMY =
    "#CSO2_UI_ROOM_COUNTDOWN_FAILED_NOENEMY"sv;
}  // namespace l10n

#endif  // __SYSTEM_L10N_H_
