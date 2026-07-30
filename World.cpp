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
		std::vector<GameEvent> civ_events = civilizations[i].addAdjTiles(game_map, i);
		return_events.insert(return_events.end(), civ_events.begin(), civ_events.end());
	}

	return return_events;
}
