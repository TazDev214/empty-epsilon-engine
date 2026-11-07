#pragma once
#include <memory>
#include <vector>
#include <cstdint>
enum { relayOfficer = 4, max_crew_positions = 15 };
struct PlayerInfo { int32_t client_id = -1; int32_t ship_id = -1; uint32_t crew_position[max_crew_positions] = {}; };
template<class T> using P = std::shared_ptr<T>;
inline std::vector<P<PlayerInfo>> player_info_list;
#define foreach(type, item, container) for (auto item : container)
