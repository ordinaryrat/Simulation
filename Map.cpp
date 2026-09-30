#include "Map.h"
#include "Simplex.h"
#include <vector>
#include <algorithm>
#include <iterator>

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

	land_map = new bool[input_width * input_height];
	terrain_map = new terrain[input_width * input_height];
	temperature_map = new float[input_width * input_height];
	equ_temperature_map = new float[input_width * input_height];
	fertility_map = new float[input_width * input_height];
	devastation_map = (float*)calloc(input_width * input_height, sizeof(float));

	Simplex temp_elevation_map(input_width, input_height, 0.005, 1);
	Simplex factor_temperature_map(input_width, input_height, 0.02, 2);
	Simplex factor_precipitation_map(input_width, input_height, 0.01, 3);
	
	for (uint16_t x = 0; x < input_width; x++) {
		for (uint16_t y = 0; y < input_height; y++) {
			int i = (y * input_width) + x;
			
			land_map[i] = temp_elevation_map.grid[i] > (1 - land_factor);
			float temp_temperature = 
				max_temp - 
				(((4.0*(y - input_height/2.0)*(y - input_height/2.0))
				/((float)(input_height*input_height))) 
				* (max_temp - min_temp));
			temp_temperature += ((30 * (0.5 + factor_temperature_map.grid[i])) - 30);
			
			if (!land_map[i]) // Sea is colder
				temp_temperature = (temp_temperature + ((max_temp + min_temp) * 0.5 * 1))/2;
			
			temperature_map[i] = temp_temperature;
			equ_temperature_map[i] = temp_temperature;

			precipitation_map = factor_precipitation_map.grid;
			elevation_map = temp_elevation_map.grid;
			
			if (!land_map[i]) {
				fertility_map[i] = 0;
			} else {
				float c1 = (fastAbs(0.8f - precipitation_map[i])/0.8f) * (fastAbs(0.8f - precipitation_map[i])/0.8f);
				float c2 = fastAbs(26.f - temperature_map[i])/26.f;
				fertility_map[i] = 1.f - (c1+c2);
				if (fertility_map[i] < 0) {
					fertility_map[i] = 0;
				}
			}
			
			if (elevation_map[i] < (1 - land_factor) && temperature_map[i] < 0) {
				terrain_map[i] = ICE;
				continue;
			}
			else if (elevation_map[i] < (1 - land_factor * 2)) {
				terrain_map[i] = OCEAN;
				continue;
			} else if (elevation_map[i] < (1 - land_factor * 1.2)) {
				terrain_map[i] = SEA;
				continue;
			} else if (elevation_map[i] < (1 - land_factor)) {
				terrain_map[i] = COASTAL;
				continue;
			}
			
			// This is land now.
			if (elevation_map[i] > 0.95) {
				terrain_map[i] = MOUNTAIN;
				continue;
			}
			
			if (temperature_map[i] < 0) {
				terrain_map[i] = ARCTIC;
				continue;
			}

			if (precipitation_map[i] < 0.2) {
				if (temperature_map[i] < 15)
					terrain_map[i] = TUNDRA;
				else
					terrain_map[i] = DESERT;
				continue;
			}
			if (precipitation_map[i] < 0.4) {
				if (temperature_map[i] < 12) 
					terrain_map[i] = TUNDRA;
				else 
					terrain_map[i] = PLAINS;
				continue;
			}
			if (precipitation_map[i] < 0.8) {
				if (temperature_map[i] < 8)
					terrain_map[i] = TUNDRA;
				else 
					terrain_map[i] = GRASSLAND;
				continue;
			}
			if (temperature_map[i] < 25)
				terrain_map[i] = GRASSLAND;
			else 
				terrain_map[i] = JUNGLE;
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

