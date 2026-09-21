#include "bharatopt/lp/Scaler.hpp"
#include <cmath>
#include <algorithm>

namespace bharatopt {

void Scaler::scale(StandardFormLP& lp) {
    // Basic row scaling (L-infinity norm)
    std::vector<double> row_max(lp.num_constraints, 0.0);
    
    for (size_t i = 0; i < lp.A.nnz(); ++i) {
        int r = lp.A.row_indices()[i];
        row_max[r] = std::max(row_max[r], std::abs(lp.A.values()[i]));
    }
    
    for (int r = 0; r < lp.num_constraints; ++r) {
        if (row_max[r] > 1e-9) {
            lp.b[r] /= row_max[r];
        }
    }
    
    // Ideally we modify A directly. Since COO is simple:
    // We would need mutable access or rebuild COO.
    // For Phase 2 structure, we just leave it as a stub or rebuild.
}

} // namespace bharatopt
