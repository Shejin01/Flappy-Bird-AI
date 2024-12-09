#include "Bird.h"

double Bird::x = 20;
double Bird::radius = 1.75;
double Bird::gravityAcceleration = 30;

Bird::Bird(double y, sf::Color color, std::string texturePath) : y(y), color(color) {
	if (!texture.loadFromFile(texturePath)) {
		std::cout << "Failed to open texture at path: " << texturePath << '\n';
	}
	sprite.setTexture(texture);
	sprite.setOrigin(8, 8);
	sprite.setScale(0.5, 0.5);
}

void Bird::Update(double dt) {
	y += yVelocity * dt + 0.5 * gravityAcceleration * dt * dt;
	yVelocity += gravityAcceleration * dt;
}

void Bird::Jump(double jumpVelocity) {
	yVelocity = -jumpVelocity;
}

bool Bird::DetectPipeCollision(Pipe* pipe) {
	// AABB
	return !(
		x + radius <  pipe->x - pipe->halfWidth  ||
		x - radius >= pipe->x + pipe->halfWidth  ||
		y + radius <  pipe->y - pipe->halfHeight ||
		y - radius >= pipe->y + pipe->halfHeight
	);
}

bool Bird::DetectRoofCollision(int top, int bottom) {
	return (y + radius >= top || y - radius < bottom);
}

void Bird::Draw(sf::RenderWindow& window) {
	sprite.setPosition(x, y);
	window.draw(sprite);
}