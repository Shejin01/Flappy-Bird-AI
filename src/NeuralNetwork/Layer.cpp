#include <NeuralNetwork/Layer.h>

Layer::Layer(int numOfNodes, int numOfIncomingNodes, AF::FunctionType activationFunctionType)
	: numOfNodes(numOfNodes), numOfIncomingNodes(numOfIncomingNodes) {
	activationFunction = AF::GetFunctionFromEnum(activationFunctionType);
	weights = Matrix(numOfNodes, numOfIncomingNodes);
	weightCostGradients = weights;
	biases = Vector(numOfNodes);
	biasCostGradients = biases;
	RandomizeParameters();
}

void Layer::RandomizeParameters() {
	srand(time(0));
	for (int i = 0; i < numOfNodes; i++) {
		for (int j = 0; j < numOfIncomingNodes; j++) {
			//weights(i, j) = ((double)(rand() % 10000) - 5000.0) / 10000.0;
			weights(i, j) = ((double)RandomFromRange(0, 10000) - 5000.0) / 10000.0;
		}
		//biases(i) = ((double)(rand() % 10000) - 5000.0) / 10000.0;
		biases(i) = ((double)RandomFromRange(0, 10000) - 5000.0) / 10000.0;
	}
}

Vector Layer::FeedForward(Vector input) {
	Vector output = weights * input + biases;
	weightedInputs = output;
	rawInputs = input;
	output = activationFunction->Activate(output);
	return output;
}

void Layer::SetActivationFunction(AF::FunctionType activationFunctionType) {
	this->activationFunction = AF::GetFunctionFromEnum(activationFunctionType);
}

void Layer::ApplyGradients(double learningRate, int miniBatchSize, double lambda, int datasetSize, bool isL2) {
	if (isL2) {
		double weightDecayFactor = 1 - (learningRate * lambda) / (double)datasetSize;
		weights = weights * weightDecayFactor - weightCostGradients * (learningRate / (double)miniBatchSize);
	}
	else {
		weights -= weights.Sign() * (learningRate * lambda / (double)datasetSize) + weightCostGradients * (learningRate / (double)miniBatchSize);
	}
	biases -= biasCostGradients * (learningRate / (double)miniBatchSize);
}

void Layer::UpdateGradients(Vector errors) {
	for (int i = 0; i < numOfNodes; i++) {
		for (int j = 0; j < numOfIncomingNodes; j++) {
			weightCostGradients(i, j) += rawInputs(j) * errors(i);
		}
		biasCostGradients(i) += errors(i);
	}
	//weightCostGradients += errors * rawInputs.transpose();
	//biasCostGradients += errors;
}

void Layer::ClearGradients() {
	weightCostGradients.SetZero();
	biasCostGradients.SetZero();
}

Vector Layer::CalculateOutputLayerErrors(Vector actualOutput, Vector expectedOutput, Cost::CostFunction* costFunction) {
	AF::FunctionType activationFunctionType = activationFunction->GetFunctionType();
	Cost::CostType costType = costFunction->GetCostType();
	// Optimization
	if ((activationFunctionType == AF::SoftmaxAF && (costType == Cost::CategoricalCrossEntropyCost || costType == Cost::BinaryCrossEntropyCost)) ||
		(activationFunctionType == AF::SigmoidAF && costType == Cost::SigmoidCrossEntropyCost)) {
		return actualOutput - expectedOutput;
	}

	return costFunction->EvaluateDerivative(actualOutput, expectedOutput) * activationFunction->ActivateDerivative(weightedInputs);
}

Vector Layer::CalculateHiddenLayerErrors(Layer& nextLayer, Vector nextLayerErrors) {
	Vector errors = nextLayer.weights.Transpose() * nextLayerErrors;
	return errors * activationFunction->ActivateDerivative(weightedInputs);
}