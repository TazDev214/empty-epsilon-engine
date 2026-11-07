#pragma once
#include "playerInfo.h"

inline bool veyraIsDecisionMaker(P<PlayerInfo> player, int32_t ship_id)
{
    return player && player->ship_id == ship_id && player->crew_position[relayOfficer] != 0;
}

inline bool veyraClientMayAnswer(int32_t client_id, int32_t ship_id)
{
    if (client_id < 0) return false;
    foreach(PlayerInfo, player, player_info_list)
        if (player->client_id == client_id && veyraIsDecisionMaker(player, ship_id))
            return true;
    return false;
}
