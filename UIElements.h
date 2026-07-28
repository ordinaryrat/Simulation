#ifndef UI_ELEMENTS_H
#define UI_ELEMENTS_H

#include <SFML/Graphics.hpp>
#include <string>
#include <tuple>

enum ButtonID {
	TERRAIN_SECTION,
	BACK,
	LANDMAP,
	RAINFALLMAP,
	TERRAINMAP,
	HEATMAP,
};

enum PanelType {
	RADIO,
	MULTICHOICE,
};

struct Button {
	sf::Sprite* button_sprite;
	sf::FloatRect* button_collision_box;
	float pos_x = 0;
	float pos_y = 0;
	
	sf::Vector2u button_size;
	
	ButtonID button_id;
	
	void setPosition(float input_x, float input_y) {
		pos_x = input_x;
		pos_y = input_y;
	
		button_collision_box = new sf::FloatRect({pos_x, pos_y}, {(float)button_size.x, (float)button_size.y});
	}

	Button(sf::Texture& button_texture, ButtonID input_button_id) {
		button_sprite = new sf::Sprite(button_texture);
		button_size = button_texture.getSize();
		button_collision_box = new sf::FloatRect({pos_x, pos_y}, {(float)button_size.x, (float)button_size.y});
	}

	Button(float input_x, float input_y, sf::Texture& button_texture, ButtonID input_button_id) {
		pos_x = input_x;
		pos_y = input_y;

		button_id = input_button_id;

		button_sprite = new sf::Sprite(button_texture);
		button_size = button_texture.getSize();
		button_collision_box = new sf::FloatRect({pos_x, pos_y}, {(float)button_size.x, (float)button_size.y});
	}
};

enum selection_panel_id {
	MAP_TYPE,
	MAP_LAYERS,
};

struct SelectionPanel {
	PanelType panel_type;

	selection_panel_id panel_id;

	std::string title;
	std::vector<std::string> text_options;
	
	uint8_t value;
	bool* values;
	
	float font_size;

	sf::Text** text_options_sprites;
	std::vector<sf::FloatRect> text_options_rects = {};
	
	float left;
	float top;
	
	sf::FloatRect panel_rect;

	SelectionPanel(float new_left, float new_top, std::string new_title, std::vector<std::string> input_options, sf::Font &used_font, int new_font_size, selection_panel_id new_panel_id, PanelType new_panel_type, uint8_t default_value = 0); 
};

#endif
