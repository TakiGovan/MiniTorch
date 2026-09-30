#include <iostream>
#include <vector>
#include <functional>
#include <unordered_set>
#include "Matrix.h"
#include "Tensor.h"

Tensor MSE(Tensor& pred, Tensor& target) {
	Tensor Loss = ((target - pred) ^ 2).Sum();

	return Loss;
}