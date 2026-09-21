#pragma once

namespace bharatopt {

struct MILPStatistics {
    int nodes_explored = 0;
    int nodes_pruned_infeasibility = 0;
    int nodes_pruned_bound = 0;
    int nodes_pruned_integrality = 0;
    
    int cuts_generated = 0;
    int cuts_accepted = 0;
    
    int lp_relaxations_solved = 0;
    
    double initial_lp_bound = 0.0;
    double best_incumbent = 0.0;
    double mip_gap = 1e30;
};

} // namespace bharatopt
