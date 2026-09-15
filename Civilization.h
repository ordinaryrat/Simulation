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

struct Civilization {
	int id;

	std::string name;
	std::vector<uint32_t> owned_tiles;
	std::tuple<uint8_t, uint8_t, uint8_t> color;
	Civilization(int civ_id, std::string input_name, std::tuple<uint8_t, uint8_t, uint8_t> input_color, Character* leader);

	std::vector<GameEvent> addAdjTiles(Map* game_map, uint16_t seed_effect);
	
	Government* government;
	std::vector<Character*> characters;
};

#endif
