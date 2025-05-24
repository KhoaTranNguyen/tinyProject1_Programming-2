#include <iostream>
#include <cstdlib>        // For exit()
#include <initializer_list>
#include "Vector.hpp"
using namespace std;

// Default constructor
Vector::Vector() : mData(nullptr), mSize(0) {}

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

// Destructor
Vector::~Vector() {
    delete[] mData;
}

// Addition
Vector Vector::operator+(const Vector& other) const {
    if (mSize != other.mSize) {
        throw runtime_error("Error: Cannot add vectors with different sizes.");
        exit(EXIT_FAILURE);
    }
    double* newData = new double[mSize];
    for (int i = 0; i < mSize; ++i) newData[i] = mData[i] + other.mData[i];
    return Vector(mSize, newData);
}

// Subtration
Vector Vector::operator-(const Vector& other) const {
    if (mSize != other.mSize) {
        throw runtime_error("Error: Cannot subtract vectors with different sizes.");
        exit(EXIT_FAILURE);
    }
    double* newData = new double[mSize];
    for (int i = 0; i < mSize; ++i) newData[i] = mData[i] - other.mData[i];
    return Vector(mSize, newData);
}

// Scalar multiplication
Vector Vector::operator*(double scalar) const {
    double* newData = new double[mSize];
    for (int i = 0; i < mSize; ++i) newData[i] = mData[i] * scalar;
    return Vector(mSize, newData);
}

// Valid index checking (1 based)
const bool Vector::operator[](int index) const {
    if (index < 1 || index > mSize) {
        throw out_of_range("Error: Index " + to_string(index) + " out of bounds (1-based).");
        return false;
    }
    return true;
}

// Valid index accessing (1 based)
const double& Vector::operator()(int index) const {
    if ((*this)[index]) {
        return mData[index - 1];
    }
    exit(EXIT_FAILURE);
}

// Vector content cout
ostream& operator<<(ostream& os, const Vector& v) {
    cout << "(";
    for (int i = 0; i < v.mSize - 1; ++i)
        cout << v.mData[i] << ", ";
    cout << v.mData[v.mSize - 1] << ")" << endl;
    return os;
}