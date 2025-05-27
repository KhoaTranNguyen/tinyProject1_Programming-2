#include "Matrix.hpp"

bool debug = false;

DebugScope::DebugScope(const std::string& n) : name(n) {
    if (debug) std::cout << "\n[ Entering scope: " << name << std::endl;
}
DebugScope::~DebugScope() {
    if (debug) std::cout << "Exiting scope: " << name << " ]" << std::endl;
}

Matrix::Matrix(int row, int col, const std::string& name)
    : mNumRows(row), mNumCols(col), mName(name) {
    if (mNumRows > 0 && mNumCols > 0) {
        mData = new double*[mNumRows];
        for (int i = 0; i < mNumRows; ++i) {
            mData[i] = new double[mNumCols]{};
        }
    } else {
        mData = nullptr;
    }
    if (debug) std::cout << "\n>> Constructor: Matrix " << (mName.empty() ? "<unnamed>" : mName)
                         << " (" << mNumRows << "x" << mNumCols << ")\n";
}

Matrix::Matrix(std::initializer_list<std::initializer_list<double>> initList) {
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
    if (debug) std::cout << "\n>> Constructor: Matrix " << mName
            << " (" << mNumRows << "x" << mNumCols << ")\n";
}

Matrix::Matrix(const std::string& name, std::initializer_list<std::initializer_list<double>> initList)
    : mNumRows(initList.size()), mNumCols(initList.begin()->size()), mName(name) {
    // Allocate and copy data
    mData = new double*[mNumRows];
    int i = 0;
    for (const auto& rowList : initList) {
        mData[i] = new double[mNumCols];
        std::copy(rowList.begin(), rowList.end(), mData[i]);
        ++i;
    }
    if (debug) std::cout << "\n>> Constructor: Matrix " << mName << " (" << mNumRows << "x" << mNumCols << ")\n";
}

// Copy constructor
Matrix::Matrix(const Matrix& other)
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
    if (debug) std::cout << "\n>> Copy Constructor: Matrix " << mName << "\n";
}

// Move constructor
Matrix::Matrix(Matrix&& other) noexcept
    : mNumRows(other.mNumRows), mNumCols(other.mNumCols),
      mData(other.mData), mName(std::move(other.mName)) {
    other.mData = nullptr;
    other.mNumRows = 0;
    other.mNumCols = 0;
    if (debug) std::cout << "\n>> Move Constructor: Matrix " << mName << " has been moved\n";
}

Matrix& Matrix::operator=(Matrix&& other) noexcept {
    if (this != &other) {
        // Free old memory
        for (int i = 0; i < mNumRows; ++i) delete[] mData[i];
        delete[] mData;

        // Transfer ownership
        mData = other.mData;
        mNumRows = other.mNumRows;
        mNumCols = other.mNumCols;
        mName = std::move(other.mName);

        // Nullify source
        other.mData = nullptr;
        other.mNumRows = 0;
        other.mNumCols = 0;
    }
    return *this;
}

Matrix::~Matrix() {
    if (debug) std::cout << "\n>> Destructor: Matrix " << mName << " has been deleted\n";
    if (mData) {
        for (int i = 0; i < mNumRows; ++i) delete[] mData[i];
        delete[] mData;
    }
}

// Access element (1-based) - mutable
double& Matrix::operator()(const int& row, const int& col) {
    if (row < 1 || row > mNumRows || col < 1 || col > mNumCols)
        throw out_of_range("\nError: The matrix index (" + to_string(row) + ", " + to_string(col) + ") is out of range.");
    return mData[row - 1][col - 1];
}

// Access element (1-based) - immutable
double Matrix::operator()(const int& row, const int& col) const {
    if (row < 1 || row > mNumRows || col < 1 || col > mNumCols)
        throw out_of_range("\nError: The matrix index (" + to_string(row) + ", " + to_string(col) + ") is out of range.");
    return mData[row - 1][col - 1];
}

int Matrix::rows() const { return mNumRows; }
int Matrix::cols() const { return mNumCols; }
std::pair<int, int> Matrix::shape() const { return {mNumRows, mNumCols}; }

// WARNING: Since C++17 has RVO and NRVO function, don't need to add debug scope for +, -, *
// Addition
Matrix Matrix::operator+(const Matrix& other) const {
    if (shape() != other.shape()) throw std::runtime_error("\nError: Cannot add matrices that have mismatch sizes.");
    Matrix result(mNumRows, mNumCols);
    for (int i = 0; i < mNumRows; ++i)
        for (int j = 0; j < mNumCols; ++j)
            result.mData[i][j] = mData[i][j] + other.mData[i][j];
    return result;
}

// Subtraction
Matrix Matrix::operator-(const Matrix& other) const {
    if (shape() != other.shape()) throw std::runtime_error("\nError: Cannot subtract matrices that have mismatch sizes.");
    Matrix result(mNumRows, mNumCols);
    for (int i = 0; i < mNumRows; ++i)
        for (int j = 0; j < mNumCols; ++j)
            result.mData[i][j] = mData[i][j] - other.mData[i][j];
    return result;
}

// Scalar multiplication <Matrix*scalar>
Matrix Matrix::operator*(const double& scalar) const {
    Matrix result(mNumRows, mNumCols);
    for (int i = 0; i < mNumRows; ++i)
        for (int j = 0; j < mNumCols; ++j)
            result.mData[i][j] = mData[i][j] * scalar;
    return result;
}

