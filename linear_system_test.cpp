#include <iostream>
#include "Matrix/Matrix.hpp"
#include "Vector/Vector.hpp"
#include "LinearSystem/LinearSystem.hpp"
#include "LinearSystem/PosSymLinSystem.hpp"
#include "LinearSystem/LeastSquaresSystem.hpp"

int main() {
    std::cout << "=== LinearSystem Test ===\n";
    {
        // A is 3x3 invertible
        DECLARE_MATRIX(A,
            {2, -1, 0},
            {-1, 2, -1},
            {0, -1, 2}
        );
        Vector b = {1, 0, 1};
        LinearSystem sys(&A, &b);
        Vector x = sys.solve();
        std::cout << "Solution x:\n" << x << "\n";
    }

    std::cout << "\n=== PosSymLinSystem Test ===\n";
    {
        // SPD matrix
        DECLARE_MATRIX(A,
            {4, 1, 1},
            {1, 3, 0},
            {1, 0, 2}
        );
        Vector b = {1, 2, 3};
        PosSymLinSystem spdSys(&A, &b);
        Vector x = spdSys.solve();
        std::cout << "Solution x (Conjugate Gradient):\n" << x << "\n";
    }

    std::cout << "\n=== LeastSquaresSystem Test (Overdetermined) ===\n";
    {
        // Overdetermined system: 4x2
        DECLARE_MATRIX(A,
            {1, 1},
            {1, 2},
            {1, 3},
            {1, 4}
        );
        Vector b = {6, 5, 7, 10};
        LeastSquaresSystem lsqSys(&A, &b);
        Vector x = lsqSys.solve();
        std::cout << "Least Squares solution x:\n" << x << "\n";
    }

    std::cout << "\n=== Test Completed ===\n";
    return 0;
}