#include <SFML/Graphics.hpp>

#include <vector>
#include <iostream>


void move_sprites(float x_offset, std::vector<sf::Sprite *> &sprites) {
    for (auto * sprite : sprites) {
	sprite->move(x_offset, 0.f);
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Red);

    sf::Texture texture;
    if (!texture.loadFromFile("./assets/blocks.png")) {
	throw std::runtime_error("Failed to find blocks texture file.");
    }

    int height = 28, width = 28, skip = 2, start = 1;

    std::vector<sf::Sprite> sprites;
    for (int i = 0; i < 5; ++i) {
	int h_offset = start + i * (width + skip);
	sf::Sprite sprite;
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(h_offset, 1, width, height));
	sprites.push_back(sprite);
    }

    // make a shape - start simple, square(2x2).
    sf::Sprite purple = sprites[0];
    sf::Sprite cp1 = purple;
    sf::Sprite cp2 = purple;
    sf::Sprite cp3 = purple;
    sf::Sprite cp4 = purple;
    
    cp1.setPosition(10, 10);
    cp2.setPosition(10+width, 10);
    cp3.setPosition(10, 10+height);
    cp4.setPosition(10+width, 10+height);
    std::vector<sf::Sprite *> rect{&cp1, &cp2, &cp3, &cp4};

    while (window.isOpen()) {
	sf::Event event;
	while (window.pollEvent(event)) {
	    if (event.type == sf::Event::Closed) {
		window.close();
	    }
	    float x_offset = 0.f;
	    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		x_offset = -1.f;
	    }
	    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		x_offset = 1.f;
	    }

	    // move all the sprites corresponding to the box either left or right.
	    move_sprites(x_offset, rect);
	}

	window.clear();
	window.draw(cp1);
	window.draw(cp2);
	window.draw(cp3);
	window.draw(cp4);
	window.display();
    }
    return 0;
}
