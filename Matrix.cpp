#include <iostream>
#include "Matrix.h"
#include <random>
#include <cmath>

#define LOG(x) std::cout << " " << x;


//INTERNAL MATRIX CLUSS FUNCITONALITY
//===================================================================================//
Matrix::Matrix(int rows, int cols) {
	//Create Matrix object with rows, columns
	//Intialise all vlaues by reading in an array
	this->rows = rows;
	this->columns = cols;

	data = new double[rows * cols];
}

Matrix::~Matrix() {
	//Destroy the Matrix object to free the memory
	delete[] data;
}

Matrix::Matrix(const Matrix& other) {
	//Copy constructor to prevent the compilation error during Matrix operations
	this->rows = other.rows;
	this->columns = other.columns;

	data = new double[rows * columns];
	for (int i = 0; i < rows * columns; i++) {
		data[i] = other.data[i];
	}
}

void Matrix::SetZero() {
	for (int i = 0; i < rows * columns; i++) {
		data[i] = 0.0;
	}
}

void Matrix::Print() {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			LOG(data[i * columns + j])
		}
		std::cout << std::endl;
	}
}

void Matrix::SetFromArray(const double* inputData) {
	for (int i = 0; i < rows * columns; i++) {
		data[i] = inputData[i];
	}
}

void Matrix::SetIdentity() {
	this->SetZero();
	if (rows == columns) {
		for (int i = 0; i < rows; i++) {
			data[i * columns + i] = 1.0;
		}
	}
	else {
		std::cout << "NON-Square matrix -- Identity ERROR" << std::endl;
	}
}

void Matrix::SetXavier(int fanIn, int fanOut) {
	double mean = 0.0;
	double limit = std::sqrt(6.0 / (fanIn + fanOut));

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(-limit, limit);

	for (int i = 0; i < rows * columns; i++) {
		data[i] = dist(mt);
		//std::cout << data[i];
	}
}

Matrix Matrix::T() const {
	//Function to trnaspose the matrix. Returns a trnaposed matrix
	Matrix result(columns, rows);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			result(j, i) = data[i * columns + j];
		}
	}
	return result;
}

void Matrix::ApplyFunction(double (*func) (double)) {
	for (int i = 0; i < rows * columns; i++) {
		data[i] = func(data[i]);
	}
}

double Matrix::Sum() {
	double sum = 0;
	for (int i = 0; i < this->rows * this->columns; i++) {
		sum += this->data[i];
	}

	return sum;
}

//MATRIX OPERATORS
//============================================================================//

// 1D access (vector)
double& Matrix::operator()(int index) {
    if (rows != 1 && columns != 1) {
        std::cout << "ERROR: operator()(int) called on non-vector matrix!" << std::endl;
        std::cout << "Matrix is " << rows << "x" << columns << ", expected 1xN or Nx1" << std::endl;
        throw std::runtime_error("Invalid matrix dimensions for 1D access");
    }
    // Bounds check for 1D index
    if (index < 0 || index >= rows * columns) {
        std::cout << "ERROR: Index " << index << " out of bounds!" << std::endl;
        std::cout << "Matrix size is " << rows * columns << std::endl;
        throw std::out_of_range("Index out of bounds");
    }
    return data[index];
}

double Matrix::operator()(int index) const {
    if (rows != 1 && columns != 1) {
        std::cout << "ERROR: operator()(int) called on non-vector matrix!" << std::endl;
        std::cout << "Matrix is " << rows << "x" << columns << ", expected 1xN or Nx1" << std::endl;
        throw std::runtime_error("Invalid matrix dimensions for 1D access");
    }
    if (index < 0 || index >= rows * columns) {
        std::cout << "ERROR: Index " << index << " out of bounds!" << std::endl;
        std::cout << "Matrix size is " << rows * columns << std::endl;
        throw std::out_of_range("Index out of bounds");
    }
    return data[index];
} 

// 2D access
double& Matrix::operator()(int row, int col) {
    if (row < 0 || row >= rows || col < 0 || col >= columns) {
        std::cout << "ERROR: Matrix index out of bounds!" << std::endl;
        std::cout << "Ind ex (" << row << ", " << col << ") ";
        std::cout << "on matrix " << rows << "x" << columns << std::endl;
        throw std::out_of_range("Matrix index out of bounds");
    }
    return data[row * columns + col];
}

double Matrix::operator()(int row, int col) const {
    if (row < 0 || row >= rows || col < 0 || col >= columns) {
        std::cout << "ERROR: Matrix index out of bounds!" << std::endl;
        std::cout << "Index (" << row << ", " << col << ") ";
        std::cout << "on matrix " << rows << "x" << columns << std::endl;
        throw std::out_of_range("Matrix index out of bounds");
    }
    return data[row * columns + col];
}

Matrix& Matrix::operator=(const Matrix& other) {
    if (this == &other) {
        return *this;
    }

    delete[] data;
    rows = other.rows;
    columns = other.columns;

    data = new double[rows * columns];

    for (int i = 0; i < rows * columns; i++) {
        data[i] = other.data[i];
    }
    return *this;
}

// Arithmetic operators
Matrix Matrix::operator+(const Matrix& other) const {
    return Add(*this, other);
}

