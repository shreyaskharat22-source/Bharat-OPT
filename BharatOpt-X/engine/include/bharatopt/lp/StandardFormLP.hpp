#pragma once

#include "bharatopt/sparse/COO.hpp"
#include <vector>

namespace bharatopt {

// Standard form: min c^T x, s.t. A x = b, x >= 0
struct StandardFormLP {
    int num_vars = 0;
    int num_constraints = 0;
    std::vector<double> c; // Objective coefficients
    std::vector<double> b; // Right hand side
    COO A = COO(0, 0);     // Constraint matrix in COO format (can be converted to CSR/CSC)
    double objective_offset = 0.0;
    
    // To map back to original model
    // primal_map[i] = {original_var_idx, multiplier} e.g. for free variable x = x+ - x-
    // We keep this simple for Phase 2: just keep track of how to map back.
    struct VarMapping {
        int original_var_index = -1;
        double multiplier = 1.0;
    };
    std::vector<VarMapping> var_mapping;
};

} // namespace bharatopt
