#include "World.h"
#include "Event.h"
#include "Character.h"
#include <iostream>
#include <algorithm>

std::vector<GameEvent*> World::processCivilization(Civilization* this_civ) {
	std::vector<GameEvent*> return_events = {};
	for (uint16_t c = 0; c < this_civ->characters.size(); c++) {
		std::vector<GameEvent*> temp_list = processCharacter(this_civ, this_civ->characters[c]);
		return_events.insert(return_events.end(), temp_list.begin(), temp_list.end());
	}
	
	// These should become parameters.
	float birth_rate = 0.2f;
	float death_rate = 0.1f;
	for (uint32_t i = 0; i < this_civ->owned_tiles.size(); i++) {
		uint32_t this_tile = this_civ->owned_tiles[i];

		game_map->devastation_map[this_tile] += this_civ->tile_info[this_tile]->population * 0.0001f;
		
		float food_remaining = 10000 * (game_map->fertility_map[this_tile] - game_map->devastation_map[this_tile]) - this_civ->tile_info[this_tile]->population;
		std::cout << "--- NEW DAY ---" << std::endl;
		std::cout << food_remaining << " food left" << std::endl;
		if (food_remaining < 0) {
			if (food_remaining * -1.f > this_civ->tile_info[this_tile]->population) {
				this_civ->tile_info[this_tile]->population = 0;
				std::cout << food_remaining << " people starved" << std::endl;
			} else {
				this_civ->tile_info[this_tile]->population += (int)(food_remaining);
				std::cout << food_remaining << " people starved" << std::endl;
			}
		}
		
		float this_birth_rate = (((rand() + 1) % 10) - 5.f) * 0.02f + birth_rate;
		float this_death_rate = (((rand() + 1) % 10) - 5.f) * 0.02f + death_rate;
		
		uint32_t births = this_civ->tile_info[this_tile]->population * this_birth_rate;
		uint32_t natural_deaths = this_civ->tile_info[this_tile]->population * this_death_rate;
		
		this_civ->tile_info[this_tile]->population += births;
		std::cout << births << " births, " << natural_deaths << " natural deaths" << std::endl;		
		if (natural_deaths >= this_civ->tile_info[this_tile]->population) {
			this_civ->tile_info[this_tile]->population = 0;
		} else {
			this_civ->tile_info[this_tile]->population -= natural_deaths;
		}
		
		std::cout << "End Population: " << this_civ->tile_info[this_tile]->population << std::endl;
		if (this_civ->tile_info[this_tile]->population == 0) {
			this_civ->owned_tiles.erase(std::find(this_civ->owned_tiles.begin(), this_civ->owned_tiles.end(), this_tile));
			this_civ->tile_info.erase(this_civ->tile_info.find(this_tile));
			return_events.push_back(new GameEvent(LAND_LOST, this_tile, this_civ->id));
		
			if (this_civ->owned_tiles.size() == 0) {
				this_civ->government->leader->decisions.clear();
			}
		}
	}
	return return_events;
}
std::vector<GameEvent*> World::processCharacter(Civilization* this_civ, Character* this_char) {
	std::vector<GameEvent*> return_events = {};
	
	if (date[1] == this_char->date_of_birth[1] && date[2] == this_char->date_of_birth[2])
		this_char->age++;

	for (uint16_t i = 0; i < this_char->decisions.size(); i++) {
		Decision* decision = this_char->decisions[i];
		decision->duration--;
		if (decision->duration == 0) {
			switch (decision->type) {
				case MIGRATE: {
					uint32_t former_tile = this_civ->government->capital;
					this_civ->owned_tiles.erase(std::find(this_civ->owned_tiles.begin(), this_civ->owned_tiles.end(), former_tile));
					this_civ->owned_tiles.push_back(decision->related_tile);
					this_civ->tile_info[decision->related_tile] = this_civ->tile_info[former_tile];
					this_civ->tile_info.erase(this_civ->tile_info.find(former_tile));
					this_civ->government->capital = decision->related_tile;

					return_events.push_back(new GameEvent(LAND_TAKEN, decision->related_tile, this_civ->id));
					return_events.push_back(new GameEvent(LAND_BECOME_TRIBAL, former_tile, this_civ->id));
					
					this_char->location = decision->related_tile;

					break;
				}
			}
			this_char->decisions.erase(this_char->decisions.begin() + i);
			for (uint16_t u = 0; u < this_char->decisions_per_type[decision->type].size(); u++) {
				if (this_char->decisions_per_type[decision->type][u] == decision) {
					this_char->decisions_per_type[decision->type].erase(this_char->decisions_per_type[decision->type].begin() + u);
				}
			}
		}
	}
	
	uint32_t capital = this_civ->government->capital;
	// Character decisions
	if (this_char == this_civ->government->leader) {
		// Is leader
		if (this_civ->government->state_type == MIGRATORY) {
			// Check for migration.
			if (this_char->decisions_per_type[MIGRATE].size() == 0 && this_civ->owned_tiles.size() == 1) {
				if (300.f * (game_map->fertility_map[capital] - game_map->devastation_map[capital]) < this_civ->tile_info[capital]->population) { // Very pre-emtive since it takes time to migrate.
					std::vector<uint32_t> possible_tiles = game_map->getAdjacentTiles(capital, false);
					if (possible_tiles.size() != 0) {
						uint32_t tile_weights[possible_tiles.size()];
						uint32_t total_weight = 0;
						
						for (uint8_t i = 0; i < possible_tiles.size(); i++) {
							uint32_t this_weight = (uint32_t)(1000.f * (game_map->fertility_map[possible_tiles[i]] - game_map->devastation_map[possible_tiles[i]]) + 1);
							tile_weights[i] = this_weight;
							total_weight += this_weight;
						}
						
						if (total_weight > 0) {
							uint32_t chosen_tile = 0;
							int random_number = (rand() + 1) % total_weight;
							for (uint8_t i = 0; i < possible_tiles.size(); i++) {
								random_number -= tile_weights[i];
								if (random_number < 0) {
									chosen_tile = possible_tiles[i];
									break;
								}
							}

							Decision* this_decision = new Decision(MIGRATE, (int)(this_civ->tile_info[capital]->population/50.f), chosen_tile); // Larger tribes take more time to migrate.
							this_char->decisions.push_back(this_decision);
							this_char->decisions_per_type[MIGRATE].push_back(this_decision);

							std::cout << "MIGRATING!!!" << std::endl;
						}
					}
				}
			}
		}
	} else {

	}

	return return_events;
}

