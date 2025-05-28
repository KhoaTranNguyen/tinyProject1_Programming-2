// PosSymLinSystem.hpp
#pragma once
#include "LinearSystem.hpp"

class PosSymLinSystem : public LinearSystem {
public:
    PosSymLinSystem(Matrix* A, Vector* b)
    : LinearSystem(A, b) {
        if (!A->isSymmetric()) throw std::invalid_argument("Matrix is not symmetric");
    }

    Vector solve() override {
        return mpA->conjugateGradient(*mpb);
    }; // Conjugate Gradient
};