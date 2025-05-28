#include <iostream>
#include "../Matrix.hpp"
#include "../Vector.hpp"

int main() {
    cout << "=== Matrix and Vector Class Tests ===" << endl;

    // --- MATRIX TESTS ---
    // debug = true;

    NAMED_MATRIX(T, 3, 3);
    T(1, 1) = 2; T(1, 2) = 1; T(1, 3) = 0;
    T(2, 1) = 1; T(2, 2) = 2; T(2, 3) = 1;
    T(3, 1) = 0; T(3, 2) = 1; T(3, 3) = 2;
    cout << "\nMatrix T:\n" << T;

    DECLARE_MATRIX(A,
        {1, 2, 0},
        {0, 2, 3},
        {3, 2, 4}
    );
    cout << "\nMatrix A:\n" << A;

    Matrix A_pinv = A.pseudoinverse();
    cout << "\nPseudoinverse of A:\n" << A_pinv;

    cout << "\nTranspose of A:\n" << A.transpose();

    Matrix B = A.inverse("B");
    cout << "\nInverse of A (Matrix B):\n" << B;

    cout << "\nProduct A * B (should be identity):\n" << A * B;

    // --- VECTOR TESTS ---

    // Create vector v via initializer list
    Vector v = {1.0, 2.0, 3.0};
    cout << "\nVector v:\n" << v;

    // Access vector elements using operator()
    cout << "\nv(0) = " << v(1) << ", v(1) = " << v(2) << ", v(2) = " << v(3) << endl;

    // Vector size
    cout << "\nSize of v: " << v.size() << endl;

    // Vector scalar multiplication
    Vector v_scaled = 2.0 * v;
    cout << "\nVector v_scaled = 2.0 * v:\n" << v_scaled;

    // Vector addition
    Vector v2 = {4.0, 5.0, 6.0};
    Vector v_sum = v + v2;
    cout << "\nVector v2:\n" << v2;
    cout << "\nVector sum of v + v2:\n" << v_sum;

    // Vector dot product
    double dot = v * v2;
    cout << "\nDot product v . v2 = " << dot << endl;

    // Matrix-vector multiplication
    Vector Av = A * v;
    cout << "\nMatrix A * Vector v:\n" << Av;

    // Vector-matrix multiplication (if supported)
    try {
        Vector vA = v * A;
        cout << "\nVector v * Matrix A:\n" << vA;
    } catch (const std::exception& e) {
        cout << "\nException on v * A: " << e.what() << endl;
    }

    cout << "\n=== Program ended ===\n";
    return 0;
}