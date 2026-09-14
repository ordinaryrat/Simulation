#ifndef CIVILIZATION_H
#define CIVILIZATION_H

#include <string>
#include <cstdint>
#include <vector>
#include "Map.h"
#include "Event.h"
struct NameGenerator {

};

struct Civilization {
	int id;

	std::string name;
	std::vector<uint32_t> owned_tiles;
	std::tuple<uint8_t, uint8_t, uint8_t> color;

	Civilization(int civ_id, std::string input_name, std::tuple<uint8_t, uint8_t, uint8_t> input_color) {
		id = civ_id;
		name = input_name;
		color = input_color;
	}

	std::vector<GameEvent> addAdjTiles(Map* game_map, uint16_t seed_effect);
};

struct Culture {
	std::string name;
	NameGenerator* name_generator;
};

#endif
