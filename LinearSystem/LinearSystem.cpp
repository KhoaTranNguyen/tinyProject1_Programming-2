#include "LinearSystem.hpp"

// LinearSystem.cpp
LinearSystem::LinearSystem(Matrix* A, Vector* b)
    : mpA(A), mpb(b) {
    if (A->rows() != A->cols()) throw std::invalid_argument("Matrix must be square");
    if (A->rows() != b->size()) throw std::invalid_argument("Incompatible matrix/vector sizes");
    mSize = A->rows();
}

Vector LinearSystem::solve() {
    // Gaussian Elimination (placeholder)
    return mpA->solve(*mpb);
}