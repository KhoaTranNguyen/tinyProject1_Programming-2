#include "LeastSquaresSystem.hpp"

// LeastSquaresSystem.cpp
LeastSquaresSystem::LeastSquaresSystem(Matrix* A, Vector* b, double lambda)
    : LinearSystem(A, b), lambda(lambda) {}

Vector LeastSquaresSystem::solve() {
    if (lambda == 0.0) {
        return mpA->pseudoinverse() * (*mpb);
    } else {
        Matrix At = mpA->transpose();
        Matrix ATA = At * (*mpA);
        Matrix regularized = ATA + lambda * Matrix::identity(ATA.rows());
        return regularized.inverse() * (At * (*mpb));
    }
}