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

void GenerateTwoPipes(std::vector<Pipe>* pipes) {
	double y = RandomFromRange(10, 50);
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

	Bird bird(30, sf::Color::Yellow, "assets/Objects/Bird.png");
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

	std::cout <<
		"Controls:\n"
		"Space - Jump\n"
		"R     - Restart\n"
		"Esc   - Quit\n\n";

	int frame = 0;
	int score = 0;
	int highScore = 0;
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
					GenerateTwoPipes(&pipes);
					frame = 0;
					score = 0;
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
				bool isCollision = bird.DetectPipeCollision(&pipes[0]) || bird.DetectPipeCollision(&pipes[1]) || bird.DetectRoofCollision(60, 0);
				if (isCollision) {
					gameOver = true;
					highScore = fmax(score, highScore);
				}
			}

			// Move Background
			for (int i = 0; i < bgdSprites.size(); i++) {
				bgdSprites[i].move(bgdSpeeds[i] * dt, 0);
				if (bgdSprites[i].getPosition().x < -160) bgdSprites[i].setPosition(0, 0);
			}

			frame++;
		}

		// Render
		window.clear(); 
		for (auto& sprite : bgdSprites) window.draw(sprite);
		bird.Draw(window);
		for (auto& pipe : pipes) pipe.Draw(window);
		window.display();
	}

	return 0;
}