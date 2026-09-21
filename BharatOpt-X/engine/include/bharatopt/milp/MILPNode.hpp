#pragma once

#include "bharatopt/model/Types.hpp"
#include "bharatopt/model/Variable.hpp"
#include <vector>

namespace bharatopt {

struct MILPNode {
    int id;
    int parent_id;
    int depth;
    
    // Bounds active at this node. size() == num_variables
    std::vector<Bounds> variable_bounds;
    
    double lp_bound;
    std::vector<double> lp_solution;
    
    bool is_integer_feasible;
    bool is_infeasible;
    
    // For comparing nodes in priority queues
    bool operator<(const MILPNode& other) const {
        return lp_bound < other.lp_bound;
    }
};

} // namespace bharatopt