std::vector<GameEvent*> World::incrementDate() {
	std::vector<GameEvent*> return_events = {};
	date[2] += 1;
	if (date[0] % 4 == 0 && date[1] == 1) {
		if (date[2] > 29) {
			date[2] = 1;
			date[1]++;
		}
	} else if (date[2] > lengths_of_months[date[1]]) {
		date[2] = 1;
		date[1]++;
	}
	if (date[1] > 11) {
		date[0]++;
		date[1] = 0;
	}
	for (uint16_t i = 0; i < civilizations.size(); i++) {
		std::vector<GameEvent*> civ_events = processCivilization(civilizations[i]);
		return_events.insert(return_events.end(), civ_events.begin(), civ_events.end());
	}

	uint16_t day_of_year = 0;
	for (uint8_t i = 0; i < date[1]; i++) {
		day_of_year += lengths_of_months[i];
	}
	if (date[0] % 4 == 0 && date[1] > 1) {
		day_of_year++;
	}
	day_of_year += date[2];

	float temperature_modifier = 30.f * (0.5f - fastAbs(day_of_year/365.f - 0.5f));	

	bool do_terrain_change = false;
	
	for (uint32_t i = 0; i < game_map->width * game_map->height; i++) {
		if (game_map->devastation_map[i] != 0.f) {
			game_map->devastation_map[i] -= 0.01f;
			if (game_map->devastation_map[i] < 0.f) {
				game_map->devastation_map[i] = 0.f;
			}
		}
		game_map->temperature_map[i] = game_map->equ_temperature_map[i] + temperature_modifier;
	
		if (!game_map->land_map[i]) {
			if (game_map->terrain_map[i] != ICE && game_map->temperature_map[i] < 0) {
				game_map->terrain_map[i] = ICE;
				do_terrain_change = true;
			}
			else {
				if (game_map->terrain_map[i] == ICE && game_map->temperature_map[i] >= 0) {
					if (game_map->elevation_map[i] < (1 - game_map->land_factor * 2)) {
						game_map->terrain_map[i] = OCEAN;
						do_terrain_change = true;
					} else if (game_map->elevation_map[i] < (1 - game_map->land_factor * 1.2)) {
						game_map->terrain_map[i] = SEA;
						do_terrain_change = true;
					} else if (game_map->elevation_map[i] < (1 - game_map->land_factor)) {
						game_map->terrain_map[i] = COASTAL;
						do_terrain_change = true;
					}
				}
			}
		} else {

			if (game_map->temperature_map[i] < 0) {
				game_map->terrain_map[i] = ARCTIC;
				continue;
			}

			if (game_map->precipitation_map[i] < 0.2) {
				if (game_map->temperature_map[i] < 15)
					game_map->terrain_map[i] = TUNDRA;
				else
					game_map->terrain_map[i] = DESERT;
				continue;
			}
			if (game_map->precipitation_map[i] < 0.4) {
				if (game_map->temperature_map[i] < 12) 
					game_map->terrain_map[i] = TUNDRA;
				else 
					game_map->terrain_map[i] = PLAINS;
				continue;
			}
			if (game_map->precipitation_map[i] < 0.8) {
				if (game_map->temperature_map[i] < 8)
					game_map->terrain_map[i] = TUNDRA;
				else 
					game_map->terrain_map[i] = GRASSLAND;
				continue;
			}
			if (game_map->temperature_map[i] < 25)
				game_map->terrain_map[i] = GRASSLAND;
			else 
				game_map->terrain_map[i] = JUNGLE;
		}
	}
	
	return_events.push_back(new GameEvent(TEMP_CHANGE));
	if (do_terrain_change) {
		return_events.push_back(new GameEvent(TERRAIN_CHANGE));
	}
	return return_events;
}

int World::fastFloor(float x) {
	if (x > 0)
		return (int)x;
	else
		return (int)x - 1;
}

float World::fastAbs(float input) {
	if (input > 0.f) {
		return input;
	} else {
		return input * -1.f;
	}
}
