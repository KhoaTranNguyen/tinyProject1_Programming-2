#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <iostream>
using namespace std;

class Vector {
private:
    double* mData;
    int mSize;

public:
    // Default constructor
    Vector();

    // Constructor with user input
    Vector(initializer_list<double> list);

    // Constructor with deep copy
    Vector(int s, const double* a);

    // Copy constructor
    Vector(const Vector& other);

    // Destructor
    ~Vector();

    // Export mSize
    int size() const;

    // Addition
    Vector operator+(const Vector& other) const;

    // Subtraction
    Vector operator-(const Vector& other) const;

    // Unary
    Vector operator-() const;

    // Assignment
    Vector& operator=(const Vector& other);

    // Vector multiplication
    double operator*(const Vector& other) const;

    // Scalar multiplication
    Vector operator*(double scalar) const;

    // Bounds-check operator (1-based index)
    const bool operator[](int index) const;

    // Access operator (1-based index)
    const double& operator()(int index) const;

    // Cout vector
    friend ostream& operator<<(ostream& os, const Vector& v);
};

// Scalar multiplication (scalar * Vector) - non-member function
Vector operator*(double scalar, const Vector& vec);

#endif // VECTOR_HPP