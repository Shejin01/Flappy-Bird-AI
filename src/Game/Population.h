#pragma once

#include "Bird.h"
#include "Pipe.h"
#include <vector>
#include <SFML/Graphics.hpp>

class Population {
private:
	double yStartPos = 30;
public:
	std::vector<Bird> birds;

	Population(int populationSize);
	void Mutate(Bird* bird, double mutationRate);
	void Crossover(Bird* parent1, Bird* parent2, Bird* child1, Bird* child2, double mutationRate);
	void Evaluate(double gapX, double gapY, double jumpVelocity);
	void Evolve(double mutationRate);
	bool IsPopulationDead();

	void Restart();
	void Update(double dt);
	void DetectCollision(Pipe* pipe1, Pipe* pipe2, int score);
	void Draw(sf::RenderWindow& window);
};