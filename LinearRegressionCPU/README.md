# CPU-Based Linear Regression for Computer Hardware Performance Prediction

This project implements a simple ridge-regularized linear regression model to predict performance metrics (e.g., PRP) of computer hardware based on multiple features. The implementation is CPU-only, using custom `Matrix`, `Vector`, and `LeastSquaresSystem` classes for linear algebra and least squares solving.

---

## Project Structure

* **cpu\_prediction.cpp** — Main program to load dataset, train the ridge regression model, and evaluate performance.
* **LinearSystem/** — Directory containing custom matrix/vector classes and least squares solver.
* **dataset/computer+hardware/machine.data** — Dataset file with hardware features and target values.

---

## Dataset Format

The dataset `machine.data` is expected to have comma-separated values with the following columns:

```
Vendor, Model, Feature1, Feature2, Feature3, Feature4, Feature5, Feature6, TargetPRP
```

* Vendor and Model are strings (only used for skipping; not included in features).
* Features are numeric values (6 columns).
* TargetPRP is the numeric target variable for prediction.

Example line:

```
adviser,32/60,125,256,6000,256,16,128,198
```

---

## How It Works

1. **Load Data** — Reads the dataset, skips the first two string columns (Vendor and Model), extracts 6 features and 1 target per sample.
2. **Split Data** — Randomly splits data into 80% training and 20% testing sets.
3. **Train Model** — Fits a ridge regression model (least squares with L2 regularization lambda=0.1) on the training data.
4. **Predict & Evaluate** — Predicts targets on the test set and computes RMSE (Root Mean Square Error) as performance metric.

---

## Building and Running with Makefile

This project uses the [Eigen](https://eigen.tuxfamily.org/) library for linear algebra operations. To build and run the project, you need to have Eigen installed on your system.

#### Installing Eigen on Windows (MSYS2)

1. **Install MSYS2** if you haven't already:
   [https://www.msys2.org/](https://www.msys2.org/)

2. **Open MSYS2 UCRT64 shell** (or MinGW64 depending on your setup).

3. **Update package database and core system packages:**

   ```bash
   pacman -Syu
   ```

4. **Install Eigen3 package:**

   ```bash
   pacman -S mingw-w64-ucrt-x86_64-eigen3
   ```

   This will install Eigen headers to a location like:
   `C:/msys64/ucrt64/include/eigen3`

#### Build and Run

1. This is the built-in `Makefile` includes the correct include path for Eigen:

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

2. Build the executable:

   ```bash
   make cpu_prediction
   ```

3. Run the program:

   ```bash
   ./cpu_prediction
   ```

Expected output:

* Learned model parameters vector
* Test RMSE error value

---

## Dependencies

* C++17 or newer
* Custom linear algebra classes: `Matrix`, `Vector`, and `LeastSquaresSystem`

---

## Notes

* The program expects the dataset file at `dataset/computer+hardware/machine.data` relative to the executable.
* The linear regression uses 1-based indexing in the `Matrix` and `Vector` classes.
* Regularization parameter lambda is fixed at 0.1, but you can adjust it in `main()`.
