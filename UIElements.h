#ifndef UI_ELEMENTS_H
#define UI_ELEMENTS_H

#include <SFML/Graphics.hpp>
#include <string>
#include <tuple>

enum ButtonID {
	ADD_TO_MAP,

};

enum PanelType {
	RADIO,
	MULTICHOICE,
};

enum selection_panel_id {
	MAP_TYPE,
	MAP_LAYER,
	MOD_CIV,
	MOD_TERRAIN,
};

enum InputFieldType {
	STRING,
	INTEGER,
	COLOR	
};

struct Button {
	sf::Sprite* button_sprite;
	sf::FloatRect* button_collision_box;
	float left = 0;
	float top = 0;
	
	sf::Vector2u button_size;
	
	ButtonID button_id;
	
	void setPosition(float input_left, float input_top) {
		left = input_left;
		top = input_top;
	
		button_collision_box = new sf::FloatRect({left, top}, {(float)button_size.x, (float)button_size.y});
	}

	Button(sf::Texture& button_texture, ButtonID input_button_id) {
		button_sprite = new sf::Sprite(button_texture);
		button_size = button_texture.getSize();
		button_collision_box = new sf::FloatRect({left, top}, {(float)button_size.x, (float)button_size.y});
	}

	Button(float input_left, float input_top, sf::Texture& button_texture, ButtonID input_button_id) {
		left = input_left;
		top = input_top;

		button_id = input_button_id;

		button_sprite = new sf::Sprite(button_texture);
		button_size = button_texture.getSize();
		button_collision_box = new sf::FloatRect({left, top}, {(float)button_size.x, (float)button_size.y});
	}
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

	SelectionPanel(float new_left, float new_top, std::string new_title, std::vector<std::string> input_options, sf::Font &used_font, int new_font_size, selection_panel_id new_panel_id, PanelType new_panel_type, int16_t default_value = 0); 
};

struct InputField {
	float left;
	float top;
	float width;
	float height;

	sf::Font* font;
	uint8_t font_size;

	InputFieldType input_field_type;
	bool selected = false;
	
	uint16_t modify_position = 0;

	std::string value;

	std::string title;
	sf::Text* title_text;
	
	sf::FloatRect input_field_rect;
	
	sf::Text* input_text;
	sf::RectangleShape* input_field;
	
	InputField(sf::Font& used_font, float new_left, float new_top, float new_width, float new_height, std::string field_name, InputFieldType input_field_type, std::string default_value = "", uint8_t new_font_size = 18);
	
	void setInputText(); // Sets input text to value.
	std::tuple<uint8_t, uint8_t, uint8_t>* getColorValue(); // Gets current color value and returns.
	void formatInput(); // Checks if user input is valid and then can modify.
	uint32_t getNumber();
};

#endif
