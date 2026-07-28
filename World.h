#ifndef WORLD_H 
#define WORLD_H 

#include <vector>
#include "Map.h"
#include "Civilization.h"

struct World {
	Map* game_map;
	std::vector<Civilization*> civilizations;
	
	float date[3] = {0.f, 0.f, 0.f};
	
	World(Map& input_game_map) {
		game_map = &input_game_map;	
	}
};

#endif
