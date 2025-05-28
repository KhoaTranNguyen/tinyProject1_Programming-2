// LeastSquaresSystem.hpp
#pragma once
#include "LinearSystem.hpp"

class LeastSquaresSystem : public LinearSystem {
private:
    double lambda;

public:
    LeastSquaresSystem(Matrix* A, Vector* b, double lambda = 0.0)
    : LinearSystem(A, b), lambda(lambda) {}

    Vector solve() override {
        if (lambda == 0.0) {
            return mpA->pseudoinverse() * (*mpb);
        } else {
            Matrix At = mpA->transpose();
            Matrix ATA = At * (*mpA);
            Matrix regularized = ATA + lambda * Matrix::identity(ATA.rows());
            return regularized.inverse("reginv") * (At * (*mpb));
        }
    }; // Pseudo-inverse or Tikhonov
};