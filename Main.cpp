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


int main(int argc, char** argv) {
	const uint16_t width = 500;
	const uint16_t height = 500;
	
	std::srand(std::time({}));
	sf::Font font("arial.ttf");

	sf::RenderWindow window(sf::VideoMode({width, height + 100}), "Simulator Game");
	window.setFramerateLimit(30);
	
	std::vector<uint8_t> pixels(width * height * 4);

	Map game_map(width, height);

	for (uint32_t i = 0; i < width * height; i += 1) {
		// std::cout << game_map.terrain_map[i] << std::endl;
		std::tuple<uint8_t, uint8_t, uint8_t> this_color = terrain_colors[game_map.terrain_map[i]];
		pixels[(i * 4) + 0] = std::get<0>(this_color);
		pixels[(i * 4) + 1] = std::get<1>(this_color);
		pixels[(i * 4) + 2] = std::get<2>(this_color);
		pixels[(i * 4) + 3] = 255;
	}

	sf::Texture texture(sf::Vector2u(width, height));

	texture.update(pixels.data());
	sf::Sprite map(texture);
	map.setPosition({0, 0});
	sf::FloatRect map_bounds = map.getGlobalBounds();
   	sf::Text bottom_text(font, "text", 24);
	bottom_text.setPosition({10, height + 10});

	while (window.isOpen()) {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
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
				
				sf::Vector2f map_top_left = map.getPosition();
			
				int pos_x = mouse_position.x - (int)map_top_left.x;
				int pos_y = mouse_position.y - (int)map_top_left.y;
				int i = (pos_y * width) + pos_x;
				bottom_text.setString("(" + std::to_string(pos_x) + ", " + std::to_string(pos_y) + ")" + " Terrain: " + terrain_names[game_map.terrain_map[i]]);
			}
		}


        window.clear();
        
		window.draw(map);
		window.draw(bottom_text);
		window.display();
	}
	
	return 0;
}
