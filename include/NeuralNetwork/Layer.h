#pragma once

#include <time.h>
#include <math.h>
#include <vector>
#include <NeuralNetwork/ActivationFunction.h>
#include <NeuralNetwork/Cost.h>
#include <NeuralNetwork/Maths/Vector.h>
#include <NeuralNetwork/Maths/Matrix.h>
#include <NeuralNetwork/Maths/Random.h>
#include <iostream>

class Layer {
private:
	Vector weightedInputs;
	Vector rawInputs;
	Matrix weightCostGradients;
	Vector biasCostGradients;

	Vector CalculateOutputLayerErrors(Vector actualOutput, Vector expectedOutput, Cost::CostFunction* costFunction);
	Vector CalculateHiddenLayerErrors(Layer& nextLayer, Vector nextLayerErrors);
public:
	AF::ActivationFunction* activationFunction;
	int numOfNodes, numOfIncomingNodes;
	Matrix weights;
	Vector biases;

	Layer(int numOfNodes, int numOfIncomingNodes, AF::FunctionType activationFunctionType);
	void RandomizeParameters();
	Vector FeedForward(Vector input);
	void SetActivationFunction(AF::FunctionType activationFunctionType);

	void ApplyGradients(double learningRate, int miniBatchSize, double lambda, int datasetSize, bool isL2);
	void UpdateGradients(Vector nodeValues);
	void ClearGradients();

	friend class NeuralNetwork;
};