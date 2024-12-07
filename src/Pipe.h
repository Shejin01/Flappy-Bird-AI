#pragma once

#include <SFML/Graphics.hpp>

class Pipe {
public:
	double x, y;
	static double width, height;
	static double xVelocity;

	Pipe(double x, double y);
	void Update(double dt);
	void Draw(sf::RenderWindow& window);
};