Matrix Matrix::operator*(const Matrix& other) const {
    return Multiply(*this, other);
}

Matrix Matrix::operator-(const Matrix& other) const {
    return Subtract(*this, other);
}

// In-place operators
Matrix& Matrix::operator+=(const Matrix& other) {
    if (rows != other.rows || columns != other.columns) {
		throw std::runtime_error("Invalid matrix dimensions for += operation");
    }

    for (int i = 0; i < rows * columns; i++) {
        data[i] += other.data[i];
    }
    return *this;
}

Matrix& Matrix::operator*=(const Matrix& other) {
    if (rows != other.rows || columns != other.columns) {
		throw std::runtime_error("Invalid matrix dimensions for *= operation");
    }

    for (int i = 0; i < rows * columns; i++) {
        data[i] *= other.data[i];
    }
    return *this;
}


//Scalar operators
Matrix& Matrix::operator*=(double scalar) {
    for (int i = 0; i < rows * columns; i++) {
        data[i] *= scalar;
    }
    return *this;
}


Matrix& Matrix::operator-=(const Matrix& other) {
	if (rows != other.rows || columns != other.columns) {
		throw std::runtime_error("Invalid matrix dimensions for -= operation");
	}

	for (int i = 0; i < rows * columns; i++) {
		data[i] -= other.data[i];
	}
	return *this;
}

Matrix Matrix::operator+(double scalar) const {
	Matrix Mat(rows, columns);

	for (int i = 0; i < rows * columns; i++) {
		Mat.data[i] = data[i] + scalar;
	}
	return Mat;
}

Matrix Matrix::operator*(double scalar) const {
	Matrix Mat(rows, columns);

	for (int i = 0; i < rows * columns; i++) {
		Mat.data[i] = data[i] * scalar;
	}
	return Mat;
}

Matrix Matrix::operator-(double scalar) const {
	Matrix Mat(rows, columns);

	for (int i = 0; i < rows * columns; i++) {
		Mat.data[i] = data[i] - scalar;
	}
	return Mat;
}

Matrix Matrix::operator/(double scalar) const {
	if (scalar == 0) {
		throw std::runtime_error("Invalid matrix dimensions for / operation");
	}

	Matrix Mat(rows, columns);

	for (int i = 0; i < rows * columns; i++) {
		Mat.data[i] = data[i] / scalar;
	}
	return Mat;
}

Matrix Matrix::operator^(double power) {
	Matrix Mat(rows, columns);
	for (int i = 0; i < rows * columns; i++) {
		Mat.data[i] = std::pow(this->data[i], power);
	}

	return Mat;
}


Matrix Matrix::EWiseMultiply(const Matrix& other) {
	Matrix result(*this);
	result *= other;

	return result;
}

//FREE FUNCTIONS FOR MATRIX OPERATIONS
//============================================================================//
Matrix Add(const Matrix& A, const Matrix& B) {
	if (A.rows != B.rows || A.columns != B.columns) {
		throw std::runtime_error("Invalid Matrix dimensional mismatch for Add operation");
	}

	Matrix Mat(A.rows, A.columns);

	for (int i = 0; i < A.rows * A.columns; i++) {
		Mat.data[i] = A.data[i] + B.data[i];
	}
	return Mat;
}

Matrix Multiply(const Matrix& A, const Matrix& B) {
	if (A.columns != B.rows) {
		throw std::runtime_error("Invalid Matrix dimensional mismatch for Multiply operation");
	}

	Matrix Mat(A.rows, B.columns);

	// O(N^3) !!!!!! WOw
	for (int i = 0; i < A.rows; i++) {
		for (int j = 0; j < B.columns; j++) {
			double sum = 0.0;
			for (int k = 0; k < A.columns; k++) {
				sum += A.data[i * A.columns + k] * B.data[j + B.columns * k];
			}
			Mat.data[i * Mat.columns + j] = sum;
			sum = 0.0;
		}
	}

	return Mat;
}

Matrix Subtract(const Matrix& A, const Matrix& B) {
	if (A.rows != B.rows || A.columns != B.columns) {
		throw std::runtime_error("Invalid Matrix dimensional mismatch for Subtract operation");
	}

	Matrix Mat(A.rows, A.columns);

	for (int i = 0; i < A.rows * A.columns; i++) {
		Mat.data[i] = A.data[i] - B.data[i];
	}
	return Mat;
}

Matrix operator+(double scalar, const Matrix& Mat) {
	return Mat + scalar;
}

Matrix operator*(double scalar, const Matrix& Mat) {
	return Mat * scalar;
}

Matrix operator-(double scalar, const Matrix& Mat) {
	Matrix result(Mat.rows, Mat.columns);

	for (int i = 0; i < Mat.rows * Mat.columns; i++) {
		result.data[i] = scalar - Mat.data[i];
	}
	return result;
}

Matrix operator/(double scalar, const Matrix& Mat) {
	if (scalar == 0) {
		throw std::runtime_error("Invalid matrix dimensions for / operation");
	}

	Matrix result(Mat.rows, Mat.columns);

	for (int i = 0; i < Mat.rows * Mat.columns; i++) {
		result.data[i] = scalar / Mat.data[i];
	}
	return result;
}