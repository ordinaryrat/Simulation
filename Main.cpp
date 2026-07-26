#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Map.h"
#include "UIElements.h"
#include <iostream>
#include <cstdint>
#include <map>
#include <tuple>
#include <string>

#include <vector>

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
sf::Texture base_map_texture(sf::Vector2u(map_width, map_height));

Map game_map(map_width, map_height);

bool right_panel_on = false;

// We will probably need to seperate different pixel layers. Like one called base map and then a map for others.

void loadBaseMap(map_type base_map_type = TERRAIN) {
	// Loading bottom map component.
	base_map_pixels = {};
	for (uint32_t i = 0; i < map_width * map_height; i += 1) {
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

void clickOnPanel(selection_panel_id panel_id, uint8_t option_pressed) {
	switch (panel_id) {
		case (MAP_TYPE):
			loadBaseMap(static_cast<map_type>(option_pressed)); // Corresponds to enum;
			base_map_texture.update(base_map_pixels.data());
	}
}

std::vector<SelectionPanel> selection_panels = {};

sf::CircleShape selected_option_circle(5.f);
sf::CircleShape unselected_option_circle(5.f);

int main(int argc, char** argv) {
	std::srand(std::time({}));
	sf::Font font("media/arial.ttf");
	
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
	sf::Sprite base_map(base_map_texture);
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
	sf::RectangleShape bottom_panel({(float) width, 100.f}); 
	bottom_panel.setFillColor(sf::Color(100,100,100));
	bottom_panel.setOutlineColor(sf::Color(200, 200, 200));
	bottom_panel.setOutlineThickness(3.f);
	bottom_panel.setPosition({0.f, height - 100.f});

	sf::RectangleShape left_panel({200.f, (float) height - 150}); 
	left_panel.setFillColor(sf::Color(100,100,100));
	left_panel.setOutlineColor(sf::Color(200, 200, 200));
	left_panel.setOutlineThickness(3.f);
	left_panel.setPosition({0.f, 0.f});

	sf::RectangleShape right_panel({150.f, height - 150.f}); 
	right_panel.setFillColor(sf::Color(100,100,100));
	right_panel.setOutlineColor(sf::Color(200, 200, 200));
	right_panel.setOutlineThickness(3.f);
	right_panel.setPosition({width - 150.f, 0.f});

	// Will need to be more flexible in the future. 
	SelectionPanel left_panel_selection(30, 30, "Map Type", {"Terrain", "Land", "Temperature", "Precipitation", "Elevation"}, font, 20, MAP_TYPE);		
	selection_panels.push_back(left_panel_selection);

	while (window.isOpen()) {
		sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
        
		while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
			else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        	{
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
					window.close();
			}
			else if (const auto* resized = event->getIf<sf::Event::Resized>())
			{
				sf::Vector2u new_size = resized->size;
				width = new_size.x;
				height = new_size.y;

				view1.setSize({(float)new_size.x, (float)new_size.y}); 
				
				ui_view.setSize({(float)new_size.x, (float)new_size.y});
				ui_view.setCenter({new_size.x/2.f, new_size.y/2.f});
				
				bottom_panel.setSize({(float) width, 100.f});
				bottom_panel.setPosition({0.f, 0.f});
				
				left_panel.setSize({200.f, height - 150.f});
				left_panel.setPosition({0.f, 0.f});
				
				right_panel.setSize({150.f, height - 150.f});
				right_panel.setPosition({(float) width - 150.f, 0.f});
				
				bottom_text.setPosition({width - 140.f, 50.f});
			}
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				if (mouse_position.x < 200.f) {
					// Left Panel
					// This will need to be made more consistent tomorrow. Maybe each selection panel has its own float rect for its bounds and it is seen if mouse is touching it? These might not only be on the left panel.
					for (uint8_t this_panel_i = 0; this_panel_i < selection_panels.size(); this_panel_i++) {
						for (uint8_t i = 0; i < selection_panels[this_panel_i].text_options_rects.size(); i++) {
							if (selection_panels[this_panel_i].text_options_rects[i].contains({(float) mouse_position.x, (float) mouse_position.y})) {
								selection_panels[this_panel_i].value = i;
								clickOnPanel(selection_panels[this_panel_i].panel_id, i);
								break;
							}
						}
					}
				} else {
					
					sf::Vector2f world_pos = window.mapPixelToCoords(mouse_position, view1);
					if (world_pos.x < 0 || world_pos.x > map_width || world_pos.y < 0 || world_pos.y > map_height)
						continue;
					int i = ((int)world_pos.y * map_width) + (int)world_pos.x;
					bottom_text.setString("(" + std::to_string((int)world_pos.x) + ", " + std::to_string((int)world_pos.y) + ")" + "\nTerrain: " + terrain_names[game_map.terrain_map[i]]);
					right_panel_on = true;
				}
			}
			else if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>())
			{
				if (!map_bounds.contains({(float)mouse_position.x, (float)mouse_position.y}))
					continue;
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

        window.clear();
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
		
		window.setView(view1);
		window.draw(base_map);

		window.setView(ui_view);
		window.draw(left_panel);
		
		for (uint8_t this_panel_i = 0; this_panel_i < selection_panels.size(); this_panel_i++) {
			SelectionPanel* this_panel = &selection_panels[this_panel_i];
			
			float current_left = this_panel->left;
			float current_top = this_panel->top;

			for (uint8_t i = 0; i < this_panel->text_options_sprites.size(); i++) {
				this_panel->text_options_sprites[i].setPosition({current_left + 20, current_top});
				this_panel->text_options_rects[i].position = {current_left, current_top};
				
				float button_offset = this_panel->text_options_rects[i].size.y/2;

				if (this_panel->value == i) {
					selected_option_circle.setPosition({current_left, current_top + button_offset});	
					window.draw(selected_option_circle);
				} else {
					unselected_option_circle.setPosition({current_left, current_top + button_offset});	
					window.draw(unselected_option_circle);
				}
				
				window.draw(this_panel->text_options_sprites[i]);
				current_top += 30;
			}
		}

		if (right_panel_on) {
			window.draw(right_panel);
			window.draw(bottom_text);
		}

		for (uint8_t i = 0; i < bottom_panel_buttons.size(); i++) {
			bottom_panel_buttons[i].button_sprite->setPosition({bottom_panel_buttons[i].pos_x, bottom_panel_buttons[i].pos_y});
			window.draw(*(bottom_panel_buttons[i].button_sprite));
		
			if (bottom_panel_buttons[i].button_collision_box->contains({(float)mouse_position.x, (float)mouse_position.y})) {
				button_hovered_overlay.setPosition({bottom_panel_buttons[i].pos_x, bottom_panel_buttons[i].pos_y});
				window.draw(button_hovered_overlay);	
			}
		}
		
		window.display();
	}
	
	return 0;
}
