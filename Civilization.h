#ifndef CIVILIZATION_H
#define CIVILIZATION_H

#include <string>
#include <cstdint>
#include <vector>
#include "Map.h"
#include "Event.h"
#include "Culture.h"
#include "Character.h"
#include "Government.h"

struct Tile {
	uint32_t population;	

	Tile(uint32_t set_population) {
		population = set_population;
	}
};

struct Civilization {
	int id;

	std::string name;
	std::vector<uint32_t> owned_tiles;

	std::map<uint32_t, Tile*> tile_info;

	std::tuple<uint8_t, uint8_t, uint8_t> color;
	Civilization(int civ_id, std::string input_name, std::tuple<uint8_t, uint8_t, uint8_t> input_color, Character* leader, uint32_t capital, Map* game_map);

	std::vector<GameEvent> addAdjTiles(Map* game_map, uint16_t seed_effect);
		
	Government* government;
	std::vector<Character*> characters;

	int total_gold = 0;
	uint32_t total_population = 0;
};

#endif
