#ifndef MAP_H
#define MAP_H

#include <cstdint>

enum terrain {
	OCEAN,
	SEA,
	COASTAL,
	DESERT,
	PLAINS,
	MOUNTAINS,
	JUNGLE,
	FOREST,
};

class Map {
	public:
		uint16_t width;
		uint16_t height;
		
		bool* land_map; // Stores if the tile is a land tile.
		float* elevation_map;
		int* heat_map;
		float* precipitation_map;

		terrain* terrain_map;

		Map(uint16_t input_width, uint16_t input_height, float land_factor = 0.3);
};
#endif
