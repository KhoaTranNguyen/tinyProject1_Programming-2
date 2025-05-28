#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <string>
#include <stdexcept>
#include <initializer_list>
#include <Eigen/Dense>
#include <Eigen/IterativeLinearSolvers>
#include "../Vector/Vector.hpp"

extern bool debug;

struct DebugScope {
    std::string name;
    DebugScope(const std::string& n);
    ~DebugScope();
};

class Matrix {
private:
    int mNumRows;
    int mNumCols;
    double** mData;
    std::string mName;

public:
    Matrix(int row = 0, int col = 0, const std::string& name = "");
    Matrix(std::initializer_list<std::initializer_list<double>> initList);
    Matrix(const std::string& name, std::initializer_list<std::initializer_list<double>> initList);
    static Matrix identity(int size);
    Matrix(const Matrix& other);
    Matrix(Matrix&& other) noexcept;
    Matrix& operator=(Matrix&& other) noexcept;
    ~Matrix();

    double& operator()(const int& row, const int& col);
    double operator()(const int& row, const int& col) const;

    int rows() const;
    int cols() const;
    std::pair<int, int> shape() const;
    bool isSymmetric() const;

    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    Matrix operator*(const double& scalar) const;
    Matrix operator*(const Matrix& other);
    Vector operator*(const Vector& other);

    double det() const;
    Matrix inverse(const std::string& name = "") const;
    Matrix transpose() const;
    Matrix pseudoinverse(double tolerance = 1e-16) const;
    Vector conjugateGradient(const Vector& b) const;
    Vector solve(const Vector& b) const;

    Eigen::MatrixXd toEigen() const;
    void fromEigen(const Eigen::MatrixXd& mat);

    friend std::ostream& operator<<(std::ostream& os, const Matrix& mat);
};

Matrix operator*(const double& scalar, const Matrix& mat);
Vector operator*(const Vector& vec, const Matrix& mat);

#define NAMED_MATRIX(var, rows, cols) Matrix var(rows, cols, #var)
#define DECLARE_MATRIX(name, ...) Matrix name(#name, { __VA_ARGS__ })

#endif // MATRIX_H