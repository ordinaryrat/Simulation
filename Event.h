#ifndef GAMEEVENT_H
#define GAMEEVENT_H

enum event_type {
	LAND_TAKEN,
};

struct GameEvent {
	event_type type;
	uint32_t para1 = 0;
	uint32_t para2 = 0;
	std::string related_civ_name = ""; // Annoyingly I cannot do Civilization* directly because Civilization needs this struct.

	GameEvent(event_type new_type, uint32_t i_para1 = 0, std::string i_related_civ_name = "") {
		type = new_type;
		para1 = i_para1;
		related_civ_name = i_related_civ_name;
	}
};

#endif
