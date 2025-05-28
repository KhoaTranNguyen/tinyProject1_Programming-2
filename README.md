# 🔢 Linear System Solver Library in C++

## 📘 Overview

This lightweight C++ linear algebra library provides a clean, modular framework for solving systems of linear equations of the form **Ax = b**, leveraging numerical techniques including:

* **Gaussian elimination with pivoting**
* **Conjugate Gradient** for symmetric positive definite systems
* **Least Squares** for over/under-determined systems via:

  * Moore-Penrose pseudo-inverse
  * Tikhonov regularization

Built with extensibility and C++ idioms in mind, this project is designed for educational and prototyping use.

---

## 📁 Folder Structure

```
LinearSystem/
├── Test/
│   ├── Makefile
│   ├── test1.cpp           # Tests for Vector and Matrix classes
│   └── test2.cpp           # Tests for solving linear systems
├── Vector.hpp            
├── Matrix.hpp            
├── LinearSystem.hpp        # Base class using Gaussian elimination
├── PosSymLinSystem.hpp     # Conjugate Gradient for symmetric systems
└── LeastSquaresSystem.hpp  # Pseudoinverse and Tikhonov for LS systems
```

---

## 🧹 Class Design

### `Vector` Class

Dynamically allocated 1D vector with:

* Custom constructors and destructors. You can use this Python-like format to initialize a vector:

  ```cpp
  Vector v1 = {1, 2, 3};
  ```

* Operator overloading for `+`, `-`, `*` with scalars and vectors:

  * Supports scaling with both `vector * scalar` and `scalar * vector`
  * Dot product with `vector * vector`

    **Input:**

    ```cpp
    Vector v2 = {4, 5, 6};
    std::cout << v1 * v2;
    ```

    **Output:**

    ```
    32
    ```

    **Input:**

    ```cpp
    std::cout << v1 * 2.0;
    ```

    **Output:**

    ```
    (2, 4, 6)
    ```

* Bounds-checked access via `[]` and `()` (1-based indexing):

  **Input:**

  ```cpp
  v1[0];
  ```

  **Output:**

  ```
  false
  ```

  **Input:**

  ```cpp
  std::cout << v2(1);
  std::cout << v2(5); // Invalid accessing
  ```

  **Output:**

  ```
  4
  Error: Index 5 is out of bounds (1-based).
  ```

* Stream output to `std::cout`:

  **Input:**

  ```cpp
  std::cout << v1;
  ```

  **Output:**

  ```
  (1, 2, 3)
  ```

* Internals:

  * `int mSize`
  * `double* mData`

---

### `Matrix` Class

Dynamic 2D matrix class with:

* Custom copy constructor, destructor, and deep copy semantics.

* Initialization via manual or macro-based input:
  **Input:**
  ```cpp
  NAMED_MATRIX(T, 3, 3);
  T(1,1) = 2; T(1,2) = 1; T(1,3) = 0;
  T(2,1) = 1; T(2,2) = 2; T(2,3) = 1;
  T(3,1) = 0; T(3,2) = 1; T(3,3) = 2;
  ```

  ```cpp
  DECLARE_MATRIX(A,
      {1, 2, 0},
      {0, 2, 3},
      {3, 2, 4}
  );
  
  std::cout << "\nMatrix A: \n"<< A;
  ```
  **Output:**
  ```cpp
  Matrix A:
  1 2 0
  0 2 3
  3 2 4
  ```
* Overloaded operators for scalar and matrix arithmetic, and matrix-vector multiplication:
  **Input:**
  ```cpp
  DECLARE_MATRIX(B,
      {0.1, -0.4, 0.3},
      {0.45, 0.2, -0.15},
      {-0.3, 0.2, 0.1},
  );
  
  std::cout << "\nA * B: \n" << ; // Should be identity
  ```
  **Output:**
  ```cpp
  A * B:
  1 0 0
  0 1 0
  0 0 1
  ```
* Access via overloaded `()` with 1-based indexing
  **Input:**
  ```cpp
  std:: cout << B(1, 1);
  ```
  **Output:**
  ```cpp
  0.1
  ```
* Core methods:

  * `determinant()`
  * `inverse()`
  * `pseudoinverse()` (Moore-Penrose)
  * `solve(Vector b)`
  * `conjugateGradient(Vector b)`

* Other methods:

  * `shape()` (`std::pair<int, int>`)
  * `rows()`
  * `cols()`
  * `isSymmetric()` (`bool`)

* Internals:

  * `int mNumRows, mNumCols`
  * `double** mData`

---

