#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Map.h"
#include <iostream>
#include <cstdint>
#include <map>
#include <tuple>
#include <string>

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

float INITIAL_SPEED = 5;
float ACCEL_RATE = 0.25;

int main(int argc, char** argv) {
	uint16_t width = 600;
	uint16_t height = 600;

	uint16_t map_width = 700;
	uint16_t map_height = 700;
	
	std::srand(std::time({}));
	sf::Font font("arial.ttf");

	sf::RenderWindow window(sf::VideoMode({width, height}), "Simulator Game");
	window.setFramerateLimit(30);
	
	std::vector<uint8_t> pixels(map_width * map_height * 4);

	Map game_map(map_width, map_height);
	
	sf::View view1({map_width/2.f, map_height/2.f}, {(float)map_width, (float)map_height});
	sf::View ui_view(sf::FloatRect({0.f, 0.f}, {(float)width, (float)height}));
	
	for (uint32_t i = 0; i < map_width * map_height; i += 1) {
		// std::cout << game_map.terrain_map[i] << std::endl;
		std::tuple<uint8_t, uint8_t, uint8_t> this_color = terrain_colors[game_map.terrain_map[i]];
		pixels[(i * 4) + 0] = std::get<0>(this_color);
		pixels[(i * 4) + 1] = std::get<1>(this_color);
		pixels[(i * 4) + 2] = std::get<2>(this_color);
		pixels[(i * 4) + 3] = 255;
	}

	sf::Texture texture(sf::Vector2u(map_width, map_height));

	texture.update(pixels.data());
	sf::Sprite map(texture);
	map.setPosition({0, 0});
	sf::FloatRect map_bounds = map.getGlobalBounds();
   	sf::Text bottom_text(font, "text", 24);
	bottom_text.setPosition({0, height - 100.f});

	float view_zoom_accel = 0.f;
	float map_x_accel = 0.f;
	float map_y_accel = 0.f;
	view1.zoom(1.f);

	while (window.isOpen()) {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
			else if (const auto* resized = event->getIf<sf::Event::Resized>())
			{
				sf::Vector2u new_size = resized->size;
				width = new_size.x;
				height = new_size.y;

				view1.setSize({(float)new_size.x, (float)new_size.y}); 
				
				ui_view.setSize({(float)new_size.x, (float)new_size.y});
				ui_view.setCenter({new_size.x/2.f, new_size.y/2.f});
				bottom_text.setPosition({0, (float)new_size.y - 100.f});
			}
			else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        	{
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
					window.close();
			}
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
				if (!map_bounds.contains({(float)mouse_position.x, (float)mouse_position.y}))
					continue;
				sf::Vector2f world_pos = window.mapPixelToCoords(mouse_position, view1);
				if (world_pos.x < 0 || world_pos.x > map_width || world_pos.y < 0 || world_pos.y > map_height)
					continue;
				int i = ((int)world_pos.y * map_width) + (int)world_pos.x;
				bottom_text.setString("(" + std::to_string((int)world_pos.x) + ", " + std::to_string((int)world_pos.y) + ")" + " Terrain: " + terrain_names[game_map.terrain_map[i]]);
			}
			else if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>())
			{
				sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
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
			view1.move({map_x_accel, 0.f});
		}
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A))
		{
			if (map_x_accel > 0)
				map_x_accel = -INITIAL_SPEED;
			map_x_accel -= ACCEL_RATE;
			view1.move({map_x_accel, 0.f});
		}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W))
		{
			if (map_y_accel > 0)
				map_y_accel = -INITIAL_SPEED;
			map_y_accel -= ACCEL_RATE;
			view1.move({0.f, map_y_accel});
		}
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S))
		{
			if (map_y_accel < 0)
				map_y_accel = INITIAL_SPEED;
			map_y_accel += ACCEL_RATE;
			view1.move({0.f, map_y_accel});
		}
		
		window.setView(view1);
		window.draw(map);

		window.setView(ui_view);
		window.draw(bottom_text);
		
		window.display();
	}
	
	return 0;
}
