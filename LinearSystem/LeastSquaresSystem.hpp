// LeastSquaresSystem.hpp
#pragma once
#include "LinearSystem.hpp"

class LeastSquaresSystem : public LinearSystem {
private:
    double lambda;

public:
    LeastSquaresSystem(Matrix* A, Vector* b, double lambda = 0.0);
    Vector solve() override; // Pseudo-inverse or Tikhonov
};