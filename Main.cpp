#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cstdint>
#include <map>
#include <tuple>
#include <string>
#include <vector>

#include "Map.h"
#include "UIElements.h"
#include "World.h"
#include "Civilization.h"
#include "Event.h"
	
sf::Font font("media/arial.ttf");

std::map<terrain, std::tuple<uint8_t, uint8_t, uint8_t>> terrain_colors = {
	{OCEAN, std::tuple(0, 0, 255)},
	{SEA, std::tuple(10, 40, 200)},
	{COASTAL, std::tuple(40, 80, 160)},
	{DESERT, std::tuple(240, 240, 130)},
	{PLAINS, std::tuple(100, 210, 40)},
	{GRASSLAND, std::tuple(10, 230, 35)},
	{MOUNTAIN, std::tuple(10, 10, 10)},
	{JUNGLE, std::tuple(10, 255, 5)},
	{ARCTIC, std::tuple(200, 200, 200)},
	{ICE, std::tuple(220, 220, 250)},
	{TUNDRA, std::tuple(184, 170, 138)},
};
std::map<terrain, std::string> terrain_names = {
	{OCEAN, "Ocean"},
	{SEA, "Sea"},
	{COASTAL, "Coastal"},
	{DESERT, "Desert"},
	{PLAINS, "Plains"},
	{GRASSLAND, "Grassland"},
	{MOUNTAIN, "Mountain"},
	{JUNGLE, "Jungle"},
	{ARCTIC, "Arctic"},
	{ICE, "Ice"},
	{TUNDRA, "Tundra"},
};

enum map_type {
	TERRAIN,
	LAND,
	TEMPERATURE,
	PRECIPITATION,
	ELEVATION,
};

enum layer_type {
	NONE,
	CULTURES,
	CIVILIZATIONS,
	RELIGIONS,
	GOVERNMENTS,
};

bool rectContains(sf::FloatRect& rect, sf::Vector2i& pos, int padding = 4) {
	if (rect.position.x - padding >= pos.x)
		return false;
	if (rect.position.y - padding >= pos.y)
		return false;
	if (rect.position.x + rect.size.x + padding <= pos.x)
		return false;
	if (rect.position.y + rect.size.y + padding <= pos.y)
		return false;
	return true;
}

std::string formatSigFigs(std::string input) {
	std::string output = input;
	while (output.size() > 0) {
		switch (*(output.end() - 1)) {
			case '0':
				output.erase(output.end() - 1);
				break;
			case '.': 
				output.erase(output.end() - 1);
				return output;
			default:	
				return output;
		}
	}
	return "";
}

std::string months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

float INITIAL_SPEED = 5;
float ACCEL_RATE = 1;
float MAX_SPEED = 10;

sf::Texture button_hovered_overlay_texture("media/ButtonOverlay.png");
sf::Sprite button_hovered_overlay(button_hovered_overlay_texture);


sf::Texture terrain_section_button_texture("media/TerrainButton.png");
sf::Texture default_button_texture("media/TemplateButton.png");
sf::Texture back_button_texture("media/TemplateButton.png");

sf::Texture terrain_heatmap_button_texture("media/TerrainHeatButton.png");
sf::Texture terrain_landmap_button_texture("media/TerrainLandButton.png");
sf::Texture terrain_rainfallmap_button_texture("media/TerrainRainfallButton.png");
sf::Texture terrain_terrainmap_button_texture("media/TerrainTerrainButton.png");

Button terrain_section_button(terrain_section_button_texture, TERRAIN_SECTION);
Button societies_section_button(default_button_texture, TERRAIN_SECTION);
Button back_button(back_button_texture, BACK);

Button heatmap_button(terrain_heatmap_button_texture, HEATMAP);
Button landmap_button(terrain_landmap_button_texture, LANDMAP);
Button rainfallmap_button(terrain_rainfallmap_button_texture, RAINFALLMAP);
Button terrainmap_button(terrain_rainfallmap_button_texture, TERRAINMAP);

uint16_t width = 600;
uint16_t height = 600;

uint16_t map_width = 700;
uint16_t map_height = 700;

std::vector<Button> bottom_panel_buttons;
std::vector<uint8_t> base_map_pixels(map_width * map_height * 4);
std::vector<uint8_t> top_layer_pixels(map_width * map_height * 4);
sf::Texture base_map_texture(sf::Vector2u(map_width, map_height));
sf::Texture top_layer_texture(sf::Vector2u(map_width, map_height));

