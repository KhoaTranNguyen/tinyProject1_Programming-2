#include <iostream>
#include <string>
#include <cmath>
#include <stdexcept>
using namespace std;

static bool debug = false;

struct DebugScope {
    string name;
    DebugScope(const string& n) : name(n) {
        if (debug) cout << "\n[ Entering scope: " << name << endl;
    }
    ~DebugScope() {
        if (debug) cout << "Exiting scope: " << name << " ]" << endl;
    }
};

class Matrix {
private:
    int mNumRows;
    int mNumCols;
    double** mData;
    string mName;

public:

    // Constructor with optional debug name
    Matrix(int row = 0, int col = 0, const string& name = "")
        : mNumRows(row), mNumCols(col), mName(name) {
        if (mNumRows > 0 && mNumCols > 0) {
            mData = new double*[mNumRows];
            for (int i = 0; i < mNumRows; i++) {
                mData[i] = new double[mNumCols];
                for (int j = 0; j < mNumCols; j++) {
                    mData[i][j] = 0;
                }
            }
        } else {
            mData = nullptr;
        }
        if (debug) cout << "Constructor: Matrix " << (mName.empty() ? "<unnamed>" : mName)
             << " (" << mNumRows << "x" << mNumCols << ")" << endl;
    }

    // Constructor from initializer list (user-friendly)
    Matrix(std::initializer_list<std::initializer_list<double>> initList) {
        mNumRows = initList.size();
        mNumCols = initList.begin()->size();

        // Allocate memory
        mData = new double*[mNumRows];
        for (int i = 0; i < mNumRows; ++i) {
            mData[i] = new double[mNumCols];
        }

        // Copy values
        int row = 0;
        for (const auto& rowList : initList) {
            if (rowList.size() != static_cast<size_t>(mNumCols)) {
                throw std::invalid_argument("All rows must have the same number of columns.");
            }
            int col = 0;
            for (double val : rowList) {
                mData[row][col++] = val;
            }
            ++row;
        }

        mName = "<unnamed>";
        if (debug) std::cout << "Constructor: Matrix " << mName
                << " (" << mNumRows << "x" << mNumCols << ")\n";
    }
    
    Matrix(const std::string& name, std::initializer_list<std::initializer_list<double>> initList)
    : mName(name), mNumRows(initList.size()), mNumCols(initList.begin()->size()) {
        // Allocate and copy data
        mData = new double*[mNumRows];
        for (int i = 0; i < mNumRows; ++i) {
            mData[i] = new double[mNumCols];
            auto row = *(initList.begin() + i);
            std::copy(row.begin(), row.end(), mData[i]);
        }
        if (debug) std::cout << "Constructor: Matrix " << mName << " (" << mNumRows << "x" << mNumCols << ")\n";
    }

    // Copy constructor
    Matrix(const Matrix& other)
        : mNumRows(other.mNumRows), mNumCols(other.mNumCols),
          mName(other.mName.empty() ? "" : other.mName + "_copy") {
        if (mNumRows > 0 && mNumCols > 0) {
            mData = new double*[mNumRows];
            for (int i = 0; i < mNumRows; i++) {
                mData[i] = new double[mNumCols];
                for (int j = 0; j < mNumCols; j++) {
                    mData[i][j] = other.mData[i][j];
                }
            }
        } else {
            mData = nullptr;
        }
        if (debug) cout << "Copy Constructor: Matrix " << (mName.empty() ? "<unnamed>" : mName) << endl;
    }

    // Move constructor
    Matrix(Matrix&& other) noexcept
    : mNumRows(other.mNumRows), mNumCols(other.mNumCols),
        mData(other.mData), mName(std::move(other.mName)) {
        other.mData = nullptr;
        other.mNumRows = 0;
        other.mNumCols = 0;
        if (debug) std::cout << "Move Constructor: Matrix " << (mName.empty() ? "<unnamed>" : mName) << " has been moved\n";
    }

