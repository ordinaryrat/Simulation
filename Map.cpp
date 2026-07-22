#include "Map.h"
#include "Simplex.h"

Map::Map(uint16_t input_width, uint16_t input_height, float land_factor) {
	land_map = new bool[input_width * input_height];
	terrain_map = new terrain[input_width * input_height];
	
	Simplex temp_elevation_map(input_width, input_height, 0.005);
	
	for (int i = 0; i < input_width * input_height; i++) {
		land_map[i] = temp_elevation_map.grid[i] > (1 - land_factor);
		if (temp_elevation_map.grid[i] < (1 - land_factor * 2))
			terrain_map[i] = OCEAN;
		else if (temp_elevation_map.grid[i] < (1 - land_factor * 1.2))
			terrain_map[i] = SEA;
		else if (temp_elevation_map.grid[i] < (1 - land_factor))
			terrain_map[i] = COASTAL;
		else
			terrain_map[i] = DESERT;
	}
	
	Simplex heat_map(input_width, input_height, 0.03);

}
