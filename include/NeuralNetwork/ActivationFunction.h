#pragma once

#include <math.h>
#include <vector>
#include <NeuralNetwork/Maths/Vector.h>

namespace ActivationFunctions {

	enum FunctionType {
		LinearAF,
		BinaryStepAF,
		ReLUAF,
		LeakyReLUAF,
		SigmoidAF,
		SoftmaxAF,
		TanHAF
	};

	class ActivationFunction {
	public:
		virtual Vector Activate(Vector input) = 0;
		virtual Vector ActivateDerivative(Vector input) = 0;
		virtual FunctionType GetFunctionType() const = 0;
	};

	class Linear : public ActivationFunction {
	public:
		Vector Activate(Vector input) override;
		Vector ActivateDerivative(Vector input) override;
		FunctionType GetFunctionType() const override;
	};
	class BinaryStep : public ActivationFunction {
	public:
		Vector Activate(Vector input) override;
		Vector ActivateDerivative(Vector input) override;
		FunctionType GetFunctionType() const override;
	};
	class ReLU : public ActivationFunction {
	public:
		Vector Activate(Vector input) override;
		Vector ActivateDerivative(Vector input) override;
		FunctionType GetFunctionType() const override;
	};
	class LeakyReLU : public ActivationFunction {
	public:
		Vector Activate(Vector input) override;
		Vector ActivateDerivative(Vector input) override;
		FunctionType GetFunctionType() const override;
	};
	class Sigmoid : public ActivationFunction {
	public:
		Vector Activate(Vector input) override;
		Vector ActivateDerivative(Vector input) override;
		FunctionType GetFunctionType() const override;
	};
	class TanH : public ActivationFunction {
	public:
		Vector Activate(Vector input) override;
		Vector ActivateDerivative(Vector input) override;
		FunctionType GetFunctionType() const override;
	};
	class Softmax : public ActivationFunction {
	public:
		Vector Activate(Vector input) override;
		Vector ActivateDerivative(Vector input) override;
		FunctionType GetFunctionType() const override;
	};

	ActivationFunction* GetFunctionFromEnum(FunctionType funcType);
}
namespace AF = ActivationFunctions;