#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include "../LinearSystem/Vector.hpp"
#include "../LinearSystem/Matrix.hpp"
#include "../LinearSystem/LeastSquaresSystem.hpp"

// Load CSV data with comma separation
void loadData(const std::string& filename,
              std::vector<std::vector<double>>& features,
              std::vector<double>& targets) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << "\n";
        exit(1);
    }
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string vendor, model, token;

        if (!std::getline(ss, vendor, ',') || !std::getline(ss, model, ',')) {
            std::cerr << "Malformed line (vendor/model missing)\n";
            exit(1);
        }

        std::vector<double> row(6);
        for (int i = 0; i < 6; ++i) {
            if (!std::getline(ss, token, ',')) {
                std::cerr << "Malformed line (features missing)\n";
                exit(1);
            }
            try {
                row[i] = std::stod(token);
            } catch (...) {
                std::cerr << "Invalid feature value: " << token << "\n";
                exit(1);
            }
        }
        if (!std::getline(ss, token, ',')) {
            std::cerr << "Malformed line (target missing)\n";
            exit(1);
        }
        double prp;
        try {
            prp = std::stod(token);
        } catch (...) {
            std::cerr << "Invalid target value: " << token << "\n";
            exit(1);
        }

        features.push_back(row);
        targets.push_back(prp);
    }
}

// Split data into train/test (80/20 split)
void splitData(const std::vector<std::vector<double>>& features,
               const std::vector<double>& targets,
               std::vector<std::vector<double>>& trainFeatures,
               std::vector<double>& trainTargets,
               std::vector<std::vector<double>>& testFeatures,
               std::vector<double>& testTargets) {
    size_t N = features.size();
    if (N == 0 || targets.size() != N) {
        std::cerr << "Empty dataset or size mismatch.\n";
        exit(1);
    }
    std::vector<size_t> indices(N);
    for (size_t i = 0; i < N; ++i) indices[i] = i;

    
    // Use fixed seed for reproducibility
    // int rd = 42;
    // std::mt19937 g(rd); // <-- fixed seed
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indices.begin(), indices.end(), g);

    size_t trainSize = static_cast<size_t>(0.8 * N);

    trainFeatures.reserve(trainSize);
    trainTargets.reserve(trainSize);
    testFeatures.reserve(N - trainSize);
    testTargets.reserve(N - trainSize);

    for (size_t i = 0; i < trainSize; ++i) {
        trainFeatures.push_back(features[indices[i]]);
        trainTargets.push_back(targets[indices[i]]);
    }
    for (size_t i = trainSize; i < N; ++i) {
        testFeatures.push_back(features[indices[i]]);
        testTargets.push_back(targets[indices[i]]);
    }
}

// Convert std::vector<std::vector<double>> to Matrix (1-based indexing)
Matrix toMatrix(const std::vector<std::vector<double>>& data) {
    if (data.empty()) return Matrix(0, 0);
    int rows = static_cast<int>(data.size());
    int cols = static_cast<int>(data[0].size());
    Matrix M(rows, cols, "M");
    for (int i = 0; i < rows; ++i) {
        if (data[i].size() != static_cast<size_t>(cols)) {
            std::cerr << "Inconsistent feature vector size in data.\n";
            exit(1);
        }
        for (int j = 0; j < cols; ++j) {
            M(i + 1, j + 1) = data[i][j];
        }
    }
    return M;
}

// Convert std::vector<double> to Vector (1-based indexing)
Vector toVector(const std::vector<double>& data) {
    Vector v(static_cast<int>(data.size()), 0.0);
    for (int i = 0; i < static_cast<int>(data.size()); ++i) {
        v(i + 1) = data[i];
    }
    return v;
}

// Compute RMSE between predictions and actual targets
double computeRMSE(const Vector& predictions, const Vector& targets) {
    int n = predictions.size();
    if (n != targets.size()) {
        std::cerr << "Prediction and target vector sizes do not match.\n";
        exit(1);
    }
    double sumSq = 0.0;
    for (int i = 1; i <= n; ++i) {
        double diff = predictions(i) - targets(i);
        sumSq += diff * diff;
    }
    return std::sqrt(sumSq / n);
}

int main() {
    std::vector<std::vector<double>> features;
    std::vector<double> targets;

    loadData("dataset/computer+hardware/machine.data", features, targets);

    std::vector<std::vector<double>> trainFeatures, testFeatures;
    std::vector<double> trainTargets, testTargets;
    splitData(features, targets, trainFeatures, trainTargets, testFeatures, testTargets);

    Matrix A = toMatrix(trainFeatures);
    Vector b = toVector(trainTargets);

    double lambda;
    cout << "Input lambda: "; cin >> lambda;
    LeastSquaresSystem lss(&A, &b, lambda); // lambda = 0.1 for ridge regularization
    Vector x = lss.solve();

    std::cout << "Learned parameters (x):\n";
    for (int i = 1; i <= x.size(); ++i) {
        std::cout << x(i) << " ";
    }
    std::cout << "\n";

    Matrix A_test = toMatrix(testFeatures);
    Vector b_test = toVector(testTargets);

    Vector predictions = A_test * x;

    double rmse = computeRMSE(predictions, b_test);

    std::cout << "Test RMSE: " << rmse << std::endl;

    return 0;
}