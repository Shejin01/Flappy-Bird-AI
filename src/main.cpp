#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game/Bird.h"
#include "Game/Pipe.h"
#include "Game/Population.h"
#include <vector>

const int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
const int WORLD_WIDTH = 80, WORLD_HEIGHT = 60;
const int FPS = 60;
double jumpVelocity = 20;
double dt = 1.0 / (double)FPS;
double pipeHalfGap = 7;
int pipeSpawnTime = 2 * FPS; // seconds * (frames / second)
double mutationRate = 0.01;

void GenerateTwoPipes(std::vector<Pipe>* pipes) {
	double y = RandomFromRange(5 + pipeHalfGap, WORLD_HEIGHT - 5 - pipeHalfGap);
	pipes->push_back(Pipe(WORLD_WIDTH + Pipe::halfWidth, y - Pipe::halfHeight - pipeHalfGap));
	pipes->push_back(Pipe(WORLD_WIDTH + Pipe::halfWidth, y + Pipe::halfHeight + pipeHalfGap, true));
}

int main() {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Flappy Bird AI", 7U, settings);
	window.setFramerateLimit(FPS);

	sf::View view(sf::FloatRect(0, 0, WORLD_WIDTH, WORLD_HEIGHT));
	window.setView(view);

	Bird::InitTextures("assets/Objects/Bird.png");
	Population population(500);
	std::vector<Pipe> pipes;
	GenerateTwoPipes(&pipes);

	Pipe::SetTexture("assets/Objects/Pipe.png");
	
	// Background
	std::vector<sf::Texture> bgdTextures(4, sf::Texture());
	std::vector<sf::Sprite> bgdSprites(4, sf::Sprite());
	std::vector<double> bgdSpeeds = { 0, -4, -5, -10 };
	std::vector<std::string> texturePaths = {
		"assets/Background/Sky.png", "assets/Background/Background_Clouds.png", "assets/Background/Clouds.png", "assets/Background/Foreground.png"
	};
	for (int i = 0; i < texturePaths.size(); i++) {
		bgdTextures[i].loadFromFile(texturePaths[i]);
		bgdTextures[i].setRepeated(true);

		bgdSprites[i].setTexture(bgdTextures[i]);
		bgdSprites[i].setScale(0.25, 0.25);
		bgdSprites[i].setPosition(0, 0);
		bgdSprites[i].setTextureRect(sf::IntRect(0, 0, 1280, 240));
	}

	std::cout << "Generation - 0\n";

	int frame = 0;
	int score = 0;
	int highScore = 0;
	int generation = 0;
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
				/*case sf::Keyboard::Space:
					bird.Jump(jumpVelocity);
					break;*/
				/*case sf::Keyboard::R:
					// Restart
					population.Restart();
					pipes.clear();
					GenerateTwoPipes(&pipes);
					frame = 0;
					score = 0;*/
			}
		}

		// Game Processes
		if (!population.IsPopulationDead()) {
			if (pipes.size()) {
				for (auto& pipe : pipes) {
					if (pipe.x < Bird::x - Pipe::halfWidth) continue;
					double gapY = pipe.y + Pipe::halfHeight + pipeHalfGap;
					population.Evaluate(pipe.x, gapY, jumpVelocity);
					break;
				}
			}

			// Update Physics
			population.Update(dt);
			for (auto& pipe : pipes) pipe.Update(dt);

			// Spawn Pipe Timer
			if (frame == pipeSpawnTime) {
				GenerateTwoPipes(&pipes);
				frame = 0;
			}
			if (pipes.size()) {
				// Score Counter
				for (auto pipe = pipes.begin(); pipe != pipes.end(); pipe += 2) {
					if (pipe->x > Bird::x + Bird::radius) break;
					if (!pipe->isEntered && pipe->x > Bird::x - Bird::radius) {
						pipe->isEntered = true;
						score++;
						highScore = fmax(score, highScore);
						std::cout << "Score: " << score << ", High Score: " << highScore << '\n';
						break;
					}
				}

				// Pipe Removal System
				if (pipes[0].x < -Pipe::halfWidth) pipes.erase(pipes.begin(), pipes.begin() + 1);

				// Collision Detection
				population.DetectCollision(&pipes[0], &pipes[1], score);
			}

			// Move Background
			for (int i = 0; i < bgdSprites.size(); i++) {
				bgdSprites[i].move(bgdSpeeds[i] * dt, 0);
				if (bgdSprites[i].getPosition().x < -160) bgdSprites[i].setPosition(0, 0);
			}

			frame++;
		}
		else {
			population.Evolve(mutationRate);
			pipes.clear();
			GenerateTwoPipes(&pipes);
			frame = 0;
			score = 0;
			generation++;
			std::cout << "\nGeneration - " << generation << '\n';
		}

		// Render
		window.clear(); 
		for (auto& sprite : bgdSprites) window.draw(sprite);
		population.Draw(window);
		for (auto& pipe : pipes) pipe.Draw(window);
		window.display();
	}

	return 0;
}