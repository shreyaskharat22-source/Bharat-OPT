#pragma once

#include <vector>

namespace bharatopt {

class Numerics {
public:
    static constexpr double EPSILON = 1e-9;

    // Infinity norm of a vector (max absolute value)
    static double infinity_norm(const std::vector<double>& x);

    // Compute residual r = b - Ax
    static std::vector<double> compute_residual(
        const std::vector<double>& b, 
        const std::vector<double>& Ax);

    // Check if a value is effectively zero
    static bool is_zero(double val, double tol = EPSILON);

    // Validate if an array contains any NaNs or Infs
    static bool is_finite(const std::vector<double>& x);
};

} // namespace bharatopt
