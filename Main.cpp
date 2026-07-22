#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Simplex.h"
#include <iostream>

int main(int argc, char** argv) {
	const uint16_t width = 500;
	const uint16_t height = 500;
	
	std::srand(std::time({}));


	sf::RenderWindow window(sf::VideoMode({width, height}), "Simulator Game");
	window.setFramerateLimit(60);
	
	std::vector<uint8_t> pixels(width * height * 4);

	Simplex simplex_grid(width, height, 0.05);
	simplex_grid.simplexNoise();

	for (uint32_t i = 0; i < width * height; i += 1) {
		pixels[(i * 4) + 0] = (int)(255 * simplex_grid.grid[i]);	
		pixels[(i * 4) + 1] = (int)(255 * simplex_grid.grid[i]);	
		pixels[(i * 4) + 2] = (int)(255 * simplex_grid.grid[i]);	
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
