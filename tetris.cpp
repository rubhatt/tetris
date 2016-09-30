#include <SFML/Graphics.hpp>

#include <vector>
#include <iostream>
#include <array>
#include <cassert>

template<typename T>
struct Coordinates {
    T x;
    T y;
    Coordinates(const T x, const T y) : x(x), y(y) {}
};

using Offset = Coordinates<int>;

const int k_NumTetrisShapes = 7;
const int k_NumBlocksPerShape = 4;

using Offsets = std::array<Offset, k_NumBlocksPerShape>;
using TetrisShapeDef = std::array<Offsets, k_NumTetrisShapes>;

const TetrisShapeDef k_Offsets = {
{
    { {Offset(0,0), Offset(1,0), Offset(2,0), Offset(3,0)} }, // I
    { {Offset(0,0), Offset(1,0), Offset(2,0), Offset(1,1)} }, // T
    { {Offset(0,0), Offset(1,0), Offset(1,1), Offset(0,1)} }, // square
    { {Offset(0,0), Offset(0,1), Offset(0,2), Offset(1,2)} }, // L
    { {Offset(0,0), Offset(0,1), Offset(0,2), Offset(-1,2)} }, // backwards L
    { {Offset(0,0), Offset(0,1), Offset(-1,1), Offset(-1,2)} }, // S
    { {Offset(0,0), Offset(0,1), Offset(1,1), Offset(1,2)} } // Z
}
};

void move_sprites(float x_offset, std::vector<sf::Sprite *> &sprites) {
    for (auto * sprite : sprites) {
	sprite->move(x_offset, 0.f);
    }
}

std::vector<sf::Sprite> tetris_blocks(sf::Texture &texture) {
    std::vector<sf::Sprite> sprites;
    sprites.reserve(7);
    // there are 7 sprites in total that we care about for each color.
    
    sf::Sprite purple;
    purple.setTexture(texture);
    purple.setTextureRect(sf::IntRect(47, 39, 50, 50));
    sprites.push_back(purple);

    sf::Sprite green;
    green.setTexture(texture);
    green.setTextureRect(sf::IntRect(79, 157, 50, 50));
    sprites.push_back(green);

    sf::Sprite yellow;
    yellow.setTexture(texture);
    yellow.setTextureRect(sf::IntRect(266, 42, 50, 50));
    sprites.push_back(yellow);


    sf::Sprite red;
    red.setTexture(texture);
    red.setTextureRect(sf::IntRect(263, 206, 50, 50));
    sprites.push_back(red);

    sf::Sprite cyan;
    cyan.setTexture(texture);
    cyan.setTextureRect(sf::IntRect(428, 114, 50, 50));
    sprites.push_back(cyan);

    sf::Sprite orange;
    orange.setTexture(texture);
    orange.setTextureRect(sf::IntRect(539, 107, 50, 50));
    sprites.push_back(orange);

    sf::Sprite magenta;
    magenta.setTexture(texture);
    magenta.setTextureRect(sf::IntRect(570, 216, 50, 50));
    sprites.push_back(magenta);

    return sprites;
}

enum class Color {
    e_Purple,
    e_Green,
    e_Yellow,
    e_Red,
    e_Cyan,
    e_Orange,
    e_Magenta
};

class TetrisShape : public sf::Drawable, public sf::Transformable {
    std::vector<sf::Sprite> m_sprites;
  public:
    TetrisShape(
	    const sf::Sprite &base_sprite,
	    const std::array<Offset, 4> & offset_array);

    virtual void draw(
	    sf::RenderTarget &target, sf::RenderStates states) const;
};

TetrisShape::TetrisShape(
	const sf::Sprite & base_sprite,
	const std::array<Offset, 4> &offset_array) {
    for (auto & offset : offset_array) {
	sf::Sprite sprite(base_sprite);
	sprite.setPosition(
		(offset.x * 50),
		(offset.y *50));
	m_sprites.push_back(sprite);
    }
}

void TetrisShape::draw(
	sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= this->getTransform();
    for (auto & sprite : m_sprites) {
	target.draw(sprite, states.transform);
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "RuTetris");
    window.setFramerateLimit(60);

    sf::Texture texture;
    if (!texture.loadFromFile("./assets/tiles.png")) {
	throw std::runtime_error("Failed to find blocks texture file.");
    }

    auto sprites = tetris_blocks(texture);
    assert(sprites.size() == k_Offsets.size());

    unsigned shape_index = 0;

    std::vector<TetrisShape> tetris_shapes;
    static Coordinates<int> k_Coords{300,300};
    for (unsigned i = 0; i < sprites.size(); ++i) {
	tetris_shapes.emplace_back(sprites[i], k_Offsets[i]);
	tetris_shapes.back().setPosition(k_Coords.x, k_Coords.y);
    }

    TetrisShape * current = &tetris_shapes[shape_index++];
    while (window.isOpen()) {
	sf::Event event;
	while (window.pollEvent(event)) {
	    if (event.type == sf::Event::Closed) {
		window.close();
	    }
	    bool change_shape = false;
	    int rotate = 0;
	    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		change_shape = true;
	    }
	    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		rotate = 1;
	    }
	    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		rotate = -1;
	    }

	    if (change_shape) {
		std::cout << "changing shape to ";
		if (shape_index >= k_Offsets.size()) { shape_index = 0; }
		std::cout << shape_index << "\n";
		current = &tetris_shapes[shape_index++];
	    }
	    if (rotate) {
		current->rotate(rotate * 45);
	    }
	}

	window.clear();
	if (current) { window.draw(*current); }
	window.display();
    }
    return 0;
}
