#include "Character.h"
#include "Culture.h"

Character::Character(uint32_t set_location, uint32_t date[3], Culture* set_culture) {
	age = 20;
	name = set_culture->name_generator->generateName();
	location = set_location;

	date_of_birth[0] = date[0];
	date_of_birth[1] = date[1];
	date_of_birth[2] = date[2];
}
Character::Character(uint32_t set_location, uint32_t date[3], Character* set_parent) {
	age = 0;
	name = "Dob";
	location = set_location;

	date_of_birth[0] = date[0];
	date_of_birth[1] = date[1];
	date_of_birth[2] = date[2];
}
void Character::initializeDecisionTypes() {
	for (uint16_t i = 0; i != TEMP_DECISION_TYPE_END; i++) {
		decisions_per_type[static_cast<DecisionType>(i)] = {};	
	}
}