Map game_map(map_width, map_height);
World game_world(game_map);

Civilization new_civ("rat", {255, 0, 0});
Civilization new_civ2("bat", {255, 255, 0});

bool right_panel_on = false;
	
sf::Sprite base_map(base_map_texture);
sf::Sprite top_layer_map(top_layer_texture);

// We will probably need to seperate different pixel layers. Like one called base map and then a map for others.

void loadBaseMap(map_type base_map_type = TERRAIN) {
	// Loading bottom map component.
	base_map_pixels = {};
	for (uint32_t i = 0; i < map_width * map_height; i++) {
		std::tuple<uint8_t, uint8_t, uint8_t> this_color;
		switch (base_map_type) {
			case TERRAIN:
				this_color = terrain_colors[game_map.terrain_map[i]];
				break;
			case LAND:
				if (game_map.land_map[i])
					this_color = {200, 200, 0};
				else
					this_color = {0, 0, 200};
				break;
			case TEMPERATURE: {
				// Maybe a key for colors would be nice.
				float this_temp = game_map.temperature_map[i]; 
				float r = (255 * (this_temp + 100.f) * 0.004f);
				float b = (255 * (1 - (this_temp + 100.f) * 0.004f));
				
				if (r > 255)
					r = 255.f;
				else if (r < 0)
					r = 0.f;
	
				if (b > 255)
					b = 255.f;
				else if (b < 0)
					b = 0.f;
			
				this_color = {(int)r, 0, (int)b};
				break;
			}
			case PRECIPITATION:
				this_color = {0, 0, (int) (game_map.precipitation_map[i] * 255.f)};
				break;
			case ELEVATION:
				this_color = {(int) (game_map.elevation_map[i] * 255.f), (int) (game_map.elevation_map[i] * 255.f), (int) (game_map.elevation_map[i] * 255.f)};
				break;
		}

		base_map_pixels[(i * 4) + 0] = std::get<0>(this_color);
		base_map_pixels[(i * 4) + 1] = std::get<1>(this_color);
		base_map_pixels[(i * 4) + 2] = std::get<2>(this_color);
		base_map_pixels[(i * 4) + 3] = 255;
	}
}

void loadTopLayer(layer_type layer = CIVILIZATIONS) {
	// Loading top layer map component. Note that live changes should not be processed to the list through here.
	if (layer == NONE) {
		top_layer_pixels.assign(top_layer_pixels.size(), 0);
		return;
	}
	switch (layer) {
		case CIVILIZATIONS: {
			top_layer_pixels.assign(top_layer_pixels.size(), 0);
			for (uint16_t i = 0; i < game_world.civilizations.size(); i++) {
				Civilization* this_civ = &game_world.civilizations[i];
				for (uint32_t owned_tile : this_civ->owned_tiles) {
					top_layer_pixels[(owned_tile * 4) + 0] = std::get<0>(this_civ->color);
					top_layer_pixels[(owned_tile * 4) + 1] = std::get<1>(this_civ->color);
					top_layer_pixels[(owned_tile * 4) + 2] = std::get<2>(this_civ->color);
					top_layer_pixels[(owned_tile * 4) + 3] = 160;
				}
			}
		}
	}
}

void loadMainButtons() {
	terrain_section_button.setPosition(10.f, (float)height - 70);
	societies_section_button.setPosition(110.f, (float)height - 70);

	bottom_panel_buttons = {};
	bottom_panel_buttons.push_back(terrain_section_button);
	bottom_panel_buttons.push_back(societies_section_button);
}

void loadTerrainSectionButtons() {
	back_button.setPosition(10.f, (float)height - 70);
	terrainmap_button.setPosition(110.f, (float)height - 70);
	landmap_button.setPosition(210.f, (float)height - 70);
	heatmap_button.setPosition(310.f, (float)height - 70);
	rainfallmap_button.setPosition(410.f, (float)height - 70);

	bottom_panel_buttons = {};
	bottom_panel_buttons.push_back(back_button);
	bottom_panel_buttons.push_back(terrainmap_button);
	bottom_panel_buttons.push_back(landmap_button);
	bottom_panel_buttons.push_back(heatmap_button);
	bottom_panel_buttons.push_back(rainfallmap_button);
}

