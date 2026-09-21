#ifndef GOVERNMENT_H
#define GOVERNMENT_H

#include <string>
#include "Character.h"

enum StateType {
	MIGRATORY,
};

struct Government {
	std::string name;
	// TODO this will need to be greatly expanded in the future.

	uint32_t capital;
	Character* leader;
	
	StateType state_type;

	Government();
	Government(Character* set_leader, StateType set_state_type, uint32_t set_capital) {
		leader = set_leader;
		state_type = set_state_type;
		capital = set_capital;
	}
};

#endif
