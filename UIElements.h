#ifndef UI_ELEMENTS_H
#define UI_ELEMENTS_H

#include <SFML/Graphics.hpp>
#include <string>
#include <tuple>

#include "Tile.h"

enum Statistic {
	TILE_TEMP,
};

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

enum PageType {
	TILE_PAGE, // Maybe TILE_PAGE_RAW and TILE_PAGE to seperate civilization details about tile.
	CIV_PAGE,
	REGION_PAGE,
	CULTURE_PAGE,
	RELIGION_PAGE,
	CHARACTER_PAGE,
};

// This ends up just working better for single buttons rather than a group.
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

struct Page {
	// TODO this should be developed into the way pages are done.
	// If shift is done when clicking on a 'link' it makes a new page.
	std::vector<std::tuple<sf::Text*, uint32_t, uint32_t>> text_sprites;
	std::vector<InputField*> input_fields;
	std::vector<Button*> stat_buttons;

	std::string title_text;
	sf::Text* title;

	uint32_t left = 0;
	uint32_t top = 0;
	uint32_t width = 0;
	uint32_t height = 0;
	
	sf::Font* font;
	uint8_t font_size;
	
	sf::RectangleShape* page_sprite;
	sf::RectangleShape* top_sprite;

	sf::FloatRect top_rect;
	sf::FloatRect page_rect;
	
	PageType type;
	Tile* tile;

	void changePosition(sf::Vector2i change_position, uint32_t current_page_anchor_x, uint32_t current_page_anchor_y);
	void addTextString(std::string text_string, uint32_t set_x, uint32_t set_y);
	void updateTextString(std::string new_text_string, uint16_t index);

	Page(sf::Font& used_font, float new_left, float new_top, float new_width, float new_height, std::string new_title, PageType set_type, uint8_t new_font_size, Tile* set_tile);
};

#endif
