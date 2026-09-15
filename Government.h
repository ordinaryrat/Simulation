#ifndef GOVERNMENT_H
#define GOVERNMENT_H

#include <string>
#include "Character.h"

struct Government {
	std::string name;
	// TODO this will need to be greatly expanded in the future.
	
	Character* leader;

	Government();
	Government(Character* set_leader) {
		leader = set_leader;
	}
};

#endif
