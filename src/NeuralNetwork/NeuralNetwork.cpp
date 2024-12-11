#include <NeuralNetwork/NeuralNetwork.h>

NeuralNetwork::NeuralNetwork(std::vector<int> numberOfNeurons, AF::FunctionType hiddenLayerAF, AF::FunctionType outputLayerAF, Cost::CostType costType) {
	inputSize = numberOfNeurons[0];
	costFunction = Cost::GetFunctionFromEnum(costType);
	for (int i = 1; i < numberOfNeurons.size(); i++) {
		layers.push_back(Layer(numberOfNeurons[i], numberOfNeurons[i-1], hiddenLayerAF));
	}
	layers[layers.size()-1].SetActivationFunction(outputLayerAF);
}
void NeuralNetwork::RandomizeAllParameters() {
	for (auto& layer : layers) {
		layer.RandomizeParameters();
	}
}

Vector NeuralNetwork::Evaluate(Vector input) {
	for (int i = 0; i < layers.size(); i++) {
		input = layers[i].FeedForward(input);
	}
	return input;
}
double NeuralNetwork::Cost(Vector actualOutput, Vector expectedOutput) {
	return costFunction->Evaluate(actualOutput, expectedOutput) + RegularizationAmount(lambda, datasetSize);
}
double NeuralNetwork::RegularizationAmount(double lambda, int datasetSize) const {
	double weightSum = 0;
	for (auto& layer : layers) {
		for (int i = 0; i < layer.weights.rows; i++) {
			for (int j = 0; j < layer.weights.cols; j++) {
				// L2 (Weight Decay) Regularization
				if (isL2) weightSum += layer.weights.Get(i, j) * layer.weights.Get(i, j);
				// L1 Regularization
				else weightSum += abs(layer.weights.Get(i, j));
			}
		}
	}
	if (isL2) weightSum *= 0.5;
	return weightSum * lambda / datasetSize;
}

