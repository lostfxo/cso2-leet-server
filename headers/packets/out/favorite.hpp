#ifndef __OUT_PKT_FAVORITE_
#define __OUT_PKT_FAVORITE_

#include "cso2/shared.hpp"
#include "packets/builder.hpp"

class OutFavoritePacket
{
public:
    static PacketBuilder SetCosmetics(cso2::CosmeticsPtr cosmetics);
    static PacketBuilder SetAllLoadouts(const cso2::LoadoutsArray& loadouts);

private:
};

#endif  // __OUT_PKT_FAVORITE_
