#include <NeuralNetwork/Maths/Matrix.h>

Matrix::Matrix() : rows(0), cols(0) {}
Matrix::Matrix(int rows, int cols) : rows(rows), cols(cols) {
	elements = std::vector<std::vector<double>>(rows, std::vector<double>(cols, 0));
}
Matrix::Matrix(std::vector<std::vector<double>> elements)
	: elements(elements), rows(elements.size()), cols(elements[0].size()) {}

Matrix Matrix::operator+(Matrix other) {
	Matrix mat = *this;
	if (CheckForErrors(other.rows, other.cols)) return mat;
	mat += other;
	return mat;
}
Matrix Matrix::operator-(Matrix other) {
	Matrix mat = *this;
	if (CheckForErrors(other.rows, other.cols)) return mat;
	mat -= other;
	return mat;
}
Matrix Matrix::operator*(double other) {
	Matrix mat = *this;
	mat *= other;
	return mat;
}
Vector Matrix::operator*(Vector other) {
	Vector vec(rows);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			vec(i) += other(j) * (*this)(i, j);
		}
	}
	return vec;
}

void Matrix::operator+=(Matrix other) {
	if (CheckForErrors(other.rows, other.cols)) return;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			elements[i][j] += other(i, j);
		}
	}
}
void Matrix::operator-=(Matrix other) {
	if (other.rows != rows || other.cols != cols) {
		std::cerr << "Both matrices are of different dimension.\n";
		return;
	}

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			elements[i][j] -= other(i, j);
		}
	}
}
void Matrix::operator*=(double other) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			elements[i][j] *= other;
		}
	}
}

double& Matrix::operator()(int row, int col) {
	return elements[row][col];
}
double Matrix::Get(int row, int col) const {
	return elements[row][col];
}

Matrix Matrix::Transpose() {
	Matrix mat(cols, rows);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			mat(j, i) = elements[i][j];
		}
	}
	return mat;
}
Matrix Matrix::Sign() {
	Matrix mat(cols, rows);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (!elements[i][j]) {
				mat(i, j) = 0;
				continue;
			}
			mat(i, j) = -2 * (int)std::signbit(elements[i][j]) + 1;
		}
	}
	return mat;
}
void Matrix::SetZero() {
	for (auto& row : elements) {
		std::fill(row.begin(), row.end(), 0);
	}
}
void Matrix::Print() {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			std::cout << elements[i][j] << ' ';
		}
		std::cout << '\n';
	}
}

bool Matrix::CheckForErrors(int otherRows, int otherCols) {
	if (otherRows != rows || otherCols != cols) {
		std::cerr << "Both matrices are of different dimension.\n";
		return true;
	}
	return false;
}