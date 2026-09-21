#pragma once

#include "MILPNode.hpp"
#include "MILPStatistics.hpp"
#include "NodeSelector.hpp"
#include "BranchingRule.hpp"
#include "CutGenerator.hpp"
#include "bharatopt/model/Model.hpp"
#include "bharatopt/lp/SimplexSolver.hpp"

#include <memory>
#include <vector>

namespace bharatopt {

class MILPSolver {
public:
    MILPSolver();
    
    // Setters for strategies
    void set_node_selector(std::unique_ptr<NodeSelector> selector);
    void set_branching_rule(std::unique_ptr<BranchingRule> rule);
    void add_cut_generator(std::unique_ptr<CutGenerator> generator);
    
    SolverResult solve(const Model& model);
    
    const MILPStatistics& get_statistics() const { return stats_; }

private:
    std::unique_ptr<NodeSelector> node_selector_;
    std::unique_ptr<BranchingRule> branching_rule_;
    std::vector<std::unique_ptr<CutGenerator>> cut_generators_;
    
    MILPStatistics stats_;
    
    // Helper to build a cloned model with node-specific bounds
    Model build_node_model(const Model& original_model, const MILPNode& node);
    
    // Tolerance for integer checking
    const double INT_TOL = 1e-6;
};

} // namespace bharatopt
