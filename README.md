# 🔢 Linear System Solver Library & CPU-Based Regression in C++

## 📘 Overview

This project provides a **modular C++ library** for solving linear systems and performing **linear regression** on real-world data, using only CPU-based computations. It consists of two key components:

1. **Linear System Solver Library**
   A clean and flexible implementation for solving linear systems of the form **Ax = b** using various numerical techniques:

   * Gaussian elimination with partial pivoting
   * Conjugate Gradient method (for symmetric positive-definite matrices)
   * Least Squares and Ridge Regression (via pseudo-inverse and Tikhonov regularization)

2. **CPU-Based Linear Regression for Hardware Performance Prediction**
   A complete regression pipeline that:

   * Loads and parses real-world CPU benchmark data (`machine.data`)
   * Applies Ridge Regression using the solver library
   * Evaluates predictions using RMSE

Built in **C++17** using **Eigen** for numerical computations, this project is ideal for students and engineers interested in numerical methods, regression, and software prototyping.

---

## 📁 Folder Structure

```
ProjectRoot/
├── LinearSystem/                      # 📦 Linear System Solver Library
│   ├── Vector.hpp                    # Custom 1D vector class
│   ├── Matrix.hpp                    # Custom 2D matrix class
│   ├── LinearSystem.hpp              # Base class (Gaussian elimination)
│   ├── PosSymLinSystem.hpp           # Conjugate Gradient solver
│   ├── LeastSquaresSystem.hpp        # Ridge & Least Squares regression
│   └── Test/
│       ├── Makefile
│       ├── test1.cpp                 # Matrix & vector operations
│       └── test2.cpp                 # Solving example linear systems
│
└── LinearRegressionCPU/              # 📊 Ridge Regression with Real Dataset
    ├── cpu_prediction.cpp            # Main program for training/prediction
    ├── Makefile
    └── dataset/
        └── computer+hardware/
            ├── Index
            ├── machine.data          # Raw CPU performance dataset
            └── machine.names         # Metadata for dataset
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

## 📈 CPU Hardware Performance Regression

The `LinearRegressionCPU` directory demonstrates how to apply Ridge Regression on a real dataset.

### Dataset: `machine.data`

The dataset `machine.data` is expected to have comma-separated values with the following columns:

```csv
Vendor, Model, Feature1, Feature2, Feature3, Feature4, Feature5, Feature6, TargetPRP
```

* Vendor and Model are strings (only used for skipping; not included in features).
* Features are numeric values (6 columns).
* TargetPRP is the numeric target variable for prediction.

Example line:


```csv
adviser,32/60,125,256,6000,256,16,128,198
```

### Pipeline in `cpu_prediction.cpp`

1. Loads and parses CSV data.
2. Converts data into matrices `A` (features) and `b` (target).
3. Constructs and solves a `LeastSquaresSystem`.
4. Predicts outputs and computes RMSE.

---

### 📊 Results & Interpretation

#### RMSE vs Regularization (λ)

| λ Value |  RMSE |
| ------: | ----: |
|    0.01 | 28.93 |
|     0.1 | 28.96 |
|     1.0 | 29.37 |
|    10.0 | 35.29 |
|   100.0 | 80.29 |

* Lower RMSE indicates better generalization.
* Very high λ over-regularizes the model, leading to poor fit.

#### Insights

* λ ≈ 0.01–0.1 gives the best performance.
* Moderate regularization helps prevent overfitting, especially with noisy real-world data.

---

## 🛠️ Build & Run with Makefile (Windows / MSYS2)

This project uses a `Makefile` for convenient compilation. It relies on the [Eigen](https://eigen.tuxfamily.org/) library for linear algebra operations.

### 📦 Prerequisites

You must have [MSYS2](https://www.msys2.org/) installed with the UCRT64 environment and the Eigen header-only library.

#### Step-by-Step Installation on Windows:

1. **Install MSYS2:**
   [https://www.msys2.org/](https://www.msys2.org/)

2. **Launch MSYS2 UCRT64 shell**

3. **Update all packages:**

   ```sh
   pacman -Syu
   ```

4. **Install Eigen3:**

   ```sh
   pacman -S mingw-w64-ucrt-x86_64-eigen3
   ```

   This will install Eigen headers into:
   `C:/msys64/ucrt64/include/eigen3`

---

### ⚙️ Using the Makefile

Each test directory contains a Makefile. To build and run tests:

#### Build 2 Linear System tests:

```sh
cd LinearSystem/Test
make all
```

#### Run individual tests:

```sh
./test1.exe
./test2.exe
```

#### Clean Build Artifacts

```sh
make clean
```

#### Do it the same with CPU Prediction:

```sh
cd LinearRegressionCPU/
make cpu_prediction
./cpu_prediction.exe
```

### 📁 Example Makefile

```makefile
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -IC:/msys64/ucrt64/include/eigen3

# You can add more test files here
TESTS = test1 test2

.PHONY: all clean $(TESTS)

all: $(TESTS)

$(TESTS):
	$(CXX) $(CXXFLAGS) $@.cpp -o $@

clean:
	@echo "Cleaning up..."
	-del /Q $(addsuffix .exe, $(TESTS)) 2>nul || rm -f $(TESTS)
```

Or for the CPU Prediction:

```makefile
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -IC:/msys64/ucrt64/include/   eigen3

SRC = cpu_prediction.cpp
TARGET = cpu_prediction

$(TARGET): $(SRC)
    $(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
    @echo "Cleaning up..."
    -del /Q $(TARGET).exe 2>nul || rm -f $(TARGET)
```

Expected output:

* All matrix, vector and linear system tests
* Learned model parameters vector
* Test RMSE error value

---

### Dependencies

* C++17 or newer
* Custom linear algebra classes: `Matrix`, `Vector`, and `LeastSquaresSystem`

---

### Notes

* 📝 Ensure the Eigen include path matches your local MSYS2 installation (typically `C:/msys64/ucrt64/include/eigen3` for UCRT64 shell).
* The program expects the dataset file at `dataset/computer+hardware/machine.data` relative to the executable.
* The linear regression uses 1-based indexing in the `Matrix` and `Vector` classes.
* Regularization parameter lambda is fixed at 0.1, but you can adjust it in `main()`.

---

## 📜 License

This project is licensed under the [MIT License](LICENSE).

---

## 👨‍💼 Author

**Khoa Tran Nguyen**
🐙 [GitHub – khoatrannguyen](https://github.com/khoatrannguyen)
📧 [hellokhoatrannguyen@gmail.com](mailto:hellokhoatrannguyen@gmail.com)