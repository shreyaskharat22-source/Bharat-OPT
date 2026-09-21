#pragma once

#include "MILPNode.hpp"
#include "bharatopt/model/Model.hpp"
#include <vector>
#include <cmath>

namespace bharatopt {

class BranchingRule {
public:
    virtual ~BranchingRule() = default;
    
    // Returns the index of the variable to branch on, or -1 if all are integer
    virtual int select_branching_variable(const MILPNode& node, const Model& original_model) = 0;
};

class MostFractionalBranching : public BranchingRule {
public:
    int select_branching_variable(const MILPNode& node, const Model& original_model) override {
        int best_var = -1;
        double max_fractionality = -1.0;
        
        for (size_t i = 0; i < original_model.num_variables(); ++i) {
            if (original_model.variable(i).type() != VariableType::Continuous) {
                double val = node.lp_solution[i];
                double frac = std::abs(val - std::round(val));
                
                if (frac > 1e-6 && frac > max_fractionality) {
                    max_fractionality = frac;
                    best_var = i;
                }
            }
        }
        return best_var;
    }
};

class StrongBranching : public BranchingRule {
public:
    int select_branching_variable(const MILPNode& node, const Model& original_model) override {
        // Full strong branching is very expensive without hot-starting.
        // We will default back to most fractional if not fully implemented in MILPSolver yet.
        MostFractionalBranching fallback;
        return fallback.select_branching_variable(node, original_model);
    }
};

} // namespace bharatopt
