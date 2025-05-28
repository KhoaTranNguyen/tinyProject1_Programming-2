#include "PosSymLinSystem.hpp"

PosSymLinSystem::PosSymLinSystem(Matrix* A, Vector* b)
    : LinearSystem(A, b) {
    if (!A->isSymmetric()) throw std::invalid_argument("Matrix is not symmetric");
}

Vector PosSymLinSystem::solve() {
    // Conjugate Gradient algorithm placeholder
    return mpA->conjugateGradient(*mpb);
}