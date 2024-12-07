#pragma once

#include <SFML/Graphics.hpp>
#include "Pipe.h"

class Bird {
public:
	double y;
	double yVelocity = 0;
	sf::Color color;
	static double x;
	static double radius;
	static double gravityAcceleration;

	Bird(double y, sf::Color color);
	void Update(double dt);
	void Jump(double jumpVelocity);
	bool DetectPipeCollision(Pipe* pipe);
	bool DetectRoofCollision(int top, int bottom);
	void Draw(sf::RenderWindow& window);
};