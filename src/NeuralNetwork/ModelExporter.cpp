#include <NeuralNetwork/ModelExporter.h>

void SaveModelToCSV(std::string path, NeuralNetwork* nn) {
	std::ofstream file;
	file.open(path);

	// Write Number of Layers
	file << nn->GetInputSize() << ',';
	for (int i = 0; i < nn->layers.size(); i++) {
		file << nn->layers[i].numOfNodes << ',';
	}
	file << '\n';

	// Write Activation Functions and Cost Function
	file << nn->layers[0].activationFunction->GetFunctionType() << ',';
	file << nn->layers[nn->layers.size() - 1].activationFunction->GetFunctionType() << ',';
	file << nn->costFunction->GetCostType() << '\n';

	// Write Weights of Each Layer
	for (auto& layer : nn->layers) {
		for (int i = 0; i < layer.numOfNodes; i++) {
			for (int j = 0; j < layer.numOfIncomingNodes; j++) {
				file << layer.weights(i, j) << ',';
			}
		}
		file << '\n';
	}

	// Write Biases of Each Layer
	for (auto& layer : nn->layers) {
		for (int i = 0; i < layer.numOfNodes; i++) {
			file << layer.biases(i) << ',';
		}
		file << '\n';
	}

	file.close();
	std::cout << "Model successfully saved at " << path << "\n\n";
}

NeuralNetwork LoadModelFromCSV(std::string path) {
	std::ifstream file;
	file.open(path);

	std::vector<int> numberOfNeurons;
	AF::FunctionType hiddenLayerAF, outputLayerAF;
	Cost::CostType costType;

	// Load Number Of Layers
	if (file.good()) {
		std::string line;
		std::string substring;
		std::getline(file, line);
		std::stringstream ss(line);
		while (std::getline(ss, substring, ',')) {
			numberOfNeurons.push_back(std::stoi(substring));
		}

		std::getline(file, line);
		ss = std::stringstream(line);
		std::getline(ss, substring, ',');
		hiddenLayerAF = AF::FunctionType(std::stoi(substring));
		std::getline(ss, substring, ',');
		outputLayerAF = AF::FunctionType(std::stoi(substring));
		std::getline(ss, substring, ',');
		costType = Cost::CostType(std::stoi(substring));
	}

	NeuralNetwork nn(numberOfNeurons, hiddenLayerAF, outputLayerAF, costType);

	// Load Weights Of Each Layer
	for (int i = 0; i < numberOfNeurons.size() - 1; i++) {
		std::string line;
		std::string substring;
		std::vector<double> weights;
		std::getline(file, line);
		std::stringstream ss(line);
		while (std::getline(ss, substring, ',')) {
			weights.push_back(std::stod(substring));
		}

		Layer& layer = nn.layers[i];
		for (int j = 0; j < layer.numOfNodes; j++) {
			for (int k = 0; k < layer.numOfIncomingNodes; k++) {
				layer.weights(j, k) = weights[j * layer.numOfIncomingNodes + k];
			}
		}
	}

	// Load Biases Of Each Layer
	for (int i = 0; i < numberOfNeurons.size() - 1; i++) {
		std::string line;
		std::string substring;
		std::vector<double> biases;
		std::getline(file, line);
		std::stringstream ss(line);
		while (std::getline(ss, substring, ',')) {
			biases.push_back(std::stod(substring));
		}

		Layer& layer = nn.layers[i];
		for (int j = 0; j < layer.numOfNodes; j++) {
			layer.biases(j) = biases[j];
		}
	}

	file.close();
	return nn;
}

void SaveModelToJS(std::string path, NeuralNetwork* nn) {
	std::ofstream file;
	file.open(path);

	// Write Number of Layers
	file << "var model = {\n	'config': [";
	file << nn->GetInputSize() << ',';
	for (int i = 0; i < nn->layers.size(); i++) {
		file << nn->layers[i].numOfNodes << ',';
	}
	file << "],\n";

	// Write Activation Functions
	file << "	'activationFunctions': [";
	file << nn->layers[0].activationFunction->GetFunctionType() << ',';
	file << nn->layers[nn->layers.size() - 1].activationFunction->GetFunctionType() << "],\n";

	// Write Cost Function
	file << "	'cost': " << nn->costFunction->GetCostType() << ",\n";

	// Write Weights of Each Layer
	file << "	'weights': [";
	for (auto& layer : nn->layers) {
		file << "[";
		for (int i = 0; i < layer.numOfNodes; i++) {
			file << "[";
			for (int j = 0; j < layer.numOfIncomingNodes; j++) {
				file << layer.weights(i, j) << ',';
			}
			file << "],";
		}
		file << "],";
	}
	file << "],\n";

	// Write Biases of Each Layer
	file << "	'biases': [";
	for (auto& layer : nn->layers) {
		file << "[";
		for (int i = 0; i < layer.numOfNodes; i++) {
			file << layer.biases(i) << ',';
		}
		file << "],";
	}
	file << "]\n}";

	file.close();
	std::cout << "Model successfully saved at " << path << "\n\n";
}