#include "UIElements.h"
#include <cstring>

SelectionPanel::SelectionPanel(float new_left, float new_top, std::string new_title, std::vector<std::string> input_options, sf::Font &used_font, int new_font_size, selection_panel_id new_panel_id, PanelType new_panel_type, int16_t default_value) {
	text_options = input_options;
	text_options_sprites = new sf::Text*[input_options.size() + 1];
	
	panel_type = new_panel_type;
	if (panel_type == MULTICHOICE)
		values = new bool[text_options.size()];
	

	font_size = new_font_size;
	title = new_title;
		
	sf::Text* title_text = new sf::Text(used_font, title, (int)(font_size*1.2));
	title_text->setStyle(sf::Text::Bold | sf::Text::Underlined);

	text_options_sprites[0] = title_text;
	sf::FloatRect this_text_bounds = title_text->getGlobalBounds();
	text_options_rects.push_back(this_text_bounds);
	
	
	for (uint8_t i = 0; i < text_options.size(); i++) {
		sf::Text* this_text = new sf::Text(used_font, text_options[i], font_size);
		text_options_sprites[i + 1] = this_text;
		
		sf::FloatRect this_text_bounds = this_text->getGlobalBounds();
		this_text_bounds.size.x += 20; // Increasing by 20 so clicks include option.
		// this_text_bounds.size.y += 5;
		// this_text_bounds.position.y -= 2.5;
		text_options_rects.push_back(this_text_bounds);

		if (panel_type == MULTICHOICE) {
			if (default_value != -1)
				values[i] = default_value == i;
			else
				values[i] = false;
		}
	}
	if (panel_type == RADIO)
		value = default_value;
	
	left = new_left;
	top = new_top;

	panel_id = new_panel_id;
	panel_rect = sf::FloatRect({left, top}, {0, 0});
}

InputField::InputField(sf::Font& used_font, float new_left, float new_top, float new_width, float new_height, std::string field_name, InputFieldType field_type, std::string default_value, uint8_t new_font_size) {
	left = new_left;
	top = new_top;
	width = new_width;
	height = new_height;
	value = default_value;
	input_field_type = field_type;
	
	font = &used_font;
	font_size = new_font_size;
	
	title = field_name;
	title_text = new sf::Text(used_font, title, (int) font_size * 1.25);

	input_text = new sf::Text(used_font, value, font_size);
	input_field = new sf::RectangleShape({width, height});
	input_field->setPosition({left, top});
	input_field->setOutlineColor({25, 25, 25, 255});
	input_field->setOutlineThickness(1);
	input_field->setFillColor({100, 100, 100, 255});

	input_field_rect = input_field->getGlobalBounds();

	modify_position = value.size();
}

void InputField::setInputText() {
	input_text->setString(value);
}

std::tuple<uint8_t, uint8_t, uint8_t>* InputField::getColorValue() {
	std::tuple<uint8_t, uint8_t, uint8_t>* return_value = new std::tuple<uint8_t, uint8_t, uint8_t>(0, 0, 0);
	for (uint8_t i = 0; i < 3; i++) {
		uint8_t found_value = 0;
		if (value[(i * 2) + 1] > 64)
			found_value += 16 * (10 + (value[(i * 2) + 1] - 65)); 
		else 
			found_value += 16 * (value[(i * 2) + 1] - 48);
		
		if (value[(i * 2) + 2] > 64)
			found_value += (10 + (value[(i * 2) + 2] - 65)); 
		else 
			found_value += (value[(i * 2) + 2] - 48); 
		
		switch (i) {
			case 0:
				std::get<0>(*return_value) = found_value;
				break;
			case 1:
				std::get<1>(*return_value) = found_value;
				break;
			case 2:
				std::get<2>(*return_value) = found_value;
				break;
		}
	}

	return return_value;
}
void InputField::formatInput() {
	if (input_field_type == COLOR) {
		if (value.length() > 7)
			value = value.substr(0, 7);
		while (value.length() < 7)
			value += '0';
		if (value[0] != '#')
			value[0] = '#';
		for (uint8_t i = 1; i < 7; i++) {
			if (value[i] == '#')
				value[i] = '0';
		}
	}
}

uint32_t InputField::getNumber() {
	uint32_t return_value = 0;
	for (uint16_t i = 0; i < value.length(); i++) {
		uint32_t base = 1;
		for (uint16_t b = 1; b < value.length() - i; b++) {
			base *= 10;
		}
		return_value += base * (value[i] - 48);
	}
	return return_value;
}
Page::Page(sf::Font& used_font, float new_left, float new_top, float new_width, float new_height, std::string new_title, PageType set_type, uint8_t new_font_size, Tile* set_tile) {
	left = new_left;
	top = new_top;
	width = new_width;
	height = new_height;
	
	font = &used_font;
	font_size = new_font_size;

	type = set_type;

	tile = set_tile;
	
	title_text = new_title;
	title = new sf::Text(used_font, title_text, (uint8_t)(font_size*1.5));
	title->setPosition({left + 3.f, top - font_size*2.f});

	page_sprite = new sf::RectangleShape({(float)width, (float)height});
	page_sprite->setPosition({(float)left, (float)top});
	page_sprite->setOutlineColor({25, 25, 25, 255});
	page_sprite->setOutlineThickness(1);
	page_sprite->setFillColor({140, 140, 140, 255});
	
	top_sprite = new sf::RectangleShape({(float)width, font_size*2.f});
	top_sprite->setPosition({(float)left, top - font_size*2.f});
	top_sprite->setOutlineColor({25, 25, 25, 255});
	top_sprite->setOutlineThickness(1);
	top_sprite->setFillColor({80, 80, 80, 255});

	page_rect = page_sprite->getGlobalBounds();
	top_rect = top_sprite->getGlobalBounds();
}
void Page::changePosition(sf::Vector2i change_position, uint32_t current_page_anchor_x, uint32_t current_page_anchor_y) {
	left = change_position.x + current_page_anchor_x;
	top = change_position.y + current_page_anchor_y + font_size*2;

	page_rect.position.x = (float)left;
	page_rect.position.y = (float)top;
	top_rect.position.x = (float)left;
	top_rect.position.y = top - font_size*2.f;

	title->setPosition({left + 3.f, top - font_size*2.f});
	
	page_sprite->setPosition({(float)left, (float)top});
	top_sprite->setPosition({(float)left, top - font_size*2.f});

	for (uint16_t i = 0; i < text_sprites.size(); i++) {
		std::get<0>(text_sprites[i])->setPosition({(float)(left + std::get<1>(text_sprites[i])), (float)(top + std::get<2>(text_sprites[i]))});
	}
}
void Page::addTextString(std::string text_string, uint32_t set_x, uint32_t set_y) {
	sf::Text* new_text = new sf::Text(*font, text_string, font_size);
	new_text->setPosition({(float)(left + set_x), (float)(top + set_y)});

	text_sprites.push_back({new_text, set_x, set_y});
}
void Page::updateTextString(std::string new_text_string, uint16_t index) {
	std::get<0>(text_sprites[index])->setString(new_text_string);
}
