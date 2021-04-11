#ifndef __PACKETS_USERINFOSHARED_H_
#define __PACKETS_USERINFOSHARED_H_

#include "cso2/shared.hpp"

class DynamicBuffer;

void WriteUserInfoUpdate(DynamicBuffer& buf, std::uint32_t flags,
                         cso2::UserPtr user);

#endif  // __PACKETS_USERINFOSHARED_H_
