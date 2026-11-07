#include "../../src/veyraCrew.h"
#include <cassert>
#include <iostream>

int main()
{
    auto captain = std::make_shared<PlayerInfo>();
    captain->client_id = 17; captain->ship_id = 101; captain->crew_position[relayOfficer] = 1;
    auto reader = std::make_shared<PlayerInfo>();
    reader->client_id = 18; reader->ship_id = 101;
    auto other_ship = std::make_shared<PlayerInfo>();
    other_ship->client_id = 19; other_ship->ship_id = 202; other_ship->crew_position[relayOfficer] = 1;
    player_info_list = {captain, reader, other_ship};
    assert(veyraClientMayAnswer(17,101));
    assert(!veyraClientMayAnswer(18,101));
    assert(!veyraClientMayAnswer(19,101));
    assert(veyraClientMayAnswer(19,202));
    assert(!veyraClientMayAnswer(999,101));
    assert(!veyraIsDecisionMaker({},101));
    captain->crew_position[relayOfficer] = 0;
    assert(!veyraClientMayAnswer(17,101));
    captain->client_id = 0; captain->crew_position[relayOfficer] = 2;
    assert(veyraClientMayAnswer(0,101));
    assert(!veyraClientMayAnswer(-1,101));
    captain->ship_id = -1;
    assert(!veyraClientMayAnswer(0,101));
    std::cout << "PASS: Relay authority checks reject readers, other ships, unknown clients and unassigned roles.\n";
}
