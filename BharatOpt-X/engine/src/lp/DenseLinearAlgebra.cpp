#include "bharatopt/lp/DenseLinearAlgebra.hpp"
#include <cmath>
#include <algorithm>

namespace bharatopt {

std::vector<double> DenseLinearAlgebra::solve_lu(std::vector<double> A, const std::vector<double>& b) {
    int n = b.size();
    if (A.size() != static_cast<size_t>(n * n)) throw std::invalid_argument("A must be n x n");
    
    std::vector<double> x = b;
    std::vector<int> p(n);
    for (int i = 0; i < n; ++i) p[i] = i;

    // LU decomposition with partial pivoting
    for (int i = 0; i < n; ++i) {
        double max_val = 0.0;
        int max_idx = i;
        for (int k = i; k < n; ++k) {
            double val = std::abs(A[k * n + i]);
            if (val > max_val) {
                max_val = val;
                max_idx = k;
            }
        }
        if (max_val < 1e-12) {
            throw std::runtime_error("Matrix is singular");
        }
        if (max_idx != i) {
            std::swap(p[i], p[max_idx]);
            for (int j = 0; j < n; ++j) {
                std::swap(A[i * n + j], A[max_idx * n + j]);
            }
            std::swap(x[i], x[max_idx]);
        }
        for (int j = i + 1; j < n; ++j) {
            A[j * n + i] /= A[i * n + i];
            for (int k = i + 1; k < n; ++k) {
                A[j * n + k] -= A[j * n + i] * A[i * n + k];
            }
        }
    }
    
    // Forward substitution (L y = Pb) - x already has Pb, we do it in place
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            x[i] -= A[i * n + j] * x[j];
        }
    }
    
    // Backward substitution (U x = y)
    for (int i = n - 1; i >= 0; --i) {
        for (int j = i + 1; j < n; ++j) {
            x[i] -= A[i * n + j] * x[j];
        }
        x[i] /= A[i * n + i];
    }
    
    return x;
}

std::vector<double> DenseLinearAlgebra::solve_cholesky(std::vector<double> A, const std::vector<double>& b) {
    int n = b.size();
    if (A.size() != static_cast<size_t>(n * n)) throw std::invalid_argument("A must be n x n");
    
    // Cholesky decomposition L L^T
    std::vector<double> L(n * n, 0.0);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            double sum = 0;
            for (int k = 0; k < j; k++)
                sum += L[i * n + k] * L[j * n + k];

            if (i == j) {
                double val = A[i * n + i] - sum;
                // Add a small regularization if not perfectly PSD due to numerical issues
                if (val <= 0) val = 1e-12; 
                L[i * n + j] = std::sqrt(val);
            } else {
                L[i * n + j] = (1.0 / L[j * n + j]) * (A[i * n + j] - sum);
            }
        }
    }
    
    // Forward substitution Ly = b
    std::vector<double> y(n, 0.0);
    for (int i = 0; i < n; i++) {
        double sum = b[i];
        for (int j = 0; j < i; j++) {
            sum -= L[i * n + j] * y[j];
        }
        y[i] = sum / L[i * n + i];
    }
    
    // Backward substitution L^T x = y
    std::vector<double> x(n, 0.0);
    for (int i = n - 1; i >= 0; i--) {
        double sum = y[i];
        for (int j = i + 1; j < n; j++) {
            sum -= L[j * n + i] * x[j];
        }
        x[i] = sum / L[i * n + i];
    }
    
    return x;
}

} // namespace bharatopt
