#include "Pipe.h"

double Pipe::halfWidth = 2.5;
double Pipe::halfHeight = 25;
double Pipe::xVelocity = -20;
sf::Texture Pipe::texture;
sf::Sprite Pipe::sprite;

Pipe::Pipe(double x, double y, bool flipTexture)
	: x(x), y(y), flipTexture(flipTexture) {}

void Pipe::Update(double dt) {
	x += xVelocity * dt;
}

void Pipe::SetTexture(std::string texturePath) {
	if (!texture.loadFromFile(texturePath)) {
		std::cout << "Failed to open texture at path: " << texturePath << '\n';
	}
	sprite.setTexture(texture);
	sprite.setOrigin(6, 50);
	sprite.setScale(0.5, 0.5);
}

void Pipe::Draw(sf::RenderWindow& window) {
	sprite.setPosition(x, y);
	if (flipTexture) sprite.setScale(0.5, -0.5);
	window.draw(sprite);
	if (flipTexture) sprite.setScale(0.5, 0.5);
}