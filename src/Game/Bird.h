#pragma once

#include <SFML/Graphics.hpp>
#include <NeuralNetwork/NeuralNetwork.h>
#include "Pipe.h"
#include <string>
#include <iostream>

class Bird {
private:
	static sf::Texture texture;
	static sf::Sprite sprite;
public:
	double y;
	double yVelocity = 0;
	static double x;
	static double radius;
	static double gravityAcceleration;
	double xOffset = 0;
	bool isDead = false;

	NeuralNetwork nn{ {2, 3, 1} };
	int score = 0;

	Bird();
	Bird(double y);
	static void InitTextures(std::string texturePath);
	void Update(double dt);
	void Jump(double jumpVelocity);
	bool DetectPipeCollision(Pipe* pipe);
	bool DetectRoofCollision(int top, int bottom);
	void Draw(sf::RenderWindow& window);
};