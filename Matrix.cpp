#include <iostream>
#include <cmath>
#include "Vector\Vector.hpp"
using namespace std;

class Matrix {
    private:
        int mNumRows;
        int mNumCols;
        double** mData;
    public:
        // Default constructor
        Matrix() : mData(nullptr), mNumRows(0), mNumCols(0) {}
        
        // Copy constructor with deep copy
        Matrix(const Matrix& other) : mNumRows(other.mNumRows), mNumCols(other.mNumCols) {
            this->mData = new double*[mNumRows];
            for (int i = 0; i < this->mNumRows; i++) {
                this->mData[i] = new double[mNumCols];
                for (int j = 0; j < this->mNumCols; j++) {
                    this->mData[i][j] = other.mData[i][j];
                }
            }
        }

        // Input constructor that accept mNumRows and mNumCols
        Matrix(int row, int col) : mNumRows(row), mNumCols(col) {
            this->mData = new double*[mNumRows];
            for (int i = 0; i < this->mNumRows; i++) {
                this->mData[i] = new double[mNumCols];
                for (int j = 0; j < this->mNumCols; j++) {
                    this->mData[i][j] = 0; // Create row.col "zero" matrix
                }
            }
        }

        // Default constructor
        ~Matrix() {
            cout << "\n>> Destructor: The called matrix which has the (0,0) is " << mData[0][0] << " has been deleted" << endl;
            for (int i = 0; i < mNumRows; i++) {
                delete[] mData[i];
            }
            delete[] mData;
        }

        // Accessing element (1 based) that allows modification
        double& operator()(const int& row, const int& col) {
            if (row < 1 || row > this->mNumRows || col < 1 || col > this->mNumCols) {
                throw out_of_range("Error: The matrix index (" + to_string(row) + ", " + to_string(col) + ") is out of range.");
            }
            return this->mData[row-1][col-1];
        }
        
        // Accessing element (1 based) that does not allow modification
        const double operator()(const int& row, const int& col) const {
            if (row < 1 || row > this->mNumRows || col < 1 || col > this->mNumCols) {
                throw out_of_range("Error: The matrix index (" + to_string(row) + ", " + to_string(col) + ") is out of range.");
            }
            return this->mData[row-1][col-1];
        }

        // Accessing the number of rows and columns
        const int rows() const {
            return this->mNumRows;
        }

        const int cols() const {
            return this->mNumCols;
        }

        const std::pair<int, int> shape() const {
            return {mNumRows, mNumCols};
        }

        // Addition
        Matrix operator+(const Matrix& other) const {
            if (this->shape() != other.shape()) {
                throw runtime_error("Error: Cannot add matrices that have different sizes.\n");
            }
            else {
                Matrix new_Matrix(mNumRows, mNumCols);  // Create row.col "zero" matrix
                for (int i = 0; i < this->mNumRows; i++) {
                    for (int j = 0; j < this->mNumCols; j++) {
                        new_Matrix.mData[i][j] = this->mData[i][j] + other.mData[i][j];
                    }
                }
                return new_Matrix;
            }
        }

        // Subtracion
        Matrix operator-(const Matrix& other) const {
            if (this->shape() != other.shape()) {
                throw runtime_error("Error: Cannot subtract matrices that have different sizes.\n");
            }
            else {
                Matrix new_Matrix(mNumRows, mNumCols);  // Create row.col "zero" matrix
                for (int i = 0; i < this->mNumRows; i++) {
                    for (int j = 0; j < this->mNumCols; j++) {
                        new_Matrix.mData[i][j] = this->mData[i][j] - other.mData[i][j];
                    }
                }
                return new_Matrix;
            }
        }

        // Scalar multiplication
        Matrix operator*(const double& scalar) const {
            Matrix new_Matrix(mNumRows, mNumCols);  // Create row.col "zero" matrix
            for (int i = 0; i < this->mNumRows; i++) {
                for (int j = 0; j < this->mNumCols; j++) {
                    new_Matrix.mData[i][j] = this->mData[i][j]*scalar;
                }
            }
            return new_Matrix;
        }

        // Matrix multiplication
        Matrix operator*(const Matrix& other) {
            if (this->mNumCols != other.mNumRows) {
                throw runtime_error("Error: Cannot multiply matrices that do not have suitable sizes.\n");
            } else {
                Matrix new_Matrix(this->mNumRows, other.mNumCols);  // (m × p) matrix
                for (int i = 0; i < this->mNumRows; i++) {
                    for (int j = 0; j < other.mNumCols; j++) {
                        for (int k = 0; k < this->mNumCols; k++) {
                            new_Matrix.mData[i][j] += this->mData[i][k] * other.mData[k][j];
                        }
                    }
                }
                return new_Matrix;
            }
        }

