#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include "Matrix.h"
#include <unordered_set>

class Tensor {
private:
	Matrix data_;
	Matrix grads_;

	bool useGrad_;

public:
	Tensor(int rows, int cols, double* data = nullptr, bool useGrad = false) : data_(rows, cols), grads_(rows, cols), useGrad_(useGrad) {
		grads_.SetZero();
		
		if (data != nullptr) { 
			data_.SetFromArray(data); 
		}
	}
	Tensor(const Tensor& other);

	//Back prop autograd engine stuff
	std::vector<Tensor*> parents_;
	std::function<void()> backwardFn;

	void backward();
	
	//Tensor();
	Tensor& operator=(const Tensor& other);
	Tensor operator+(Tensor& other);
	Tensor operator*(Tensor& other);
	Tensor operator-(Tensor& other);
	Tensor operator^(double power);

	void SetGrads(double* grads) { grads_.SetFromArray(grads); }
	void ZeroGrads() { grads_.SetZero(); }

	void Print() { data_.Print(); }
	void PrintG() { grads_.Print(); }

	int Rows() const { return data_.rows; }
	int Cols() const { return data_.columns; }

	bool RequiresGrad() const { return useGrad_; }

	double GetData(int i, int j) const { return data_(i, j); }
	double GetGrad(int i, int j) const { return grads_(i, j); }

	Tensor Sum();
};

//backprop funcitonality
void dfs(Tensor& Node, std::vector<Tensor*>& results, std::unordered_set<Tensor*>& visited);
std::vector<Tensor*> TopoSort_(Tensor& RootNode);