## 🏗️ LinearSystem Class Hierarchy

### 🔹 `LinearSystem`

* Abstract base class for solving square systems using **Gaussian elimination with partial pivoting**:

  ```cpp
  Vector solve();
  ```

* Used for general systems where `A` is square and invertible.

* Calls `Matrix::solve(Vector)` internally using `Eigen::::colPivHouseholderQr()`, which is a robust method for solving general square (or even rectangular) systems and equivalent to LU with column pivoting. This is a safe choice for numerical stability.

* Stores:

  * `Matrix* mpA`
  * `Vector* mpb`

---

### 🔹 `PosSymLinSystem`

* Derived from `LinearSystem`, optimized for **symmetric positive definite** matrices.

* Uses **Conjugate Gradient Method**, efficient for large sparse systems:

  ```cpp
  Vector solve() override;
  ```

* Internally calls `Matrix::conjugateGradient(Vector)` which uses `Eigen::ConjugateGradient<Eigen::MatrixXd, 3>`, a class that solves linear problems of the form A·x = b using an ***iterative conjugate gradient algorithm***.

* The matrix A must be selfadjoint. The matrix A and the vectors x and b can be either dense or sparse.

* If the matrix A is not symmetric, the algorithm **may not converge or may produce incorrect results**, since the Conjugate Gradient method is mathematically defined only for symmetric positive definite matrices.

* Inherits storage and interface from `LinearSystem`.

---

### 🔹 `LeastSquaresSystem`

Derived from `LinearSystem`, this class is designed to solve **overdetermined** and **underdetermined** systems—where standard Gaussian elimination may not apply or leads to unstable results.

It supports **Tikhonov-regularized least squares**, a method used to handle noisy or ill-posed systems by introducing a regularization term:

```cpp
LeastSquaresSystem(Matrix* A, Vector* b, double lambda = 0.0);
Vector solve() override;
```

Instead of directly solving $\min_x \|Ax - b\|^2$, which can lead to unstable or large-magnitude solutions, it minimize the regularized objective:

$$
\min_x \left( \|Ax - b\|^2 + \lambda \|x\|^2 \right)
$$

* If $\lambda = 0$, it falls back to the Moore–Penrose **pseudoinverse**.
* If $\lambda > 0$, it uses **Tikhonov regularization** with a user-provided penalty weight.

In this implementation, $\lambda$ is passed manually. While there's no automatic rule to find the best $\lambda$, it's a great opportunity in prototyping to **experiment** and see its effect on numerical stability and generalization.


The class adds:

```cpp
double lambda; // Regularization factor
```

and internally performs:

* $A^T A + \lambda I$ if `lambda` is regularized,
* or simply calls `Matrix::pseudoinverse()` otherwise.

---

## 🧪 Test Cases & Output

### 🧷 `test1.cpp` – Matrix & Vector Test

```sh
make test1 && ./test1
```

**Output:**

```go
=== Matrix and Vector Class Tests ===

Matrix T:
2 1 0
1 2 1
0 1 2

Matrix A:
1 2 0
0 2 3
3 2 4

Pseudoinverse of A:
0.1 -0.4 0.3
0.45 0.2 -0.15
-0.3 0.2 0.1

Transpose of A:
1 0 3
2 2 2
0 3 4

Inverse of A (Matrix B):
0.1 -0.4 0.3
0.45 0.2 -0.15
-0.3 0.2 0.1

Product A * B (should be identity):
1 0 0
0 1 0
0 0 1

Vector v:
(1, 2, 3)

v(0) = 1, v(1) = 2, v(2) = 3

Size of v: 3

Vector v_scaled = 2.0 * v:
(2, 4, 6)

Vector v2:
(4, 5, 6)

Vector sum of v + v2:
(5, 7, 9)

Dot product v . v2 = 32

Matrix A * Vector v:
(5, 13, 19)

Vector v * Matrix A:
(10, 12, 18)

=== Program ended ===
```

---

### 🧷 `test2.cpp` – System Solving Test

```sh
make test2 && ./test2
```

**Output:**

```go
=== LinearSystem Test ===
Solution x:
(1, 1, 1)

=== PosSymLinSystem Test ===
Solution x (Conjugate Gradient):
(-0.368421, 0.789474, 1.68421)

=== LeastSquaresSystem Test (Overdetermined) ===
Least Squares solution x:
(3.5, 1.4)

=== Test Completed ===
```

---

## 📜 License

\[Your license here]

---

## 👨‍💼 Author

\[Khoa Tran Nguyen]
\[GitHub/email/contact]