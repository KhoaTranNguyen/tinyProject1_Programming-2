#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <iostream>
using namespace std;

class Vector {
private:
    int mSize;
    double* mData;  

public:
    // Default constructor
    Vector();

    // Constructor with user input
    Vector(initializer_list<double> list);

    // Constructor with deep copy
    Vector(int s, const double* a);

    // Copy constructor
    Vector(const Vector& other);

    // Move constructor
    Vector(Vector&& other) noexcept;

    // Number-constructor
    Vector(int s, const double a);

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
    bool operator[](int index) const;

    // Access operator (1-based index)
    double& operator()(int index) const;

    // Cout vector
    friend ostream& operator<<(ostream& os, const Vector& v);
};

// Scalar multiplication (scalar * Vector) - non-member function
Vector operator*(double scalar, const Vector& vec);

#endif // VECTOR_HPP