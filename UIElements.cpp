#include "UIElements.h"

SelectionPanel::SelectionPanel(float new_left, float new_top, std::string new_title, std::vector<std::string> input_options, sf::Font &used_font, int new_font_size, selection_panel_id new_panel_id, PanelType new_panel_type, uint8_t default_value) {
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

		if (panel_type == MULTICHOICE)
			values[i] = default_value == i;
			
	}
	value = default_value;
	
	left = new_left;
	top = new_top;

	panel_id = new_panel_id;
	panel_rect = sf::FloatRect({left, top}, {0, 0});
}
