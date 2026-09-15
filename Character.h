#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <cstdint>
#include <vector>
#include <map>
#include <cstdint>
#include "Culture.h"

enum Trait {
	
};

struct PersonalityTrait {
	Trait trait;
	uint8_t strength;

	PersonalityTrait(Trait set_trait, uint8_t set_strength) {
		trait = set_trait;
		strength = set_strength;
	}
};

struct Character {
	std::string name;
	
	uint16_t age;
	uint32_t location;

	Character* parent;
	std::vector<Character*> children;
	
	uint32_t date_of_birth[3] = {0, 0, 0};	

	Culture* culture;
	std::vector<PersonalityTrait> natural_traits;
	std::vector<PersonalityTrait> learned_traits;
	std::map<std::string, uint8_t> stats;
	
	void perDay(); // Will need info from world.h?

	Character(uint32_t set_location, uint32_t date[3], Culture* set_culture);
	Character(uint32_t set_location, uint32_t date[3], Character* set_parent);
};

#endif
