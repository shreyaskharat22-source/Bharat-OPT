#include "bharatopt/utils/Numerics.hpp"
#include <cmath>
#include <algorithm>
#include <stdexcept>

namespace bharatopt {

double Numerics::infinity_norm(const std::vector<double>& x) {
    double norm = 0.0;
    for (double val : x) {
        norm = std::max(norm, std::abs(val));
    }
    return norm;
}

std::vector<double> Numerics::compute_residual(
    const std::vector<double>& b, 
    const std::vector<double>& Ax) {
    if (b.size() != Ax.size()) {
        throw std::invalid_argument("Vector dimensions mismatch in residual computation");
    }
    std::vector<double> r(b.size());
    for (size_t i = 0; i < b.size(); ++i) {
        r[i] = b[i] - Ax[i];
    }
    return r;
}

bool Numerics::is_zero(double val, double tol) {
    return std::abs(val) <= tol;
}

bool Numerics::is_finite(const std::vector<double>& x) {
    for (double val : x) {
        if (!std::isfinite(val)) {
            return false;
        }
    }
    return true;
}

} // namespace bharatopt