void clickButton(ButtonID button_id) {
	switch (button_id) {
		case (TERRAIN_SECTION):
			loadTerrainSectionButtons();
			break;
		case (BACK):
			loadMainButtons();
			break;
	}
}

uint8_t current_map_view = 0;
uint8_t current_top_layer_view = 2;

std::vector<SelectionPanel> selection_panels = {};

SelectionPanel modification_civ_panel(30, height - 140, "Modify Civilizations", {"Add Civilization", "Paint Borders", "Modify Civilization", "Remove Civilization"}, font, 20, MOD_CIV, RADIO);
SelectionPanel modification_terrain_panel(30, height - 140, "Modify Terrain", {"Draw Land", "Modify Elevation", "Modify Heat", "Modify Precipitation"}, font, 20, MOD_TERRAIN, RADIO);

void addPanel(std::vector<SelectionPanel>& selection_panels, SelectionPanel& new_panel) {
	for (uint8_t i = 0; i < selection_panels.size(); i++) {
		// This function being called implies a chnage so we are fine just deleting stuff.
		if (selection_panels[i].title == "Modify Civilizations" || selection_panels[i].title == "Modify Terrain") {
			selection_panels.erase(selection_panels.begin() + i);
			i--;
		}
	}
	// Note that this is a different object with a different memory address. May be a problem TODO.
	new_panel.top = height - 140;
	selection_panels.push_back(new_panel);
}

void clickOnPanel(selection_panel_id panel_id, uint8_t option_pressed) {
	switch (panel_id) {
		case (MAP_TYPE):
			loadBaseMap(static_cast<map_type>(option_pressed)); // Corresponds to enum;
			base_map_texture.update(base_map_pixels.data());
			current_map_view = option_pressed;
			break;
		case (MAP_LAYER):
			loadTopLayer(static_cast<layer_type>(option_pressed)); // Corresponds to enum;
			top_layer_texture.update(top_layer_pixels.data());
			current_top_layer_view = option_pressed;
			switch (static_cast<layer_type>(option_pressed)) {
				case NONE:
					addPanel(selection_panels, modification_terrain_panel);
					break;
				case CIVILIZATIONS:
					addPanel(selection_panels, modification_civ_panel);
					break;
			}
			break;
	}
}

sf::CircleShape selected_option_circle(5.f);
sf::CircleShape unselected_option_circle(5.f);

SelectionPanel* this_panel;

sf::Clock delta_clock;
float date_advance_time = 0.f;
float time_until_date_advance = 2.f;
bool paused = true;
bool active_window = true;


void processEvents(std::vector<GameEvent> events) {
	bool update_top_layer_texture = false;
	for (GameEvent event : events) {
		switch (event.type) {
			case (LAND_TAKEN): {
				if (current_top_layer_view == 2) {
					Civilization* related_civ = game_world.civ_look_up_table[event.related_civ_name];
					top_layer_pixels[(event.para1 * 4) + 0] = std::get<0>(related_civ->color);
					top_layer_pixels[(event.para1 * 4) + 1] = std::get<1>(related_civ->color);
					top_layer_pixels[(event.para1 * 4) + 2] = std::get<2>(related_civ->color);
					top_layer_pixels[(event.para1 * 4) + 3] = 160;
					update_top_layer_texture = true;
				}
			}
		}
	}
	if (update_top_layer_texture) {
		top_layer_texture.update(top_layer_pixels.data());
		top_layer_map.setTexture(top_layer_texture);
	}
}

