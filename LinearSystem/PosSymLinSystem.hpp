// PosSymLinSystem.hpp
#pragma once
#include "LinearSystem.hpp"

class PosSymLinSystem : public LinearSystem {
public:
    PosSymLinSystem(Matrix* A, Vector* b);
    Vector solve() override; // Conjugate Gradient
};