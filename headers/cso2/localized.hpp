#ifndef __CSO2_LOCALIZED_H_
#define __CSO2_LOCALIZED_H_

#include <string_view>

using namespace std::string_view_literals;

namespace cso2
{
constexpr const std::string_view ROOM_JOIN_FAILED_CLOSED =
    "#CSO2_POPUP_ROOM_JOIN_FAILED_CLOSED"sv;
constexpr const std::string_view ROOM_JOIN_FAILED_FULL =
    "#CSO2_POPUP_ROOM_JOIN_FAILED_FULL"sv;
constexpr const std::string_view ROOM_JOIN_FAILED_BAD_PASSWORD =
    "#CSO2_POPUP_ROOM_JOIN_FAILED_INVALID_PASSWD"sv;
constexpr const std::string_view ROOM_CHANGETEAM_FAILED =
    "#CSO2_POPUP_ROOM_CHANGETEAM_FAILED"sv;
constexpr const std::string_view ROOM_COUNTDOWN_FAILED_NOENEMIES =
    "#CSO2_UI_ROOM_COUNTDOWN_FAILED_NOENEMY"sv;
constexpr const std::string_view LOGIN_BAD_USERNAME =
    "#CSO2_LoginAuth_Certify_NoPassport"sv;
constexpr const std::string_view LOGIN_BAD_PASSWORD =
    "#CSO2_LoginAuth_WrongPassword"sv;
constexpr const std::string_view LOGIN_INVALID_USERINFO =
    "#CSO2_ServerMessage_INVALID_USERINFO"sv;
}  // namespace cso2

#endif  // __CSO2_LOCALIZED_H_
