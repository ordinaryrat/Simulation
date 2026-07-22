#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Map.h"
#include <iostream>
#include <cstdint>
#include <map>

/*
std::map<terrain, uint8_t[]> terrain_colors = {
	{OCEAN, {0, 255, 0}},
};
*/

int main(int argc, char** argv) {
	const uint16_t width = 500;
	const uint16_t height = 500;
	
	std::srand(std::time({}));


	sf::RenderWindow window(sf::VideoMode({width, height}), "Simulator Game");
	window.setFramerateLimit(60);
	
	std::vector<uint8_t> pixels(width * height * 4);

	Map game_map(width, height);

	for (uint32_t i = 0; i < width * height; i += 1) {
		if (game_map.terrain_map[i] == OCEAN) {
			pixels[(i * 4) + 0] = 0;	
			pixels[(i * 4) + 1] = 0;	
			pixels[(i * 4) + 2] = 255;
		} 
		else if (game_map.terrain_map[i] == SEA) {
			pixels[(i * 4) + 0] = 40;	
			pixels[(i * 4) + 1] = 40;	
			pixels[(i * 4) + 2] = 200;
		}
		else if (game_map.terrain_map[i] == COASTAL) {
			pixels[(i * 4) + 0] = 80;	
			pixels[(i * 4) + 1] = 80;	
			pixels[(i * 4) + 2] = 200;
		}
		else if (game_map.terrain_map[i] == DESERT) {
			pixels[(i * 4) + 0] = 200;	
			pixels[(i * 4) + 1] = 200;	
			pixels[(i * 4) + 2] = 30;
		}
		pixels[(i * 4) + 3] = 255;
	}

	sf::Texture texture(sf::Vector2u(width, height));

	texture.update(pixels.data());
	sf::Sprite map(texture);
	map.setPosition({0, 0});
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
		}


        window.clear();
        
		window.draw(map);
		window.display();
	}
	
	return 0;
}
