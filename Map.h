#ifndef MAP_H
#define MAP_H

#include <cstdint>
#include <vector>
#include <map>

enum terrain {
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

class Map {
	public:
		uint16_t width;
		uint16_t height;
		
		bool* land_map; // Stores if the tile is a land tile.
		float* elevation_map;
		float* temperature_map;
		float* precipitation_map;
		float* fertility_map;
		
		float* devastation_map;

		terrain* terrain_map;

		float fastAbs(float input);
	
		std::vector<uint32_t> getAdjacentTiles(uint32_t tile, bool water_invalid = true);
		Map(uint16_t input_width, uint16_t input_height, float land_factor = 0.3, float max_temp = 40, float min_temp = -40);
};
#endif
