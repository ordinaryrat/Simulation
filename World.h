#ifndef WORLD_H 
#define WORLD_H 

#include <vector>
#include <map>
#include "Map.h"
#include "Civilization.h"

struct World {
	Map* game_map;
	std::vector<Civilization*> civilizations;
	int next_civ_id = 0; // If we want to store info on past civs we need to do this.

	int date[3] = {0, 0, 0};
	
	World(Map& input_game_map) {
		game_map = &input_game_map;	
	}

	std::vector<GameEvent> incrementDate(); 
	
	std::map<int, Civilization*> civ_look_up_table = {};

	std::map<uint32_t, Civilization*> tiles_look_up_table = {};
};

#endif
