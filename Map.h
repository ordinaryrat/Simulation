#ifndef MAP_H
#define MAP_H

#include <cstdint>
#include <vector>
#include <map>
#include "Tile.h"

struct Map {
	float land_factor;

	uint16_t width;
	uint16_t height;
	
	std::vector<Tile*> tiles;

	float fastAbs(float input);

	std::vector<uint32_t> getAdjacentTiles(uint32_t tile, bool water_invalid = true);
	Map(uint16_t input_width, uint16_t input_height, float land_factor = 0.3, float max_temp = 40, float min_temp = -40);
};
#endif
