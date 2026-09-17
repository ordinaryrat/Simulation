#ifndef GAMEEVENT_H
#define GAMEEVENT_H

// Game Events are sent over and signify visual changes. These can also be used to notify player of stuff.

enum event_type {
	LAND_TAKEN,
};

struct GameEvent {
	event_type type;
	uint32_t para1 = 0;
	uint32_t para2 = 0;
	int related_civ_id = 0;

	GameEvent(event_type new_type, uint32_t i_para1 = 0, int i_related_civ_id = 0) {
		type = new_type;
		para1 = i_para1;
		related_civ_id = i_related_civ_id;
	}
};

#endif
