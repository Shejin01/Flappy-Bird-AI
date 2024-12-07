#include <iostream>
#include <SFML/Graphics.hpp>
#include "Bird.h"
#include "Pipe.h"
#include "Random.h"
#include <vector>

const int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
const int WORLD_WIDTH = 80, WORLD_HEIGHT = 60;
const int FPS = 60;
double jumpVelocity = 20;
double dt = 1.0 / (double)FPS;
double pipeHalfGap = 5;
int pipeSpawnTime = 2 * FPS; // seconds * (frames / second)

int main() {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Flappy Bird AI", 7U, settings);
	window.setFramerateLimit(FPS);

	sf::View view(sf::FloatRect(0, 0, WORLD_WIDTH, WORLD_HEIGHT));
	window.setView(view);

	Bird bird(30, sf::Color::Yellow);
	std::vector<Pipe> pipes;
	double y = RandomFromRange(10, 50);
	pipes.push_back(Pipe(80, y - Pipe::height - pipeHalfGap));
	pipes.push_back(Pipe(80, y + pipeHalfGap));

	int frame = 0;
	bool gameOver = false;
	while (window.isOpen()) {
		// Handle Events
		/*
			Escape - Close Window;
			Space  - Jump;
			R      - Restart;
		*/
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				switch (event.key.code) {
				case sf::Keyboard::Escape:
					window.close();
					break;
				case sf::Keyboard::Space:
					bird.Jump(jumpVelocity);
					break;
				case sf::Keyboard::R:
					// Restart
					bird.y = 30;
					bird.yVelocity = 0;
					pipes.clear();
					y = RandomFromRange(10, 50);
					pipes.push_back(Pipe(80, y - Pipe::height - pipeHalfGap));
					pipes.push_back(Pipe(80, y + pipeHalfGap));
					frame = 0;
					gameOver = false;
				}
			}
		}

		// Game Processes
		if (!gameOver) {
			// Update Physics
			bird.Update(dt);
			for (auto& pipe : pipes) pipe.Update(dt);

			// Spawn Pipe Timer
			if (frame == pipeSpawnTime) {
				double y = RandomFromRange(10, 50);
				pipes.push_back(Pipe(80, y - Pipe::height - pipeHalfGap));
				pipes.push_back(Pipe(80, y + pipeHalfGap));
				frame = 0;
			}
			// Collision Detection and Pipe Removal System
			if (pipes.size()) {
				if (pipes[0].x < -Pipe::width) pipes.erase(pipes.begin(), pipes.begin() + 1);

				bool isCollision = bird.DetectPipeCollision(&pipes[0]) || bird.DetectPipeCollision(&pipes[1]) || bird.DetectRoofCollision(60, 0);
				if (isCollision) gameOver = true;
			}
			frame++;
		}

		// Render
		window.clear(); 
		bird.Draw(window);
		for (auto& pipe : pipes) pipe.Draw(window);
		window.display();
	}

	return 0;
}