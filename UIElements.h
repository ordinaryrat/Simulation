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
};

struct SelectionPanel {
	selection_panel_id panel_id;

	std::string title;
	std::vector<std::string> text_options;
	
	uint8_t value;
	
	std::vector<sf::Text> text_options_sprites = {};
	std::vector<sf::FloatRect> text_options_rects = {};

	float left;
	float top;

	SelectionPanel(float new_left, float new_top, std::string new_title, std::vector<std::string> input_options, sf::Font &used_font, int font_size, selection_panel_id new_panel_id, uint8_t default_value = 0) {
		text_options = input_options;

		for (std::string option : text_options) {
			sf::Text this_text(used_font, option, font_size);
			text_options_sprites.push_back(this_text);
			
			sf::FloatRect this_text_bounds = this_text.getGlobalBounds();
			this_text_bounds.size.x += 20; // Increasing by 20 so clicks include option.
			text_options_rects.push_back(this_text_bounds);
		}
		value = default_value;
		title = new_title;
		
		left = new_left;
		top = new_top;

		panel_id = new_panel_id;
	}
};

#endif
