#ifndef WORLD_H 
#define WORLD_H 

#include <vector>
#include <map>
#include "Map.h"
#include "Civilization.h"
#include "Character.h"

struct World {
	Map* game_map;
	std::vector<Civilization*> civilizations;
	int next_civ_id = 0; // If we want to store info on past civs we need to do this.

	uint32_t date[3] = {0, 0, 1}; 
	
	World(Map& input_game_map) {
		game_map = &input_game_map;	
	}

	std::vector<GameEvent*> incrementDate(); 
	
	std::vector<GameEvent*> processCivilization(Civilization* this_civ); 
	std::vector<GameEvent*> processCharacter(Civilization* this_civ, Character* this_char); 
	int fastFloor(float x);
	float fastAbs(float x);

	std::map<int, Civilization*> civ_look_up_table = {};

	std::map<uint32_t, Civilization*> tiles_look_up_table = {};
	std::map<uint32_t, Civilization*> tribal_land_look_up_table = {};
	
	uint8_t lengths_of_months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
};

#endif
