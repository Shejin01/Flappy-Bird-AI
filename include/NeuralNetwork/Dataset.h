#pragma once

#include <NeuralNetwork/Maths/Vector.h>
#include <vector>

struct DataPoint {
	Vector input, expectedOutput;
	DataPoint(Vector input, Vector expectedOutput)
		: input(input), expectedOutput(expectedOutput) {
	}
};
typedef std::vector<DataPoint> Dataset;