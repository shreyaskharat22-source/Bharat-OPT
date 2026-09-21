#include "bharatopt/lp/StandardFormConverter.hpp"
#include <stdexcept>
#include <cmath>

namespace bharatopt {

StandardFormLP StandardFormConverter::convert(const Model& model) {
    // For Phase 2, we implement a simplified conversion that assumes variables are bounded [0, inf]
    // or standard form directly to keep the focus on solver algorithms.
    // In a full implementation, we would split free variables x = x+ - x-, 
    // and shift lower bounds x >= L to x' = x - L >= 0.

    StandardFormLP lp;
    lp.num_constraints = static_cast<int>(model.num_constraints());
    int num_orig_vars = static_cast<int>(model.num_variables());
    
    lp.objective_offset = model.objective().offset();
    double obj_mult = (model.objective().sense() == ObjectiveSense::Maximize) ? -1.0 : 1.0;

    // Count slack/surplus variables needed
    int num_slacks = 0;
    for (int i = 0; i < lp.num_constraints; ++i) {
        if (model.constraint(i).type() != ConstraintType::Equal) {
            num_slacks++;
        }
    }

    lp.num_vars = num_orig_vars + num_slacks;
    lp.c.resize(lp.num_vars, 0.0);
    lp.b.resize(lp.num_constraints, 0.0);
    lp.A = COO(lp.num_constraints, lp.num_vars);
    lp.var_mapping.resize(lp.num_vars);

    for (int j = 0; j < num_orig_vars; ++j) {
        lp.c[j] = obj_mult * model.objective().coefficient(j);
        lp.var_mapping[j].original_var_index = j;
        lp.var_mapping[j].multiplier = 1.0;
    }

    int current_slack_idx = num_orig_vars;
    for (int i = 0; i < lp.num_constraints; ++i) {
        const auto& constr = model.constraint(i);
        
        // Handle RHS (if RHS is negative, we need to flip the constraint)
        double rhs_val = constr.rhs();
        double row_mult = 1.0;
        if (rhs_val < 0) {
            row_mult = -1.0;
            rhs_val = -rhs_val;
        }
        lp.b[i] = rhs_val;
        
        // Add original variables
        for (int j = 0; j < num_orig_vars; ++j) {
            double coeff = constr.coefficient(j);
            if (coeff != 0.0) {
                lp.A.add_element(i, j, row_mult * coeff);
            }
        }
        
        // Handle slacks based on constraint type
        ConstraintType ctype = constr.type();
        if (row_mult < 0) {
            // Flip constraint type
            if (ctype == ConstraintType::LessEq) ctype = ConstraintType::GreaterEq;
            else if (ctype == ConstraintType::GreaterEq) ctype = ConstraintType::LessEq;
        }
        
        if (ctype == ConstraintType::LessEq) {
            lp.A.add_element(i, current_slack_idx, 1.0);
            lp.var_mapping[current_slack_idx].original_var_index = -1; // It's a slack
            current_slack_idx++;
        } else if (ctype == ConstraintType::GreaterEq) {
            lp.A.add_element(i, current_slack_idx, -1.0);
            lp.var_mapping[current_slack_idx].original_var_index = -1; // It's a surplus
            current_slack_idx++;
        }
    }

    return lp;
}

SolverResult StandardFormConverter::map_back(const StandardFormLP& std_lp, const SolverResult& std_result, const Model& original_model) {
    SolverResult res = std_result;
    res.objective_value = std_result.objective_value;
    if (original_model.objective().sense() == ObjectiveSense::Maximize) {
        res.objective_value = -res.objective_value;
    }
    res.objective_value += original_model.objective().offset();

    res.primal_variables.assign(original_model.num_variables(), 0.0);
    for (size_t i = 0; i < std_result.primal_variables.size(); ++i) {
        int orig_idx = std_lp.var_mapping[i].original_var_index;
        if (orig_idx >= 0) {
            res.primal_variables[orig_idx] += std_lp.var_mapping[i].multiplier * std_result.primal_variables[i];
        }
    }
    return res;
}

} // namespace bharatopt
