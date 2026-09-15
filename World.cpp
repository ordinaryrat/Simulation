#include "World.h"
#include "Event.h"

std::vector<GameEvent> World::incrementDate() {
	std::vector<GameEvent> return_events = {};
	date[1] += 1;
	if (date[1] > 11) {
		date[0]++;
		date[1] = 0;
	}
	for (uint16_t i = 0; i < civilizations.size(); i++) {
		std::vector<GameEvent> civ_events = civilizations[i]->addAdjTiles(game_map, i);
		return_events.insert(return_events.end(), civ_events.begin(), civ_events.end());
		for (uint16_t c = 0; c < civilizations[i]->characters.size(); c++) {
			Character* this_char = civilizations[i]->characters[c];
			if (date[1] == this_char->date_of_birth[1] && date[2] == this_char->date_of_birth[2])
				this_char->age++;
		}
	}

	return return_events;
}
