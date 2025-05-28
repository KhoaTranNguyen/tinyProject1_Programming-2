// LinearSystem.hpp
#pragma once
#include "Matrix.hpp"
#include "Vector.hpp"
#include <stdexcept>

class LinearSystem {
protected:
    int mSize;
    Matrix* mpA;
    Vector* mpb;

    LinearSystem() = delete;
    LinearSystem(const LinearSystem&) = delete;
    LinearSystem& operator=(const LinearSystem&) = delete;

public:
    LinearSystem(Matrix* A, Vector* b)
    : mpA(A), mpb(b) {
        if (A->rows() != A->cols()) throw std::invalid_argument("Matrix must be square");
        if (A->rows() != b->size()) throw std::invalid_argument("Incompatible matrix/vector sizes");
        mSize = A->rows();
    }
    virtual ~LinearSystem() = default;

    virtual Vector solve() {return mpA->solve(*mpb);}; // Gaussian elimination
};