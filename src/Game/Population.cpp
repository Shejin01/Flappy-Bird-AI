#include "Population.h"

Population::Population(int populationSize) {
	for (int i = 0; i < populationSize; i++) {
		birds.push_back(Bird(yStartPos));
	}
}

void Population::Mutate(Bird* bird, double mutationRate) {
	for (auto& layer : bird->nn.layers) {
		for (int i = 0; i < layer.weights.rows; i++) {
			for (int j = 0; j < layer.weights.cols; j++) {
				if (RandomFromRange(0, 1000) * 0.001f > mutationRate) continue;
				std::cout << "a";
				layer.weights(i, j) += ((double)RandomFromRange(0, 200) - 100) * 0.5;
			}
		}
		for (int i = 0; i < layer.biases.size; i++) {
			if (RandomFromRange(0, 1000) * 0.001f > mutationRate) continue;
			layer.biases(i) += ((double)RandomFromRange(0, 200) - 100) * 0.5;
		}
	}
}

void Population::Crossover(Bird* parent1, Bird* parent2, Bird* child1, Bird* child2, double mutationRate) {
	for (int i = 0; i < parent1->nn.layers.size(); i++) {
		Layer& parent1Layer = parent1->nn.layers[i];
		Layer& parent2Layer = parent2->nn.layers[i];

		for (int j = 0; j < parent1Layer.weights.rows; j++) {
			for (int k = 0; k < parent1Layer.weights.cols; k++) {
				int fromFather = RandomFromRange(0, 1);
				child1->nn.layers[i].weights(j, k) = parent1Layer.weights(j, k) * fromFather
					+ parent2Layer.weights(j, k) * (1 - fromFather);
				child2->nn.layers[i].weights(j, k) = parent2Layer.weights(j, k) * fromFather
					+ parent1Layer.weights(j, k) * (1 - fromFather);
			}
		}

		for (int j = 0; j < parent1Layer.biases.size; j++) {
			int fromFather = RandomFromRange(0, 1);
			child1->nn.layers[i].biases(j) = parent1Layer.biases(j) * fromFather
				+ parent2Layer.biases(j) * (1 - fromFather);
			child2->nn.layers[i].biases(j) = parent2Layer.biases(j) * fromFather
				+ parent1Layer.biases(j) * (1 - fromFather);
		}
	}

	Mutate(child1, mutationRate);
	Mutate(child2, mutationRate);
}

void Population::Evaluate(double gapX, double gapY, double jumpVelocity) {
	double dx = gapX - Bird::x;
	for (auto& bird : birds) {
		if (bird.isDead) continue;
		double dy = gapY - bird.y;
		double jumpValue = bird.nn.Evaluate(Vector(std::vector<double>{dx, dy}))(0);
		if (jumpValue >= 0.5) bird.Jump(jumpVelocity);
	}
}

void Population::Evolve(double mutationRate) {
	std::sort(birds.begin(), birds.end(), [](Bird& a, Bird& b) { return a.score > b.score; });
	int numberOfElites = birds.size() * 0.1;
	std::vector<Bird> newBirds(birds.begin(), birds.begin() + numberOfElites);
	int numberOfReproducers = birds.size() * 0.5;

	// Insert the new children
	for (int i = 0; i < birds.size() - numberOfElites; i += 2) {
		int parent1Index = RandomFromRange(0, numberOfReproducers-1);
		int parent2Index = (parent1Index + RandomFromRange(1, numberOfReproducers-1)) % numberOfReproducers;
		Bird child1(yStartPos), child2(yStartPos);
		Crossover(&birds[parent1Index], &birds[parent2Index], &child1, &child2, mutationRate);
		newBirds.push_back(child1);
		newBirds.push_back(child2);
	}

	birds = newBirds;
	Restart();
}

bool Population::IsPopulationDead() {
	for (auto& bird : birds) {
		if (!bird.isDead) return false;
	}
	return true;
}

void Population::Restart() {
	for (auto& bird : birds) {
		bird.isDead = false;
		bird.xOffset = 0;
		bird.y = yStartPos;
		bird.yVelocity = 0;
		bird.score = 0;
	}
}

void Population::Update(double dt) {
	for (auto& bird : birds) bird.Update(dt);
}

void Population::DetectCollision(Pipe* pipe1, Pipe* pipe2, int score) {
	for (auto& bird : birds) {
		bool isCollision = bird.DetectPipeCollision(pipe1) || bird.DetectPipeCollision(pipe2) || bird.DetectRoofCollision(60, 0);
		if (isCollision && !bird.isDead) {
			bird.isDead = true;
			bird.score = score;
		}
	}
}

void Population::Draw(sf::RenderWindow& window) {
	for (auto& bird : birds) bird.Draw(window);
}