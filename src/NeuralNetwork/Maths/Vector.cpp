#include <NeuralNetwork/Maths/Vector.h>

Vector::Vector() : size(0) {}
Vector::Vector(int size, double fillWithElement) : size(size) {
	elements = std::vector<double>(size, fillWithElement);
}
Vector::Vector(std::vector<double> elements) 
	: elements(elements), size(elements.size()) {}

Vector Vector::operator+(Vector other) {
	Vector vec = *this;
	if (CheckForErrors(other.size)) return vec;
	vec += other;
	return vec;
}
Vector Vector::operator-(Vector other) {
	Vector vec = *this;
	if (CheckForErrors(other.size)) return vec;
	vec -= other;
	return vec;
}
Vector Vector::operator*(Vector other) {
	Vector vec = *this;
	if (CheckForErrors(other.size)) return vec;
	vec *= other;
	return vec;
}
Vector Vector::operator*(double other) {
	Vector vec = *this;
	vec *= other;
	return vec;
}
Vector Vector::operator/(double other) {
	Vector vec = *this;
	vec /= other;
	return vec;
}

void Vector::operator+=(Vector other) {
	if (CheckForErrors(other.size)) return;
	for (int i = 0; i < size; i++) {
		elements[i] += other(i);
	}
}
void Vector::operator-=(Vector other) {
	if (CheckForErrors(other.size)) return;
	for (int i = 0; i < size; i++) {
		elements[i] -= other(i);
	}
}
void Vector::operator*=(Vector other) {
	if (CheckForErrors(other.size)) return;
	for (int i = 0; i < size; i++) {
		elements[i] *= other(i);
	}
}
void Vector::operator*=(double other) {
	for (int i = 0; i < size; i++) {
		elements[i] *= other;
	}
}
void Vector::operator/=(double other) {
	for (int i = 0; i < size; i++) {
		elements[i] /= other;
	}
}

double& Vector::operator()(int i) {
	return elements[i];
}

Vector Vector::ForEach(double (*iterativeFunc)(double)) {
	Vector vec(size);
	for (int i = 0; i < vec.size; i++) {
		vec(i) = iterativeFunc((*this)(i));
	}
	return vec;
}
double Vector::Dot(Vector other) {
	if (CheckForErrors(other.size)) return 0;

	double dotProduct = 0;
	for (int i = 0; i < size; i++) {
		dotProduct += elements[i] * other(i);
	}
	return dotProduct;
}
double Vector::Magnitude() const {
	double magnitude = 0;
	for (auto& element : elements) {
		magnitude += element * element;
	}
	return sqrt(magnitude);
}
double Vector::MagnitudeSqr() const {
	double magnitudeSqr = 0;
	for (auto& element : elements) {
		magnitudeSqr += element * element;
	}
	return magnitudeSqr;
}
int Vector::MaxIndex() const {
	int highestIndex = 0;
	double highestValue = elements[0];
	for (int i = 0; i < size; i++) {
		if (elements[i] > highestValue) {
			highestValue = elements[i];
			highestIndex = i;
		}
	}
	return highestIndex;
}

void Vector::SetZero() {
	std::fill(elements.begin(), elements.end(), 0);
}
void Vector::Print() const {
	std::cout << '(';
	for (int i = 0; i < elements.size()-1; i++) {
		std::cout << elements[i] << ", ";
	}
	std::cout << elements[size-1] << ")\n";
}

bool Vector::CheckForErrors(int otherSize) {
	if (otherSize != size) {
		std::cerr << "Both vectors are of different dimension.\n";
		return true;
	}
	return false;
}