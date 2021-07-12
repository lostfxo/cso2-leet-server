#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include <string>
#include <string_view>

std::pair<bool, std::string> FindIpOfInterface(std::string_view intf);

#endif  // _PLATFORM_H_
