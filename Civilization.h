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

struct TechnologyType {

};

/*
struct PopulationMember {
	// Basic information for less important members. There is going to be a lot of these so this should be small (currently 12 bytes).
	// If this is too slow can make optimizations. Maybe each population member represents 7 people.
	uint16_t age;
	Culture* culture;
	uint8_t date_of_birth[2];
	PopulationMember(Culture* set_culture) {
		uint8_t lengths_of_months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
		age = ((rand() + 1) % 100) - 50;
		uint8_t month_of_birth = ((rand() + 1) % 12);
		uint8_t day_of_birth = ((rand() + 1) % lengths_of_months[month_of_birth]);
		date_of_birth = {month_of_birth, day_of_birth};
	}
};
*/

struct Tile {
	uint32_t population;	
	
	uint32_t starving_population;

	std::map<Culture*, uint32_t> cultures;
	std::map<uint8_t, uint32_t> ages;

	Tile(uint32_t set_population, Culture* culture) {
		/*for (uint32_t i = 0; i < set_population; i++) {
			population.push_back();
		}*/
		population = set_population;
	}
};

struct Civilization {
	int id;

	std::string name;
	std::vector<uint32_t> owned_tiles;
	std::vector<uint32_t> tribal_land;
		
	std::map<uint32_t, Tile*> tile_info;
	
	std::map<TechnologyType, uint8_t> tech_levels;

	Culture* culture;

	std::tuple<uint8_t, uint8_t, uint8_t> color;
	Civilization(int civ_id, std::string input_name, std::tuple<uint8_t, uint8_t, uint8_t> input_color, Character* leader, uint32_t capital, Map* game_map, Culture* set_culture);

	std::vector<GameEvent> addAdjTiles(Map* game_map, uint16_t seed_effect);
		
	Government* government;
	std::vector<Character*> characters;

	int total_gold = 0;
	uint32_t total_population = 0;
};

#endif
