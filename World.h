#ifndef WORLD_H 
#define WORLD_H 

#include <vector>
#include <map>
#include "Map.h"
#include "Civilization.h"

struct World {
	Map* game_map;
	std::vector<Civilization*> civilizations;
	
	int date[3] = {0, 0, 0};
	
	World(Map& input_game_map) {
		game_map = &input_game_map;	
	}

	std::vector<GameEvent> incrementDate(); 
	
	std::map<std::string, Civilization*> civ_look_up_table = {};
};

#endif
