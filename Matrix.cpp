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
            cout << "\n>> Destructor: The called matrix of size " << mNumRows << "x" << mNumCols << " has been deleted" << endl;
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

        // Vector multiplication
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

        // Determinant of a given square matrix
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
            double determinant = 1;
            for (int j = 0; j < n; j++) {
                determinant *= mat(j + 1, j + 1);
            }

            return determinant;
        }

        // Inverse matrix
        Matrix inverse() const {
            if (det((*this)) == 0) {
                throw runtime_error("Error: The matrix is not invertible.");
            }
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
    cout << "Creating Matrix A (2x3):" << endl;
    Matrix A(2, 3);
    cout << "Matrix A:\n" << A;

    cout << "\nCreating Matrix B as a copy of A:" << endl;
    Matrix B(A);
    cout << "Matrix B:\n" << B;

    cout << B.shape();
    
    cout << "\nExiting main, destructors will be called:" << endl;
    return 0;
}