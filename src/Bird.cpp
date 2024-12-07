#include "Bird.h"

double Bird::x = 20;
double Bird::radius = 2;
double Bird::gravityAcceleration = 30;

Bird::Bird(double y, sf::Color color) : y(y), color(color) {}

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
		x + radius < pipe->x ||
		x - radius >= pipe->x + pipe->width ||
		y + radius < pipe->y ||
		y - radius >= pipe->y + pipe->height
	);
}

bool Bird::DetectRoofCollision(int top, int bottom) {
	return (y + radius >= top || y - radius < bottom);
}

void Bird::Draw(sf::RenderWindow& window) {
	sf::CircleShape shape(radius, 16);
	shape.setFillColor(color);
	shape.setOrigin(radius, radius);
	shape.setPosition(x, y);
	window.draw(shape);
}