    Matrix& operator=(Matrix&& other) noexcept {
        if (this != &other) {
            // Free old memory
            for (int i = 0; i < mNumRows; ++i)
                delete[] mData[i];
            delete[] mData;

            // Transfer ownership
            mData = other.mData;
            mNumRows = other.mNumRows;
            mNumCols = other.mNumCols;

            // Nullify source
            other.mData = nullptr;
            other.mNumRows = 0;
            other.mNumCols = 0;
        }
        return *this;
    }

    // Destructor
    ~Matrix() {
        if (debug) cout << "Destructor: Matrix " << (mName.empty() ? "<unnamed>" : mName) << " has been deleted" << endl;
        if (mData) {
            for (int i = 0; i < mNumRows; i++) {
                delete[] mData[i];
            }
            delete[] mData;
        }
    }

    // Access element (1-based) - mutable
    double& operator()(const int& row, const int& col) {
        if (row < 1 || row > mNumRows || col < 1 || col > mNumCols) {
            throw out_of_range("\nError: The matrix index (" + to_string(row) + ", " + to_string(col) + ") is out of range.");
        }
        return mData[row - 1][col - 1];
    }

    // Access element (1-based) - const
    const double operator()(const int& row, const int& col) const {
        if (row < 1 || row > mNumRows || col < 1 || col > mNumCols) {
            throw out_of_range("\nError: The matrix index (" + to_string(row) + ", " + to_string(col) + ") is out of range.");
        }
        return mData[row - 1][col - 1];
    }

    const int rows() const { return mNumRows; }
    const int cols() const { return mNumCols; }
    const pair<int, int> shape() const { return {mNumRows, mNumCols}; }
    

    // WARNING: Since C++17 has RVO and NRVO function, don't need to add debug scope for +, -, *
    // Addition
    Matrix operator+(const Matrix& other) const {
        if (shape() != other.shape()) {
            throw runtime_error("\nError: Cannot add matrices that have different sizes.\n");
        }
        Matrix new_Matrix(mNumRows, mNumCols);
        for (int i = 0; i < mNumRows; i++)
            for (int j = 0; j < mNumCols; j++)
                new_Matrix.mData[i][j] = mData[i][j] + other.mData[i][j];
        return new_Matrix;
    }

    // Subtraction
    Matrix operator-(const Matrix& other) const {
        if (shape() != other.shape()) {
            throw runtime_error("\nError: Cannot subtract matrices that have different sizes.\n");
        }
        Matrix new_Matrix(mNumRows, mNumCols);
        for (int i = 0; i < mNumRows; i++)
            for (int j = 0; j < mNumCols; j++)
                new_Matrix.mData[i][j] = mData[i][j] - other.mData[i][j];
        return new_Matrix;
    }

    // Scalar multiplication
    Matrix operator*(const double& scalar) const {
        Matrix new_Matrix(mNumRows, mNumCols);
        for (int i = 0; i < mNumRows; i++)
            for (int j = 0; j < mNumCols; j++)
                new_Matrix.mData[i][j] = mData[i][j] * scalar;
        return new_Matrix;
    }

    // Matrix multiplication
    Matrix operator*(const Matrix& other) {
        if (mNumCols != other.mNumRows) {
            throw runtime_error("\nError: Cannot multiply matrices that do not have suitable sizes.\n");
        }
        Matrix new_Matrix(mNumRows, other.mNumCols);
        for (int i = 0; i < mNumRows; i++) {
            for (int j = 0; j < other.mNumCols; j++) {
                new_Matrix.mData[i][j] = 0;
                for (int k = 0; k < mNumCols; k++) {
                    new_Matrix.mData[i][j] += mData[i][k] * other.mData[k][j];
                }
            }
        }
        return new_Matrix;
    }