// Matrix multiplication
Matrix Matrix::operator*(const Matrix& other) {
    if (mNumCols != other.mNumRows) throw std::runtime_error("\nError: Cannot multiply matrices that have ncompatible sizes.");
    Matrix result(mNumRows, other.mNumCols);
    for (int i = 0; i < mNumRows; ++i)
        for (int j = 0; j < other.mNumCols; ++j)
            for (int k = 0; k < mNumCols; ++k)
                result.mData[i][j] += mData[i][k] * other.mData[k][j];
    return result;
}

Vector Matrix::operator*(const Vector& other) {
    if (mNumCols != other.size()) throw std::runtime_error("Incompatible sizes.");
    Vector result(mNumRows, 0.0); // Create zero-vector
    for (int i = 0; i < mNumRows; ++i)
        for (int j = 0; j < mNumCols; ++j)
            result(i + 1) += mData[i][j] * other(j + 1);
    return result;
}

// Determinant
double Matrix::det() const {
    DebugScope scope((mName.empty() ? "Matrix::det" : mName + "::det"));
    Matrix temp = *this;
    if (mNumRows != mNumCols) throw std::runtime_error("\nError: Cannot calculate the determinant of a non-square matrix.");
    int n = mNumCols;
    for (int j = 0; j < n; ++j) {
        if (std::abs(temp(j + 1, j + 1)) < 1e-10) throw std::runtime_error("Zero pivot.");
        for (int i = j + 1; i < n; ++i) {
            double x = temp(i + 1, j + 1) / temp(j + 1, j + 1);
            for (int k = 0; k < n; ++k)
                temp(i + 1, k + 1) -= x * temp(j + 1, k + 1);
        }
    }
    double determinant = 1.0;
    for (int i = 0; i < n; ++i)
        determinant *= temp(i + 1, i + 1);
    return determinant;
}

// Inverse
Matrix Matrix::inverse(const string& name) const {
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
            throw runtime_error("\nError: Matrix " + name + " is singular or near-singular.");

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

    return result;
}

// Tranpose
Matrix Matrix::transpose() const {
    DebugScope scope((mName.empty() ? "Matrix::transpose" : mName + "::transpose"));
    if ((*this).shape() == pair<int, int>{1,1}) {
        Matrix result = (*this);
        return result;
    }
    Matrix result(mNumCols, mNumRows, mName + "^T");
    for (int i = 0; i < mNumCols; ++i)
        for (int j = 0; j < mNumRows; ++j)
            result(i + 1, j + 1) = (*this)(j + 1, i + 1);
    return result;
}

// Pseudoinverse
Matrix Matrix::pseudoinverse(double tolerance) const {
    DebugScope scope(mName + "::pseudoinverse");
    Eigen::MatrixXd A = toEigen();
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(A, Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::VectorXd S_inv = svd.singularValues().unaryExpr([tolerance](double s) {
        return (s > tolerance) ? 1.0 / s : 0.0;
    });
    Eigen::MatrixXd S_plus = S_inv.asDiagonal();
    Eigen::MatrixXd A_pinv = svd.matrixV() * S_plus * svd.matrixU().transpose();
    Matrix result;
    result.fromEigen(A_pinv);
    return result;
}

// Helper to convert to Eigen matrix
Eigen::MatrixXd Matrix::toEigen() const {
    Eigen::MatrixXd mat(mNumRows, mNumCols);
    for (int i = 0; i < mNumRows; ++i)
        for (int j = 0; j < mNumCols; ++j)
            mat(i, j) = mData[i][j];
    return mat;
}

// Helper to load from Eigen matrix
void Matrix::fromEigen(const Eigen::MatrixXd& mat) {
    for (int i = 0; i < mNumRows; ++i) delete[] mData[i];
    delete[] mData;
    mNumRows = mat.rows();
    mNumCols = mat.cols();
    mData = new double*[mNumRows];
    for (int i = 0; i < mNumRows; ++i) {
        mData[i] = new double[mNumCols];
        for (int j = 0; j < mNumCols; ++j)
            mData[i][j] = mat(i, j);
    }
}

std::ostream& operator<<(std::ostream& os, const Matrix& mat) {
    for (int i = 0; i < mat.mNumRows; ++i) {
        for (int j = 0; j < mat.mNumCols; ++j) {
            double val = mat.mData[i][j];
            if (std::abs(val) < 1e-12) val = 0.0;
            os << val << (j == mat.mNumCols - 1 ? "\n" : " ");
        }
    }
    return os;
}

Matrix operator*(const double& scalar, const Matrix& mat) {
    return mat * scalar;
}

Vector operator*(const Vector& vec, const Matrix& mat) {
    if (vec.size() != mat.rows()) {
        throw std::runtime_error("Incompatible sizes for vector * matrix multiplication");
    }
    Vector result(mat.cols(), 0.0);  // Result is a row vector with size = mat.cols()
    for (int j = 0; j < mat.cols(); ++j) {
        for (int i = 0; i < mat.rows(); ++i) {
            result(j + 1) += vec(i + 1) * mat(i + 1, j + 1);
        }
    }
    return result;
}
