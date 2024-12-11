#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class Pipe {
private:
	static sf::Texture texture;
	static sf::Sprite sprite;
public:
	double x, y;
	bool flipTexture;
	bool isEntered = false;
	static double halfWidth, halfHeight;
	static double xVelocity;

	Pipe(double x, double y, bool flipTexture = false);
	void Update(double dt);
	static void SetTexture(std::string texturePath);
	void Draw(sf::RenderWindow& window);
};