    // Determinant
    const double det() const {
        DebugScope scope((mName.empty() ? "Matrix::det" : mName + "::det"));

        Matrix mat = (*this);
        if (mat.mNumRows != mat.mNumCols) {
            throw runtime_error("\nError: Cannot calculate the determinant of a non-square matrix.");
        }

        int n = mat.mNumCols;

        for (int j = 0; j < n; j++) {
            if (abs(mat(j + 1, j + 1)) < 1e-10) {
                throw runtime_error("\nError: Zero pivot encountered during LU decomposition.");
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

    // Inverse
    Matrix inverse(const string& name = "") const {
        DebugScope scope((mName.empty() ? "Matrix::inverse" : mName + "::inverse"));

        if (mNumRows != mNumCols) {
            throw runtime_error("\nError: Only square matrices can be inverted.");
        }

        int n = mNumRows;
        if (n == 2) {
            double d = (*this)(1, 1) * (*this)(2, 2) - (*this)(1, 2) * (*this)(2, 1);
            if (abs(d) < 1e-12) {
                throw runtime_error("\nError: Determinant is zero. Matrix is not invertible.");
            }
            Matrix inv(2, 2, "inv");
            inv(1, 1) = (*this)(2, 2) / d;
            inv(1, 2) = -(*this)(1, 2) / d;
            inv(2, 1) = -(*this)(2, 1) / d;
            inv(2, 2) = (*this)(1, 1) / d;
            return inv;
        }

        Matrix aug(n, 2 * n, "aug");
        // Augment [A | I]
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j)
                aug(i + 1, j + 1) = (*this)(i + 1, j + 1);
            aug(i + 1, n + i + 1) = 1.0;
        }

        // Gauss-Jordan elimination
        for (int i = 0; i < n; ++i) {
            int maxRow = i;
            for (int k = i + 1; k < n; ++k) {
                if (abs(aug(k + 1, i + 1)) > abs(aug(maxRow + 1, i + 1)))
                    maxRow = k;
            }

            if (abs(aug(maxRow + 1, i + 1)) < 1e-12)
                throw runtime_error("\nError: Matrix is singular or near-singular.");

            if (i != maxRow) {
                for (int j = 0; j < 2 * n; ++j)
                    swap(aug(i + 1, j + 1), aug(maxRow + 1, j + 1));
            }

            double diag = aug(i + 1, i + 1);
            for (int j = 0; j < 2 * n; ++j)
                aug(i + 1, j + 1) /= diag;

            for (int k = 0; k < n; ++k) {
                if (k == i) continue;
                double factor = aug(k + 1, i + 1);
                for (int j = 0; j < 2 * n; ++j)
                    aug(k + 1, j + 1) -= factor * aug(i + 1, j + 1);
            }
        }

        Matrix result(n, n, name);
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                result(i + 1, j + 1) = aug(i + 1, j + n + 1);

        return std::move(result);
    }

    friend ostream& operator<<(ostream& os, const Matrix& mat) {
        for (int i = 0; i < mat.mNumRows; i++) {
            for (int j = 0; j < mat.mNumCols; j++) {
                os << mat.mData[i][j] << (j == mat.mNumCols - 1 ? "\n" : " ");
            }
        }
        return os;
    }
};

// Macro for named matrix creation
#define NAMED_MATRIX(var, rows, cols) Matrix var(rows, cols, #var)
#define DECLARE_MATRIX(name, ...) Matrix name(#name, { __VA_ARGS__ })

// Non-member scalar multiplication
Matrix operator*(const double& scalar, const Matrix& mat) {
    return mat * scalar;
}

int main() {
    // debug = true;

    cout << "=== Matrix Class Tests ===" << endl; 

    NAMED_MATRIX(A, 3, 3);
    A(1, 1) = 2; A(1, 2) = 1; A(1, 3) = 0;
    A(2, 1) = 1; A(2, 2) = 2; A(2, 3) = 1;
    A(3, 1) = 0; A(3, 2) = 1; A(3, 3) = 2;

    cout << "\nMatrix A:\n" << A;

    DECLARE_MATRIX(V,
        {2, 1, 0},
        {1, 2, 1},
        {0, 1, 2}
    );

    cout << "\nMatrix V:\n" << V;

    double detA = A.det();
    cout << "\nDeterminant of A: " << detA << endl;

    Matrix B = A.inverse("B");
    cout << "\nInverse of A:\n" << B;

    cout << "\n=== Program ended. " << (debug ? "Destructors called" : "") << "===\n";

    return 0;
}