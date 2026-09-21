#include "bharatopt/milp/MILPSolver.hpp"
#include <cmath>
#include <limits>
#include <iostream>

namespace bharatopt {

MILPSolver::MILPSolver() {
    // Default strategies
    node_selector_ = std::make_unique<DepthFirstSelector>();
    branching_rule_ = std::make_unique<MostFractionalBranching>();
}

void MILPSolver::set_node_selector(std::unique_ptr<NodeSelector> selector) {
    node_selector_ = std::move(selector);
}

void MILPSolver::set_branching_rule(std::unique_ptr<BranchingRule> rule) {
    branching_rule_ = std::move(rule);
}

void MILPSolver::add_cut_generator(std::unique_ptr<CutGenerator> generator) {
    cut_generators_.push_back(std::move(generator));
}

Model MILPSolver::build_node_model(const Model& original_model, const MILPNode& node) {
    Model m = original_model; // Copy to retain variables and structure
    
    // Apply bounds as constraints since LPSolver expects Standard Form (x >= 0)
    for (size_t i = 0; i < node.variable_bounds.size(); ++i) {
        double lb = node.variable_bounds[i].lower;
        double ub = node.variable_bounds[i].upper;
        
        std::cout << "[build] var " << i << " [" << lb << ", " << ub << "]\n";
        
        if (lb > 0) {
            int cid = m.add_constraint("bound_lb_" + std::to_string(i), ConstraintType::GreaterEq, lb);
            m.set_constraint_coefficient(cid, i, 1.0);
        }
        if (ub < 1e30) {
            int cid = m.add_constraint("bound_ub_" + std::to_string(i), ConstraintType::LessEq, ub);
            m.set_constraint_coefficient(cid, i, 1.0);
        }
    }
    return m;
}

SolverResult MILPSolver::solve(const Model& model) {
    stats_ = MILPStatistics();
    bool is_maximize = (model.objective().sense() == ObjectiveSense::Maximize);
    stats_.best_incumbent = is_maximize ? -1e30 : 1e30;
    
    std::vector<double> best_solution(model.num_variables(), 0.0);
    
    // Root node
    MILPNode root;
    root.id = 0;
    root.parent_id = -1;
    root.depth = 0;
    root.variable_bounds.reserve(model.num_variables());
    for (size_t i = 0; i < model.num_variables(); ++i) {
        root.variable_bounds.push_back(model.variable(i).bounds());
    }
    
    node_selector_->push(root);
    int node_counter = 0;
    
    while (!node_selector_->empty()) {
        MILPNode current_node = node_selector_->pop();
        stats_.nodes_explored++;
        
        // Solve LP relaxation
        Model node_model = build_node_model(model, current_node);
        SimplexSolver lp_solver;
        SolverResult lp_result = lp_solver.solve(node_model);
        stats_.lp_relaxations_solved++;
        
        // Root bound initialization
        if (current_node.id == 0) {
            if (lp_result.status == TerminationStatus::Optimal) {
                stats_.initial_lp_bound = lp_result.objective_value;
            } else {
                return lp_result; // Root is infeasible or unbounded
            }
        }
        
        // 1. Infeasibility Pruning
        if (lp_result.status != TerminationStatus::Optimal) {
            stats_.nodes_pruned_infeasibility++;
            continue;
        }
        
        current_node.lp_bound = lp_result.objective_value;
        current_node.lp_solution = lp_result.primal_variables; // this holds the original var size
        
        // 2. Bound Pruning
        if (is_maximize) {
            if (current_node.lp_bound <= stats_.best_incumbent + 1e-9) {
                stats_.nodes_pruned_bound++;
                continue;
            }
        } else {
            if (current_node.lp_bound >= stats_.best_incumbent - 1e-9) {
                stats_.nodes_pruned_bound++;
                continue;
            }
        }
        
        // 3. Integrality Check
        bool is_integer_feasible = true;
        for (size_t i = 0; i < model.num_variables(); ++i) {
            if (model.variable(i).type() != VariableType::Continuous) {
                double val = current_node.lp_solution[i];
                if (std::abs(val - std::round(val)) > INT_TOL) {
                    is_integer_feasible = false;
                    break;
                }
            }
        }
        
        if (is_integer_feasible) {
            stats_.nodes_pruned_integrality++;
            stats_.best_incumbent = current_node.lp_bound;
            best_solution = current_node.lp_solution;
            
            // Calculate MIP gap
            if (stats_.best_incumbent != 0) {
                stats_.mip_gap = std::abs(stats_.initial_lp_bound - stats_.best_incumbent) / std::abs(stats_.best_incumbent);
            }
            continue;
        }
        
        // 4. Branch and Cut (Cut Generation)
        // [Simplified: Currently we don't loop cuts, just prepare the structure]
        for (auto& generator : cut_generators_) {
            auto cuts = generator->generate_cuts(lp_solver, model, current_node.lp_solution);
            stats_.cuts_generated += cuts.size();
            // TODO: Append cuts to node model and re-solve if implemented fully
        }
        
        // 5. Branching
        int branch_var = branching_rule_->select_branching_variable(current_node, model);
        if (branch_var != -1) {
            double val = current_node.lp_solution[branch_var];
            double left_ub = std::floor(val);
            double right_lb = std::ceil(val);
            
            // Create Left Child (var <= floor(val))
            MILPNode left_child = current_node;
            left_child.id = ++node_counter;
            left_child.parent_id = current_node.id;
            left_child.depth = current_node.depth + 1;
            left_child.variable_bounds[branch_var].upper = std::min(left_child.variable_bounds[branch_var].upper, left_ub);
            
            // Create Right Child (var >= ceil(val))
            MILPNode right_child = current_node;
            right_child.id = ++node_counter;
            right_child.parent_id = current_node.id;
            right_child.depth = current_node.depth + 1;
            right_child.variable_bounds[branch_var].lower = std::max(right_child.variable_bounds[branch_var].lower, right_lb);
            
            node_selector_->push(left_child);
            node_selector_->push(right_child);
        }
    }
    
    SolverResult final_res;
    if ((is_maximize && stats_.best_incumbent == -1e30) || (!is_maximize && stats_.best_incumbent == 1e30)) {
        final_res.status = TerminationStatus::Infeasible;
    } else {
        final_res.status = TerminationStatus::Optimal;
        final_res.objective_value = stats_.best_incumbent;
        final_res.primal_variables = best_solution;
    }
    
    return final_res;
}

} // namespace bharatopt
