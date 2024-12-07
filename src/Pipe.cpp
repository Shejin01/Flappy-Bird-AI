#include "Pipe.h"

double Pipe::width = 5;
double Pipe::height = 50;
double Pipe::xVelocity = -20;

Pipe::Pipe(double x, double y)
	: x(x), y(y) {}

void Pipe::Update(double dt) {
	x += xVelocity * dt;
}

void Pipe::Draw(sf::RenderWindow& window) {
	sf::RectangleShape shape(sf::Vector2f(width, height));
	shape.setFillColor(sf::Color::Green);
	shape.setPosition(x, y);
	window.draw(shape);
}