int main(int argc, char** argv) {
	std::srand(std::time({}));
	
	// TODO REMOVE
	new_civ.owned_tiles.push_back(10000);
	new_civ2.owned_tiles.push_back(50000);
	game_world.civilizations.push_back(new_civ);
	game_world.civilizations.push_back(new_civ2);
	game_world.civ_look_up_table.insert({"rat", &new_civ});
	game_world.civ_look_up_table.insert({"bat", &new_civ2});

	selected_option_circle.setFillColor({0, 0, 200});
	unselected_option_circle.setFillColor({100, 100, 100});
	selected_option_circle.setOutlineColor({50, 50, 50});
	unselected_option_circle.setOutlineColor({50, 50, 50});
	selected_option_circle.setOutlineThickness(2.f);
	unselected_option_circle.setOutlineThickness(2.f);

	sf::RenderWindow window(sf::VideoMode({width, height}), "Simulator Game");
	window.setFramerateLimit(30);
	
	sf::View view1({map_width/2.f, map_height/2.f}, {(float)map_width, (float)map_height});
	sf::View ui_view(sf::FloatRect({0.f, 0.f}, {(float)width, (float)height}));
	
	// LOADING BASE MAP
	loadBaseMap(TERRAIN);

	base_map_texture.update(base_map_pixels.data());
	base_map.setTexture(base_map_texture);
	base_map.setPosition({0, 0});
	sf::FloatRect map_bounds = base_map.getGlobalBounds();

	float view_zoom_accel = 0.f;
	float map_x_accel = 0.f;
	float map_y_accel = 0.f;
	view1.zoom(1.f);
	
	// REMOVE AT SOME POINT
   	sf::Text bottom_text(font, "", 18);
	bottom_text.setPosition({width - 140.f, 50.f});
	
	// INITIALIZING PANELS
	sf::RectangleShape bottom_panel({(float) width, 150.f}); 
	bottom_panel.setFillColor(sf::Color(100,100,100));
	bottom_panel.setOutlineColor(sf::Color(200, 200, 200));
	bottom_panel.setOutlineThickness(3.f);
	bottom_panel.setPosition({0.f, height - 150.f});

	sf::RectangleShape left_panel({200.f, (float) height - 150}); 
	left_panel.setFillColor(sf::Color(100,100,100));
	left_panel.setOutlineColor(sf::Color(200, 200, 200));
	left_panel.setOutlineThickness(3.f);
	left_panel.setPosition({0.f, 0.f});

	sf::RectangleShape right_panel({200.f, height - 150.f}); 
	right_panel.setFillColor(sf::Color(100,100,100));
	right_panel.setOutlineColor(sf::Color(200, 200, 200));
	right_panel.setOutlineThickness(3.f);
	right_panel.setPosition({width - 200.f, 0.f});
	
	sf::RectangleShape date_panel({300.f, 30.f}); 
	date_panel.setFillColor(sf::Color(100,100,100));
	date_panel.setOutlineColor(sf::Color(200, 200, 200));
	date_panel.setOutlineThickness(3.f);
	date_panel.setPosition({width/2.f - 150.f, 0.f});
	
	sf::Text year_date_text(font, "", 18);
	sf::Text month_date_text(font, "", 18);
	sf::Text time_speed_text(font, "", 18);
	
	year_date_text.setPosition({width/2.f - 140.f, 10.f});
	month_date_text.setPosition({width/2.f - 70.f, 10.f});
	time_speed_text.setPosition({width/2.f + 10.f, 10.f});

	// Will need to be more flexible in the future. 
	SelectionPanel left_panel_selection(30, 30, "Map Type", {"Terrain", "Land", "Temperature", "Precipitation", "Elevation"}, font, 20, MAP_TYPE, RADIO);
	selection_panels.push_back(left_panel_selection);
	SelectionPanel civilization_selection(30, 220, "Base Layer", {"None", "Cultures", "Civilizations", "Religions", "Governments"}, font, 20, MAP_LAYER, RADIO, 2);		
	selection_panels.push_back(civilization_selection);
	selection_panels.push_back(modification_civ_panel);

	// LOADING CIV Layer Map 
	loadTopLayer(CIVILIZATIONS);

	top_layer_texture.update(top_layer_pixels.data());
	top_layer_map.setTexture(top_layer_texture);
	top_layer_map.setPosition({0, 0});

	while (window.isOpen()) {
		sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
        
		while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                window.close();
				return 0;
			} else if (event->is<sf::Event::FocusLost>()) {
				paused = true; // perhaps a setting for this?
				active_window = false;
			} else if (event->is<sf::Event::FocusGained>()) {
				active_window = true;
			} else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
					right_panel_on = false;
				} else if (keyPressed->scancode == sf::Keyboard::Scancode::Space) {
					paused = !paused;	
					date_advance_time = 0.f;
				} else if (keyPressed->scancode == sf::Keyboard::Scancode::P) {
					paused = true;	
					date_advance_time = 0.f;
				} else if (keyPressed->scancode == sf::Keyboard::Scancode::NumpadPlus || keyPressed->scancode == sf::Keyboard::Scancode::Equal) {
					time_until_date_advance /= 2;
				} else if (keyPressed->scancode == sf::Keyboard::Scancode::NumpadMinus || keyPressed->scancode == sf::Keyboard::Scancode::Hyphen) {
					time_until_date_advance *= 2;
				}
			}
			else if (const auto* resized = event->getIf<sf::Event::Resized>())
			{
				sf::Vector2u new_size = resized->size;
				width = new_size.x;
				height = new_size.y;

				view1.setSize({(float)new_size.x, (float)new_size.y}); 
				
				ui_view.setSize({(float)new_size.x, (float)new_size.y});
				ui_view.setCenter({new_size.x/2.f, new_size.y/2.f});
				
				bottom_panel.setSize({(float) width, 150.f});
				bottom_panel.setPosition({0.f, (float) height - 150.f});
				
				left_panel.setSize({200.f, height - 200.f});
				left_panel.setPosition({0.f, 0.f});
				
				right_panel.setSize({200.f, height - 200.f});
				right_panel.setPosition({(float) width - 200.f, 0.f});
				
				bottom_text.setPosition({width - 140.f, 50.f});
				
				date_panel.setPosition({width/2.f - 150.f, 0.f});
				
				year_date_text.setPosition({width/2.f - 140.f, 10.f});
				month_date_text.setPosition({width/2.f - 70.f, 10.f});
				time_speed_text.setPosition({width/2.f + 10.f, 10.f});
				
				for (uint8_t i = 0; i < selection_panels.size(); i++) {
					if (selection_panels[i].title == "Modify Civilizations" || selection_panels[i].title == "Modify Terrain") {
						SelectionPanel* this_panel = &selection_panels[i];
						this_panel->top = height - 140.f;
					}
				}
			}
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && active_window)
			{
				// Left Panel
				for (uint8_t this_panel_i = 0; this_panel_i < selection_panels.size(); this_panel_i++) {
					SelectionPanel* this_panel = &(selection_panels[this_panel_i]);
					if (rectContains(this_panel->panel_rect, mouse_position)) {
						for (uint8_t i = 1; i < this_panel->text_options_rects.size(); i++) {
							if (rectContains(this_panel->text_options_rects[i], mouse_position)) {
								if (this_panel->panel_type == RADIO) 
									this_panel->value = i - 1;
								else if (this_panel->panel_type == MULTICHOICE)
									this_panel->values[i - 1] = !this_panel->values[i - 1];
								clickOnPanel(this_panel->panel_id, i - 1);
								break;
							}
						}
					}
				}
				if (mouse_position.x > 200) {	
					sf::Vector2f world_pos = window.mapPixelToCoords(mouse_position, view1);
					if (world_pos.x < 0 || world_pos.x > map_width || world_pos.y < 0 || world_pos.y > map_height)
						continue;
					int i = ((int)world_pos.y * map_width) + (int)world_pos.x;
					std::string temp_build_string = "(" + std::to_string((int)world_pos.x) + ", " + std::to_string((int)world_pos.y) + ")" + "\nTerrain: " + terrain_names[game_map.terrain_map[i]];
					right_panel_on = true;
					
					// Because of the expensiveness of this operation, we will probably want to have a massive lookup table instead. If it just stores int and pointer maybe not that large.
					bool civ_found = false;
					for (uint16_t l = 0; l < game_world.civilizations.size() && !civ_found; l++) {
						Civilization* this_civ = &game_world.civilizations[l];
						for (uint32_t tile : this_civ->owned_tiles) {
							if (tile == i) {
								temp_build_string += "\nOwner: " + this_civ->name;
								break;
							}
						}
					}
					bottom_text.setString(temp_build_string);
				}
			}
			else if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>())
			{
				/*if rectContains(map_bounds, mouse_position) {
					continue;
				*/
				switch (mouseWheelScrolled->wheel)
				{
					case sf::Mouse::Wheel::Vertical:
						if (view_zoom_accel > 0 && mouseWheelScrolled->delta == 1 || view_zoom_accel < 0 && mouseWheelScrolled->delta == -1)
							view_zoom_accel = 0;
						view_zoom_accel += mouseWheelScrolled->delta/-100;
						view1.zoom(1 + view_zoom_accel);
						break;
					case sf::Mouse::Wheel::Horizontal:
						break;
				}
			}
		}
		sf::Time dt = delta_clock.restart();
		if (!paused)
			date_advance_time += dt.asSeconds();
		if (date_advance_time > time_until_date_advance) {
			processEvents(game_world.incrementDate());
			date_advance_time = 0.f;
		}

		year_date_text.setString("Year " + std::to_string(game_world.date[0]));	
		month_date_text.setString(months[game_world.date[1]]);	
		if (paused)
			time_speed_text.setString("Paused");	
		else
			time_speed_text.setString(formatSigFigs(std::to_string(2.f/time_until_date_advance)) + 'x');
        window.clear();
        if (active_window) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D))
			{
				if (map_x_accel < 0)
					map_x_accel = INITIAL_SPEED;
				map_x_accel += ACCEL_RATE;
				if (map_x_accel > MAX_SPEED)
					map_x_accel = MAX_SPEED;
				view1.move({map_x_accel, 0.f});
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A))
			{
				if (map_x_accel > 0)
					map_x_accel = -INITIAL_SPEED;
				map_x_accel -= ACCEL_RATE;
				if (-map_x_accel > MAX_SPEED)
					map_x_accel = -MAX_SPEED;
				view1.move({map_x_accel, 0.f});
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W))
			{
				if (map_y_accel > 0)
					map_y_accel = -INITIAL_SPEED;
				map_y_accel -= ACCEL_RATE;
				if (-map_y_accel > MAX_SPEED)
					map_y_accel = -MAX_SPEED;
				view1.move({0.f, map_y_accel});
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S))
			{
				if (map_y_accel < 0)
					map_y_accel = INITIAL_SPEED;
				map_y_accel += ACCEL_RATE;
				if (map_y_accel > MAX_SPEED)
					map_y_accel = MAX_SPEED;
				view1.move({0.f, map_y_accel});
			}
		}	
		window.setView(view1);
		window.draw(base_map);
		window.draw(top_layer_map);

		window.setView(ui_view);
		window.draw(left_panel);
		window.draw(date_panel);
		
		window.draw(year_date_text);
		window.draw(month_date_text);
		window.draw(time_speed_text);
		
		if (right_panel_on) {
			window.draw(right_panel);
			window.draw(bottom_text);
		}
		window.draw(bottom_panel);

		for (uint8_t this_panel_i = 0; this_panel_i < selection_panels.size(); this_panel_i++) {
			this_panel = &selection_panels[this_panel_i];
			
			float current_left = this_panel->left;
			float current_top = this_panel->top;
		
			float max_left = 0;
			
			this_panel->text_options_sprites[0]->setPosition({current_left, current_top});

			window.draw(*(this_panel->text_options_sprites[0]));
			current_top += this_panel->text_options_rects[0].size.y + 10;

			for (uint8_t i = 1; i < this_panel->text_options.size() + 1; i++) { // 0th text is the title.
				this_panel->text_options_sprites[i]->setPosition({current_left + 20, current_top});
				this_panel->text_options_rects[i].position = {current_left, current_top};
				
				float button_offset = this_panel->text_options_rects[i].size.y/2;
				
				if ((this_panel->panel_type == RADIO && this_panel->value == i - 1) || (this_panel->panel_type == MULTICHOICE && this_panel->values[i - 1])) {
					selected_option_circle.setPosition({current_left, current_top + button_offset});	
					window.draw(selected_option_circle);
				} else {
					unselected_option_circle.setPosition({current_left, current_top + button_offset});	
					window.draw(unselected_option_circle);
				}
				
				window.draw(*(this_panel->text_options_sprites[i]));
				current_top += this_panel->text_options_rects[i].size.y + 10;
				
				if (this_panel->text_options_rects[i].size.x + 25 > max_left)
					max_left = this_panel->text_options_rects[i].size.x + 25;
			}
			this_panel->panel_rect.size.x = max_left;
			this_panel->panel_rect.size.y = current_top - this_panel->top;
		}
		
		for (uint8_t i = 0; i < bottom_panel_buttons.size(); i++) {
			bottom_panel_buttons[i].button_sprite->setPosition({bottom_panel_buttons[i].pos_x, bottom_panel_buttons[i].pos_y});
			window.draw(*(bottom_panel_buttons[i].button_sprite));
		
			if (rectContains(*(bottom_panel_buttons[i].button_collision_box), mouse_position)) {
				button_hovered_overlay.setPosition({bottom_panel_buttons[i].pos_x, bottom_panel_buttons[i].pos_y});
				window.draw(button_hovered_overlay);	
			}
		}
		
		window.display();
	}
	
	return 0;
}