        // Matrix-Vector multiplication
        Matrix operator*(const Vector& vec) {
            if (this->mNumCols != vec.size()) {
                throw runtime_error("Error: Cannot multiply vector that does not have a suitable size.\n");
            } else {
                Matrix new_Matrix(this->mNumRows, 1);  // (m × p) matrix
                for (int i = 0; i < this->mNumRows; i++) {
                    for (int j = 0; j < vec.size(); j++) {
                        new_Matrix.mData[i][0] += this->mData[i][j] * vec(j + 1); // vec is 1-based index
                    }
                }
                return new_Matrix;
            }
        }

        // Determinant of a given square matrix using LU Decomposition
        const double det(Matrix mat) const {
            if (mat.mNumRows != mat.mNumCols) {
                throw runtime_error("Error: Cannot calculate the determinant of a non-given square matrix.");
            }

            int n = mat.mNumCols; // Matrix n*n

            // Lu Decomposition - L calculation
            for (int j = 0; j < n; j++) {
                if (abs(mat(j + 1, j + 1)) < 1e-10) {
                    throw runtime_error("Error: Zero pivot encountered during LU decomposition.");
                }
                for (int i = j + 1; i < n; i++) {
                    double x = mat(i + 1, j + 1) / mat(j + 1, j + 1);
                    for (int k = 0; k < n; k++) {
                        mat(i + 1, k + 1) -= x * mat(j + 1, k + 1);
                    }
                }
            }

            double determinant = 1.0;
            for (int j = 0; j < n; j++) {
                determinant *= mat(j + 1, j + 1);
            }

            return determinant;
        }

        // Inverse Matrix
        Matrix inverse(const Matrix& mat) const {
            int n = mat.rows();
            if (n != mat.cols()) {
                throw runtime_error("Error: Only square matrices can be inverted.");
            }

            if (n == 2) {
                double d = mat(1,1)*mat(2,2) - mat(1,2)*mat(2,1);
                if (d == 0) {
                    throw runtime_error("Error: Determinant is zero. Matrix is not invertible.");
                }
                Matrix inv(2, 2);
                inv(1, 1) =  mat(2, 2) / d;
                inv(1, 2) = -mat(1, 2) / d;
                inv(2, 1) = -mat(2, 1) / d;
                inv(2, 2) =  mat(1, 1) / d;
                return inv;
            }

            if (det(mat) == 0) {
                throw std::runtime_error("Error: Matrix is not invertible. Determinant is zero.");
            }

            // Augment [A | I]
            Matrix aug(n, 2 * n);
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j)
                    aug(i, j) = mat(i, j);
                aug(i, n + i) = 1.0;
            }

            // Gauss-Jordan Elimination
            for (int i = 0; i < n; ++i) {
                double diag = aug(i, i);
                if (std::abs(diag) < 1e-12)
                    throw std::runtime_error("Error: Singular matrix encountered during inversion.");

                // Normalize the pivot row
                for (int j = 0; j < 2 * n; ++j)
                    aug(i, j) /= diag;

                // Eliminate other rows
                for (int k = 0; k < n; ++k) {
                    if (k == i) continue;
                    double factor = aug(k, i);
                    for (int j = 0; j < 2 * n; ++j)
                        aug(k, j) -= factor * aug(i, j);
                }
            }

            // Extract right half as inverse
            Matrix result(n, n);
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < n; ++j)
                    result(i, j) = aug(i, j + n);

            return result;
        }

        // Matrix printing out
        friend std::ostream& operator<<(std::ostream& os, const Matrix& mat) {
            // cout << "[";
            for (int i = 0; i < mat.mNumRows; i++) {
                for (int j = 0; j < mat.mNumCols-1; j++) {
                    os << mat.mData[i][j] << " "; // Create row.col "zero" matrix
                }
                os << mat.mData[i][mat.mNumCols-1] << endl;
            }
            // #cout << m.mData[m.mNumRows-1][m.mNumCols-1];
            return os;
        }
};

// Printing out mat.shape()
template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2>& p) {
    os << "{" << p.first << ", " << p.second << "}";
    return os;
}

// Scalar multiplication (scalar * Matrix) - non-member function
Matrix operator*(const double& scalar, const Matrix& mat) {
    return mat * scalar;
}

int main() {
    cout << "=== Matrix Class Tests ===" << endl;

    // Create two 2x2 matrices
    Matrix A(2, 2);
    A(1, 1) = 1; A(1, 2) = 2;
    A(2, 1) = 3; A(2, 2) = 4;

    Matrix B(2, 2);
    B(1, 1) = 5; B(1, 2) = 6;
    B(2, 1) = 7; B(2, 2) = 8;

    // Display matrices
    cout << "\nMatrix A:\n" << A;
    cout << "\nMatrix B:\n" << B;

    // Addition
    Matrix C = A + B;
    cout << "\nC = A + B:\n" << C;

    // // Subtraction
    Matrix D = A - B;
    cout << "\nD = A - B:\n" << D;

    // // Scalar multiplication
    Matrix E = A * 2.0;
    cout << "\nE = A * 2.0:\n" << E;

    // // Matrix multiplication
    Matrix F = A * B;
    cout << "\nF = A * B:\n" << F;

    cout << "\n=== Program ended ===\n";

    return 0;
}