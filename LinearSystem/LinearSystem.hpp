// LinearSystem.hpp
#pragma once
#include "../Matrix/Matrix.hpp"
#include "../Vector/Vector.hpp"
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
    LinearSystem(Matrix* A, Vector* b);
    virtual ~LinearSystem() = default;

    virtual Vector solve(); // Gaussian elimination
};