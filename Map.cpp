#include "Map.h"
#include "Simplex.h"
#include <vector>
#include <algorithm>
#include <iterator>

#include "Tile.h"

float Map::fastAbs(float input) {
	if (input > 0.f) {
		return input;
	} else {
		return input * -1.f;
	}
}

Map::Map(uint16_t input_width, uint16_t input_height, float i_land_factor, float max_temp, float min_temp) {
	land_factor = i_land_factor;

	width = input_width;
	height = input_height;

	Simplex temp_elevation_map(input_width, input_height, 0.005, 1);
	Simplex factor_temperature_map(input_width, input_height, 0.02, 2);
	Simplex factor_precipitation_map(input_width, input_height, 0.01, 3);
	
	for (uint16_t y = 0; y < input_height; y++) {
		for (uint16_t x = 0; x < input_width; x++) {
			Tile* new_tile = new Tile();
			tiles.push_back(new_tile);

			int i = (y * input_width) + x;
			
			new_tile->land = temp_elevation_map.grid[i] > (1 - land_factor);
			float temp_temperature = 
				max_temp - 
				(((4.0*(y - input_height/2.0)*(y - input_height/2.0))
				/((float)(input_height*input_height))) 
				* (max_temp - min_temp));
			temp_temperature += ((30 * (0.5 + factor_temperature_map.grid[i])) - 30);
			
			if (!new_tile->land) // Sea is colder
				temp_temperature = (temp_temperature + ((max_temp + min_temp) * 0.5 * 1))/2;
			
			new_tile->temperature = temp_temperature;
			new_tile->equ_temperature = temp_temperature;

			new_tile->precipitation = factor_precipitation_map.grid[i];
			new_tile->elevation = temp_elevation_map.grid[i];
			
			if (!new_tile->land) {
				new_tile->fertility = 0;
			} else {
				float c1 = (fastAbs(0.8f - new_tile->precipitation)/0.8f) * (fastAbs(0.8f - new_tile->precipitation)/0.8f);
				float c2 = fastAbs(26.f - new_tile->temperature)/26.f;
				new_tile->fertility = 1.f - (c1+c2);
				if (new_tile->fertility < 0) {
					new_tile->fertility = 0;
				}
			}
			
			if (new_tile->elevation < (1 - land_factor) && new_tile->temperature < 0) {
				new_tile->terrain = ICE;
				continue;
			}
			else if (new_tile->elevation < (1 - land_factor * 2)) {
				new_tile->terrain = OCEAN;
				continue;
			} else if (new_tile->elevation < (1 - land_factor * 1.2)) {
				new_tile->terrain = SEA;
				continue;
			} else if (new_tile->elevation < (1 - land_factor)) {
				new_tile->terrain = COASTAL;
				continue;
			}
			
			// This is land now.
			if (new_tile->elevation > 0.95) {
				new_tile->terrain = MOUNTAIN;
				continue;
			}
			
			if (new_tile->temperature < 0) {
				new_tile->terrain = ARCTIC;
				continue;
			}

			if (new_tile->precipitation < 0.2) {
				if (new_tile->temperature < 15)
					new_tile->terrain = TUNDRA;
				else
					new_tile->terrain = DESERT;
				continue;
			}
			if (new_tile->precipitation < 0.4) {
				if (new_tile->temperature < 12) 
					new_tile->terrain = TUNDRA;
				else 
					new_tile->terrain = PLAINS;
				continue;
			}
			if (new_tile->precipitation < 0.8) {
				if (new_tile->temperature < 8)
					new_tile->terrain = TUNDRA;
				else 
					new_tile->terrain = GRASSLAND;
				continue;
			}
			if (new_tile->temperature < 25)
				new_tile->terrain = GRASSLAND;
			else 
				new_tile->terrain = JUNGLE;
		}
	}
}

std::vector<uint32_t> Map::getAdjacentTiles(uint32_t tile, bool water_invalid) {
	std::vector<uint32_t> output_list = {};
	
	if (tile % width > 0)
		output_list.push_back(tile - 1);
	if (tile % width < width - 1)
		output_list.push_back(tile + 1);
	if (tile / width >= 1)
		output_list.push_back(tile - width);
	if (tile / width <= height - 1)
		output_list.push_back(tile + width);
	return output_list;
}

