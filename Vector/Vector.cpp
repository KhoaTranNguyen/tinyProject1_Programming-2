#include <iostream>
#include <cstdlib>        // For exit()
#include <initializer_list>
#include "Vector.hpp"
#include "../Matrix/Matrix.hpp"
using namespace std;

// Default constructor
Vector::Vector() : mSize(0), mData(nullptr) {}

// Constructor from initializer list (user-friendly)
Vector::Vector(initializer_list<double> list) : mSize(list.size()) {
    mData = new double[mSize];
    int i = 0;
    for (auto val : list) {
        mData[i++] = val;
    }
} 

// Constructor using size and array (code only)
Vector::Vector(int s, const double* a) : mSize(s) {
    mData = new double[mSize];
    for (int i = 0; i < mSize; ++i) mData[i] = a[i];
}

// Number-constructor using size (code only)
Vector::Vector(int s, const double a) : mSize(s) {
    mData = new double[mSize];
    for (int i = 0; i < mSize; ++i) mData[i] = a;
}

// Copy constructor
Vector::Vector(const Vector& other) {
    mSize = other.mSize;
    mData = new double[other.mSize];
    for (int i = 0; i < mSize; ++i) mData[i] = other.mData[i];
}

// Move constructor
Vector::Vector(Vector&& other) noexcept
    : mSize(other.mSize), mData(other.mData) {
    other.mData = nullptr;
    other.mSize = 0;
    //if (debug) std::cout << ">> Move Constructor: Vector has been moved\n";
}

// Destructor
Vector::~Vector() {
    if (debug) cout << "\n>> Destructor: The called vector of size " << this->mSize << " has been deleted" << endl;
    delete[] mData;
}

// Export mSize
int Vector::size() const {
    return mSize;
}

// toEigen
Vector::Vector(const Eigen::VectorXd& eigenVec)
    : mSize(eigenVec.size()) {
    mData = new double[mSize];
    for (int i = 0; i < mSize; ++i) {
        mData[i] = eigenVec(i);
    }
    if (debug) std::cout << "Eigen::VectorXd to Vector constructor called\n";
}

Eigen::VectorXd Vector::toEigen() const {
    Eigen::VectorXd v(mSize);
    for (int i = 0; i < mSize; ++i) {
        v(i) = mData[i];
    }
    return v;
}

// Addition / Binary
Vector Vector::operator+(const Vector& other) const {
    if (mSize != other.mSize) {
        throw runtime_error("\n>> Error: Cannot add vectors with different sizes.");
    }
    double* newData = new double[mSize];
    for (int i = 0; i < mSize; ++i) newData[i] = mData[i] + other.mData[i];
    return Vector(mSize, newData);
}

// Subtration
Vector Vector::operator-(const Vector& other) const {
    if (mSize != other.mSize) {
        throw runtime_error("\n>> Error: Cannot subtract vectors with different sizes.");
    }
    double* newData = new double[mSize];
    for (int i = 0; i < mSize; ++i) newData[i] = mData[i] - other.mData[i];
    return Vector(mSize, newData);
}

// Unary
Vector Vector::operator-() const {
    double* newData = new double[mSize];
    for (int i = 0; i < mSize; ++i) newData[i] = -mData[i];
    return Vector(mSize, newData);
}

// Assignment
Vector& Vector::operator=(const Vector& other) {
    if (this != &other) {
        delete[] mData; // Free old memory
        this->mSize = other.mSize;
        this->mData = new double[mSize];
        for (int i = 0; i < mSize; i++) {
            this->mData[i] = other.mData[i];
        }
    }
    return (*this);
} 

// Vector multiplication
double Vector::operator*(const Vector& other) const {
    if (this->mSize != other.mSize) {
        throw runtime_error("\n>> Error: Cannot compute dot product of vectors with different sizes.");
    }
    double result = 0.0;
    for (int i = 0; i < mSize; i++) {
        result += mData[i] * other.mData[i];
    }
    return result;
}

// Scalar multiplication
Vector Vector::operator*(double scalar) const {
    double* newData = new double[mSize];
    for (int i = 0; i < mSize; ++i) newData[i] = mData[i] * scalar;
    return Vector(mSize, newData);
}

// Scalar multiplication (scalar * Vector) - non-member function
Vector operator*(double scalar, const Vector& vec) {
    return vec * scalar;
}

// Valid index checking (1 based)
bool Vector::operator[](int index) const {
    if (index < 1 || index > mSize) {
        return false;
    }
    return true;
}

// Valid index accessing (1 based)
double& Vector::operator()(int index) const {
    if ((*this)[index]) {
        return mData[index - 1];
    }
    throw out_of_range("\n>> Error: Index " + to_string(index) + " out of bounds (1-based).");
}

// Vector printing
ostream& operator<<(ostream& os, const Vector& vec) {
    cout << "(";
    for (int i = 0; i < vec.mSize - 1; ++i)
        cout << vec.mData[i] << ", ";
    cout << vec.mData[vec.mSize - 1] << ")" << endl;
    return os;
}