#include "Civilization.h"
#include "Map.h"
#include <iostream>
#include <ctime>
#include "Event.h"
#include <vector>

Civilization::Civilization(int civ_id, std::string input_name, std::tuple<uint8_t, uint8_t, uint8_t> input_color, Character* leader, uint32_t capital, Map* game_map) {
	// I don't think these parameters should be permanent.
	id = civ_id;
	name = input_name;
	color = input_color;
	
	// Assuming randomly generating government and it is a MIGRATORY tribe.
	government = new Government(leader, MIGRATORY, capital);
	characters.push_back(leader);
	tile_info.insert({capital, new Tile(100)});
	owned_tiles.push_back(capital);
	tribal_land.push_back(capital);
}

std::vector<GameEvent> Civilization::addAdjTiles(Map* game_map, uint16_t seed_effect) {
	std::vector<GameEvent> return_events = {};
	std::srand(std::time({}) + seed_effect);
	std::vector<uint32_t> temp_tiles = {};
	for (uint32_t tile : owned_tiles) {
		std::vector<uint32_t> adj_tiles = game_map->getAdjacentTiles(tile, game_map);	
		temp_tiles.insert(temp_tiles.end(), adj_tiles.begin(), adj_tiles.end());
	}
	uint32_t i = 0;
	while (i < temp_tiles.size()) {
		bool found = false;
		for (uint32_t tile : owned_tiles) {
			if (temp_tiles[i] == tile) {
				temp_tiles.erase(temp_tiles.begin() + i);
				found = true;
				break;
			}
		}
		if (!found)
			i++;
	}
	
	uint16_t selected_option = rand() % temp_tiles.size();
	
	GameEvent new_event(LAND_TAKEN, temp_tiles[selected_option], id);
	return_events.push_back(new_event);

	owned_tiles.push_back(temp_tiles[selected_option]);
	return return_events;
}
