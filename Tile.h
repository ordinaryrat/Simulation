#ifndef TILE_H 
#define TILE_H 

#include <map>

enum Terrain {
	OCEAN,
	SEA,
	COASTAL,
	DESERT,
	PLAINS,
	GRASSLAND,
	MOUNTAIN,
	JUNGLE,
	ARCTIC,
	ICE,
	TUNDRA,
};

struct Tile {
	// Raw Traits
	bool land = false;
	float elevation = 0;
	float equ_temperature = 0;
	float temperature = 0;
	float precipitation = 0;
	float fertility = 0;
	float devastation = 0;
	Terrain terrain = OCEAN;

	// Civilization Traits
	uint32_t population = 0;	
	uint32_t starving_population = 0;

	//std::map<Culture*, uint32_t> cultures;
	std::map<uint8_t, uint32_t> ages;
	
	//Civilization* owner;

	Tile() {};
};

#endif
