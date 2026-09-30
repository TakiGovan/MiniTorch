#pragma once
#include <iostream>

class Matrix {
public:
	int columns;
	int rows;
	double* data;

	//Constructor, copy constructor and destructor (DONE)
	Matrix(int rows, int columns);
	Matrix(const Matrix& other);
	~Matrix();

	Matrix T() const; // Traspose the matrix
	void ApplyFunction(double (*func) (double));
	double Sum();

	//Operators//
	//Element access 1D
	double& operator()(int index);
	double operator()(int index) const;
	//2D
	double& operator()(int row, int col); //Done
	double operator()(int row, int col) const; //Done

	//Arithmatic ops
	Matrix& operator=(const Matrix& other); //Done
	Matrix operator+(const Matrix& other) const; //Done
	Matrix operator*(const Matrix& other) const; //Done
	Matrix operator-(const Matrix& other) const; //Done
	Matrix operator+(double scalar) const; // DOne
	Matrix operator*(double scalar) const; // Done
	Matrix operator-(double scalar) const; // Done
	Matrix operator/(double scalar) const; // Done
	Matrix operator^(double power);
	Matrix EWiseMultiply(const Matrix& other);

	//In place operators
	Matrix& operator+=(const Matrix& other);
	Matrix& operator*=(const Matrix& other);
	Matrix& operator*=(double scalar);
	Matrix& operator-=(const Matrix& other);

	//Initalise data
	void SetZero();
	void SetFromArray(const double* inputData);
	void SetXavier(int fanIn, int fanOut);
	void SetIdentity();

	//Output Mat Data
	void Print();
};

Matrix Add(const Matrix& A, const Matrix& B);
Matrix Multiply(const Matrix&, const Matrix& B);
Matrix Subtract(const Matrix&, const Matrix& B);

Matrix operator+(double scalar, const Matrix& Mat);
Matrix operator*(double scalar, const Matrix& Mat);
Matrix operator-(double scalar, const Matrix& Mat);
Matrix operator/(double scalar, const Matrix& Mat);