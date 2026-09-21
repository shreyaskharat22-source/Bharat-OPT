#pragma once

#include <vector>
#include <stdexcept>

namespace bharatopt {

class DenseLinearAlgebra {
public:
    // Solve Ax = b using basic Gaussian Elimination with partial pivoting
    // A is given as a flat 1D vector representing row-major n x n matrix
    static std::vector<double> solve_lu(std::vector<double> A, const std::vector<double>& b);

    // Solve Ax = b using basic Cholesky factorization for symmetric positive definite A
    static std::vector<double> solve_cholesky(std::vector<double> A, const std::vector<double>& b);
};

} // namespace bharatopt
