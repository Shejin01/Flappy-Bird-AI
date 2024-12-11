#include <NeuralNetwork/ActivationFunction.h>

namespace ActivationFunctions {
	Vector Linear::Activate(Vector input) {
		return input;
	}
	Vector Linear::ActivateDerivative(Vector input) {
		return Vector(input.size, 1);
	}
	FunctionType Linear::GetFunctionType() const {
		return AF::LinearAF;
	}

	Vector BinaryStep::Activate(Vector input) {
		return input.ForEach([](double element) -> double { return element >= 0; });
	}
	Vector BinaryStep::ActivateDerivative(Vector input) {
		return Vector(input.size);
	}
	FunctionType BinaryStep::GetFunctionType() const {
		return AF::BinaryStepAF;
	}

	Vector ReLU::Activate(Vector input) {
		return input.ForEach([](double element) -> double { return fmax(0, element); });
	}
	Vector ReLU::ActivateDerivative(Vector input) {
		return input.ForEach([](double element) -> double { return element >= 0; });
	}
	FunctionType ReLU::GetFunctionType() const {
		return AF::ReLUAF;
	}

	Vector LeakyReLU::Activate(Vector input) {
		return input.ForEach([](double element) -> double { return fmax(0.1f * element, element); });
	}
	Vector LeakyReLU::ActivateDerivative(Vector input) {
		return input.ForEach([](double element) -> double { return (element >= 0) ? 1 : 0.1f; });
	}
	FunctionType LeakyReLU::GetFunctionType() const {
		return AF::LeakyReLUAF;
	}
	
	Vector Sigmoid::Activate(Vector input) {
		return input.ForEach([](double element) -> double { return 1.0f / (1.0f + exp(-element)); });
	}
	Vector Sigmoid::ActivateDerivative(Vector input) {
		Vector value = Activate(input);
		return value.ForEach([](double element) -> double { return element * (1 - element); });
	}
	FunctionType Sigmoid::GetFunctionType() const {
		return AF::SigmoidAF;
	}
	
	Vector TanH::Activate(Vector input) {
		return input.ForEach([](double element) -> double { return tanh(element); });
	}
	Vector TanH::ActivateDerivative(Vector input) {
		return input.ForEach([](double element) -> double { return 1.0f / pow(cosh(element), 2); });
	}
	FunctionType TanH::GetFunctionType() const {
		return AF::TanHAF;
	}
	
	Vector Softmax::Activate(Vector input) {
		Vector output(input.size);
		double sum = 0;
		for (int i = 0; i < input.size; i++) {
			output(i) = exp(input(i));
			sum += output(i);
		}
		return output / sum;
	}
	Vector Softmax::ActivateDerivative(Vector input) {
		/*Vector output = Softmax(input);
		output.ForEach([](double element) -> double { return element * (1 - element); });
		return output;*/

		Vector softmaxInputs = Activate(input);
		Vector output(input.size);
		for (int i = 0; i < input.size; i++) {
			for (int j = 0; j < input.size; j++) {
				output(i) += softmaxInputs(i) * ((i == j) - softmaxInputs(j));
			}
		}
		return output;
	}
	FunctionType Softmax::GetFunctionType() const {
		return AF::SoftmaxAF;
	}

	ActivationFunction* GetFunctionFromEnum(FunctionType funcType) {
		switch (funcType) {
		case FunctionType::SigmoidAF:
			return new Sigmoid();
			break;
		case FunctionType::SoftmaxAF:
			return new Softmax();
			break;
		case FunctionType::TanHAF:
			return new TanH();
			break;
		case FunctionType::ReLUAF:
			return new ReLU();
			break;
		case FunctionType::LinearAF:
			return new Linear();
			break;
		case FunctionType::BinaryStepAF:
			return new BinaryStep();
			break;
		case FunctionType::LeakyReLUAF:
			return new LeakyReLU();
			break;
		}
	}
}