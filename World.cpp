#include "World.h"
#include "Event.h"
#include "Character.h"

std::vector<GameEvent> World::processCivilization(Civilization* this_civ) {
	for (uint16_t c = 0; c < this_civ->characters.size(); c++) {
		processCharacter(this_civ->characters[c]);
	}
	return {};
}
std::vector<GameEvent> World::processCharacter(Character* this_char) {
	if (date[1] == this_char->date_of_birth[1] && date[2] == this_char->date_of_birth[2])
		this_char->age++;
	return {};
}

std::vector<GameEvent> World::incrementDate() {
	std::vector<GameEvent> return_events = {};
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
		std::vector<GameEvent> civ_events = processCivilization(civilizations[i]);
		return_events.insert(return_events.end(), civ_events.begin(), civ_events.end());
	}

	return return_events;
}
