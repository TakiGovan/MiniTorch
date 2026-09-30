
#include <iostream>
#include <vector>
#include <functional>
#include "Matrix.h"
#include "Tensor.h"
#include <unordered_set>



//Copy constructor
Tensor::Tensor(const Tensor& other) :
	data_(other.data_),
	grads_(other.grads_),
	useGrad_(other.useGrad_) {

}

////Default constructor
//Tensor::Tensor() : data_(0, 0), grads_(0, 0) {
//	this->useGrad_ = false;
//}

//Operators
//=======================================================

//Operator =
Tensor& Tensor::operator=(const Tensor& other) {
	if (this == &other) {
        return *this;
    }

	data_ = other.data_;
	grads_ = other.grads_;
	useGrad_ = other.useGrad_;

    return *this;
}

//Operatòr + 
Tensor Tensor::operator+(Tensor& other) {
	Tensor result(this->data_.rows, this->data_.columns, nullptr, this->useGrad_);
	result.data_ = this->data_ + other.data_;
	result.grads_.SetZero();
	result.useGrad_ = this->useGrad_ || other.useGrad_;

	result.parents_.push_back(this);
	result.parents_.push_back(&other);
	
	result.backwardFn = [this, &other, &result]() {
		other.grads_ += result.grads_;
		this->grads_ += result.grads_;
		};

	return result;
}

//operator`* 
Tensor Tensor::operator*(Tensor& other) {
	Tensor result(this->data_.rows, other.data_.columns, nullptr, this->useGrad_);
	result.data_ = this->data_ * other.data_;
	result.grads_.SetZero();
	result.useGrad_ = this->useGrad_ || other.useGrad_;
	
	result.parents_.push_back(this);
	result.parents_.push_back(&other);

	result.backwardFn = [this, &other, &result]() {
		this->grads_ += (other.data_ * result.grads_.T()).T();
		other.grads_ += this->data_.T() * result.grads_;
		};
	
	return result;
}

//operator -
Tensor Tensor::operator-(Tensor& other) {
	Tensor result(this->data_.rows, this->data_.columns, nullptr, this->useGrad_);
	result.data_ = this->data_ - other.data_;
	result.grads_.SetZero();
	result.useGrad_ = this->useGrad_ || other.useGrad_;
	
	result.parents_.push_back(this);
	result.parents_.push_back(&other);

	result.backwardFn = [this, &other, &result]() {
		this->grads_ += result.grads_;
		other.grads_ += result.grads_ * -1;
		};

	return result;
}

Tensor Tensor::operator^(double power) {
	Tensor result(this->data_.rows, this->data_.columns, nullptr, this->useGrad_);
	result.data_ = this->data_ ^ power;

	result.useGrad_ = this->useGrad_;
	result.grads_.SetZero();
	result.parents_.push_back(this);

	result.backwardFn = [this, power, &result]() {
		this->grads_ += (this->data_ ^ (power - 1)).EWiseMultiply(result.grads_) * power;
		};

	return result;
}

Tensor Tensor::Sum() {
	Tensor result(1, 1, nullptr, this->useGrad_);
	result.data_(0, 0) = this->data_.Sum();

	result.parents_.push_back(this);
	result.grads_.SetZero();
	result.backwardFn = [this, &result] {
		for (int i = 0; i < this->grads_.rows * this->grads_.columns; i++) {
			this->grads_(i) += result.grads_(0, 0);
		}

		};

	return result;
}


//External helper and backprop funcitons

void dfs(Tensor& Node, std::vector<Tensor*>& results, std::unordered_set<Tensor*>& visited) {
	if (visited.contains(&Node)) {
		return;
	}

	visited.insert(&Node);

	for (int i = 0; i < Node.parents_.size(); i++) {
		dfs(*Node.parents_[i], results, visited);
	}

	results.push_back(&Node);
}


std::vector<Tensor*> TopoSort_(Tensor& RootNode) {
	std::unordered_set<Tensor*> visited;
	std::vector<Tensor*> results;
	dfs(RootNode, results, visited);

	return results;
}

void Tensor::backward() {
	std::vector<Tensor*> sorted = TopoSort_((*this));
	int size_ = sorted.size();

	for (int i = 0; i < size_; i++) {
		if (sorted[size_ - i - 1]->backwardFn) {
			sorted[size_ - i - 1]->backwardFn();
		}
	}
}