#pragma once

#include <iostream>
#include <cstdlib>        // For exit()
#include <initializer_list>
#include <Eigen/Dense>
using namespace std;

static bool debug = false;

class Vector {
private:
    int mSize;
    double* mData;  

public:
    // Default constructor
    Vector() : mSize(0), mData(nullptr) {}

    // Constructor with user input
    Vector(initializer_list<double> list) : mSize(list.size()) {
        mData = new double[mSize];
        int i = 0;
        for (auto val : list) {
            mData[i++] = val;
        }
    }

    // Constructor with deep copy
    Vector(int s, const double* a) : mSize(s) {
        mData = new double[mSize];
        for (int i = 0; i < mSize; ++i) mData[i] = a[i];
    }

    // Number-constructor
    Vector(int s, const double a) : mSize(s) {
        mData = new double[mSize];
        for (int i = 0; i < mSize; ++i) mData[i] = a;
    }

    // Copy constructor
    Vector(const Vector& other) {
        mSize = other.mSize;
        mData = new double[other.mSize];
        for (int i = 0; i < mSize; ++i) mData[i] = other.mData[i];
    }

    // Move constructor
    Vector(Vector&& other) noexcept
    : mSize(other.mSize), mData(other.mData) {
        other.mData = nullptr;
        other.mSize = 0;
        //if (debug) std::cout << ">> Move Constructor: Vector has been moved\n";
    } 

    // Destructor
    ~Vector() {
        if (debug) cout << "\n>> Destructor: The called vector of size " << this->mSize << " has been deleted" << endl;
        delete[] mData;
    }

    // Export mSize
    int size() const { return mSize; }

    // toEigen
    Vector(const Eigen::VectorXd& eigenVec)
    : mSize(eigenVec.size()) {
        mData = new double[mSize];
        for (int i = 0; i < mSize; ++i) {
            mData[i] = eigenVec(i);
        }
        if (debug) std::cout << "Eigen::VectorXd to Vector constructor called\n";
    }

    Eigen::VectorXd toEigen() const {
        Eigen::VectorXd v(mSize);
        for (int i = 0; i < mSize; ++i) {
            v(i) = mData[i];
        }
        return v;
    }

    // Addition
    Vector operator+(const Vector& other) const {
        if (mSize != other.mSize) {
            throw runtime_error("\n>> Error: Cannot add vectors with different sizes.");
        }
        double* newData = new double[mSize];
        for (int i = 0; i < mSize; ++i) newData[i] = mData[i] + other.mData[i];
        return Vector(mSize, newData);
    }

    // Subtraction
    Vector operator-(const Vector& other) const {
        if (mSize != other.mSize) {
            throw runtime_error("\n>> Error: Cannot subtract vectors with different sizes.");
        }
        double* newData = new double[mSize];
        for (int i = 0; i < mSize; ++i) newData[i] = mData[i] - other.mData[i];
        return Vector(mSize, newData);
    }

    // Unary
    Vector operator-() const {
        double* newData = new double[mSize];
        for (int i = 0; i < mSize; ++i) newData[i] = -mData[i];
        return Vector(mSize, newData);
    }

    // Assignment
    Vector& operator=(const Vector& other) {
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
    double operator*(const Vector& other) const {
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
    Vector operator*(double scalar) const {
        double* newData = new double[mSize];
        for (int i = 0; i < mSize; ++i) newData[i] = mData[i] * scalar;
        return Vector(mSize, newData);
    }

    // Bounds-check operator (1-based index)
    bool operator[](int index) const {
        if (index < 1 || index > mSize) return false;
        return true;
    }

    // Access operator (1-based index)
    double& operator()(int index) {
        if ((*this)[index]) return mData[index - 1];
        throw out_of_range("\n>> Error: Index " + to_string(index) + " is out of bounds (1-based).");
    }

    // Access operator (1-based index) - immutable
    const double& operator()(int index) const {
        if ((*this)[index]) return mData[index - 1];
        throw out_of_range("\n>> Error: Index " + to_string(index) + " is out of bounds (1-based).");
    }

    // Cout vector
    friend ostream& operator<<(ostream& os, const Vector& vec) {
        cout << "(";
        for (int i = 0; i < vec.mSize - 1; ++i)
            cout << vec.mData[i] << ", ";
        cout << vec.mData[vec.mSize - 1] << ")\n";
        return os;
    }
};

// Scalar multiplication (scalar * Vector) - non-member function
Vector operator*(double scalar, const Vector& vec) {
    return vec * scalar;
}