void NeuralNetwork::SGD(Dataset* dataset, int epochs, double learningRate, int miniBatchSize, double lambda, bool isL2, Dataset* validation_dataset) {
	float totalTimeTaken = 0.0f;

	// Report File Settings
	std::ofstream file;
	bool isSaveData = settings.monitorFlags & MONITOR_SAVE_PERFORMANCE_DATA;
	bool isTrainAcc = settings.monitorFlags & MONITOR_TRAIN_ACCURACY;
	bool isTrainCost = settings.monitorFlags & MONITOR_TRAIN_COST;
	bool isValidAcc = settings.monitorFlags & MONITOR_VALIDATION_ACCURACY;
	bool isValidCost = settings.monitorFlags & MONITOR_VALIDATION_COST;
	if (isSaveData) {
		file.exceptions(std::ofstream::failbit | std::ofstream::badbit);
		try {
			file.open(settings.performanceReportFilePath);
			file << "Epoch, ";
			if (isTrainAcc) file << "Train Accuracy, ";
			if (isTrainCost) file << "Train Cost, ";
			if (isValidAcc) file << "Validation Accuracy, ";
			if (isValidCost) file << "Validation Cost, ";
			file << '\n';
		}
		catch (std::ofstream::failure e) {
			std::cout << e.what() << '\n';
		}
	}
	
	// SGD
	std::cout << "Training Started - " << epochs << " Epochs, Learning Rate: " << learningRate << ", Mini-Batch Size: " << miniBatchSize << ", Lambda: " << lambda << '\n';
	for (int i = 0; i < epochs; i++) {
		std::cout << "Epoch: " << i;
		if (isSaveData) file << i << ", ";

		// Randomize Train Dataset
		ShuffleVector(dataset);

		// Train
		auto startTime = std::chrono::high_resolution_clock::now();
		Learn(dataset, learningRate, miniBatchSize, lambda, isL2);
		auto endTime = std::chrono::high_resolution_clock::now();
		auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
		std::cout << " - " << (dt.count() * 0.001f) << 's';
		totalTimeTaken += dt.count() * 0.001f;

		// Test Train Dataset
		ProcessDataset(dataset, isTrainAcc, isTrainCost, file, isSaveData, "Train");
		// Test Validation Dataset
		if (validation_dataset != nullptr) {
			ProcessDataset(validation_dataset, isValidAcc, isValidCost, file, isSaveData, "Validation");
		}

		std::cout << '\n';
		if (isSaveData) file << '\n';
	}

	// Save Report File
	if (isSaveData) {
		file.close();
		std::cout << "Performance Report Successfully Saved at: " << settings.performanceReportFilePath << '\n';
	}
	std::cout << "Training Completed in " << totalTimeTaken << "s\n";
}
void NeuralNetwork::ProcessDataset(Dataset* dataset, bool isMonitorAcc, bool isMonitorCost, std::ofstream& file, bool isSaveData, std::string type) {
	if (!(isMonitorAcc || isMonitorCost)) return;

	Vector output((*dataset)[0].expectedOutput.size);
	int correctPredictions = 0;
	double cost = 0.0f;
	int size = dataset->size();
	for (int i = 0; i < size; i++) {
		output = Evaluate((*dataset)[i].input);
		int prediction = output.MaxIndex();

		if (isMonitorAcc && (*dataset)[i].expectedOutput(prediction) == 1) correctPredictions++;
		if (isMonitorCost) cost += Cost(output, (*dataset)[i].expectedOutput);
	}
	cost /= size;
	if (isMonitorAcc) {
		std::cout << " - " << type << " Accuracy: " << correctPredictions << " / " << size << " (" << (correctPredictions * 100.0f / size) << "%)";
		if (isSaveData) file << (correctPredictions * 100.0f / size) << ", ";
	}
	if (isMonitorCost) {
		std::cout << " - " << type << " Cost: " << cost;
		if (isSaveData) file << cost << ", ";
	}
}
void NeuralNetwork::Learn(Dataset* dataset, double learningRate, int miniBatchSize, double lambda, bool isL2) {
	this->lambda = lambda;
	this->datasetSize = dataset->size();
	this->isL2 = isL2;

	int numOfBatches = datasetSize / miniBatchSize;
	for (int i = 0; i < numOfBatches; i++) {
		for (int j = 0; j < miniBatchSize; j++) {
			int index = j + i * miniBatchSize;
			// Feedforward
			Vector actualOutput = Evaluate((*dataset)[index].input);
			BackPropagate(&(*dataset)[index], &actualOutput);
		}
		// Gradient Descent
		ApplyAllGradients(learningRate, miniBatchSize, lambda, datasetSize);
		ClearAllGradients();
	}
}
void NeuralNetwork::BackPropagate(DataPoint* dataPoint, Vector* actualOutput) {
	Layer& outputLayer = layers[layers.size() - 1];

	// Error
	Vector errors = outputLayer.CalculateOutputLayerErrors(*actualOutput, dataPoint->expectedOutput, costFunction);
	outputLayer.UpdateGradients(errors);

	for (int i = layers.size() - 2; i >= 0; i--) {
		errors = layers[i].CalculateHiddenLayerErrors(layers[i + 1], errors);
		layers[i].UpdateGradients(errors);
	}
}

void NeuralNetwork::ApplyAllGradients(double learningRate, int miniBatchSize, double lambda, int datasetSize) {
	for (auto& layer : layers) {
		layer.ApplyGradients(learningRate, miniBatchSize, lambda, datasetSize, isL2);
	}
}
void NeuralNetwork::ClearAllGradients() {
	for (auto& layer : layers) layer.ClearGradients();
}

int NeuralNetwork::GetInputSize() const {
	return inputSize;
}
void NeuralNetwork::SetActivationFunctions(AF::FunctionType hiddenLayerAF, AF::FunctionType outputLayerAF) {
	for (auto& layer : layers) {
		layer.SetActivationFunction(hiddenLayerAF);
	}
	layers[layers.size() - 1].SetActivationFunction(outputLayerAF);
}