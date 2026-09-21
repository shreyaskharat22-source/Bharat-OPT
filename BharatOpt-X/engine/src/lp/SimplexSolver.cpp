#include "bharatopt/lp/SimplexSolver.hpp"
#include "bharatopt/lp/StandardFormConverter.hpp"
#include "bharatopt/sparse/CSR.hpp"
#include <cmath>
#include <iostream>

namespace bharatopt {

SolverResult SimplexSolver::solve(const Model& model) {
    stats_ = SolverStatistics();
    iteration_history_.clear();
    StandardFormLP std_lp = StandardFormConverter::convert(model);
    
    build_tableau(std_lp);
    
    TerminationStatus status = run_phase_1();
    if (status != TerminationStatus::Optimal) {
        return StandardFormConverter::map_back(std_lp, extract_result(std_lp, status), model);
    }
    
    status = run_phase_2(std_lp);
    return StandardFormConverter::map_back(std_lp, extract_result(std_lp, status), model);
}

void SimplexSolver::build_tableau(const StandardFormLP& std_lp) {
    num_rows_ = std_lp.num_constraints;
    num_cols_ = std_lp.num_vars;
    
    // Determine which rows need artificial variables
    // For our simple standard form, we assume we just add an artificial var to every row for safety in Phase I
    num_artificial_ = num_rows_;
    int total_cols = num_cols_ + num_artificial_;
    
    tableau_.assign(num_rows_, std::vector<double>(total_cols, 0.0));
    rhs_ = std_lp.b;
    reduced_costs_.assign(total_cols, 0.0);
    basic_vars_.assign(num_rows_, -1);
    
    CSR csr = std_lp.A.to_csr();
    for (int r = 0; r < num_rows_; ++r) {
        for (int i = csr.row_ptrs()[r]; i < csr.row_ptrs()[r + 1]; ++i) {
            tableau_[r][csr.col_indices()[i]] = csr.values()[i];
        }
        // Add artificial variable to make a trivial basis
        int art_col = num_cols_ + r;
        tableau_[r][art_col] = 1.0;
        basic_vars_[r] = art_col;
    }
}

TerminationStatus SimplexSolver::run_phase_1() {
    // Phase 1 objective: minimize sum of artificial variables
    int total_cols = num_cols_ + num_artificial_;
    std::fill(reduced_costs_.begin(), reduced_costs_.end(), 0.0);
    for (int r = 0; r < num_rows_; ++r) {
        reduced_costs_[num_cols_ + r] = 1.0;
    }
    current_obj_ = 0.0;
    
    for (int r = 0; r < num_rows_; ++r) {
        int art_col = basic_vars_[r]; // Initially num_cols_ + r
        // The cost of artificial variable is 1. We price it out since it's basic.
        // Row operation to zero out reduced cost of basic variable:
        // reduced_costs = original_costs - sum_{i=0..m-1} (cost of basic var i) * row i
        for (int c = 0; c < total_cols; ++c) {
            reduced_costs_[c] -= tableau_[r][c];
        }
        current_obj_ -= rhs_[r];
    }
    
    TerminationStatus status = TerminationStatus::Unknown;
    while (step(status, true)) {}
    
    if (status == TerminationStatus::Optimal) {
        if (std::abs(current_obj_) > TOL) {
            return TerminationStatus::Infeasible;
        }
        
        // Drive degenerate artificial variables out of the basis
        for (int r = 0; r < num_rows_; ++r) {
            if (basic_vars_[r] >= num_cols_) {
                // Artificial variable is still in basis. Find a non-artificial pivot.
                int pivot_col = -1;
                for (int c = 0; c < num_cols_; ++c) {
                    if (std::abs(tableau_[r][c]) > TOL) {
                        pivot_col = c;
                        break;
                    }
                }
                
                if (pivot_col != -1) {
                    // Pivot it out
                    double pivot_val = tableau_[r][pivot_col];
                    for (int c = 0; c < num_cols_ + num_artificial_; ++c) {
                        tableau_[r][c] /= pivot_val;
                    }
                    rhs_[r] /= pivot_val;
                    
                    for (int i = 0; i < num_rows_; ++i) {
                        if (i != r) {
                            double factor = tableau_[i][pivot_col];
                            if (std::abs(factor) > TOL) {
                                for (int c = 0; c < num_cols_ + num_artificial_; ++c) {
                                    tableau_[i][c] -= factor * tableau_[r][c];
                                }
                                rhs_[i] -= factor * rhs_[r];
                            }
                        }
                    }
                    basic_vars_[r] = pivot_col;
                } else {
                    // Row is completely zero for non-artificials. It's a redundant row.
                }
            }
        }
        
        return TerminationStatus::Optimal;
    }
    return status;
}

TerminationStatus SimplexSolver::run_phase_2(const StandardFormLP& std_lp) {
    // Set up phase 2 objective
    std::fill(reduced_costs_.begin(), reduced_costs_.end(), 0.0);
    for (int j = 0; j < num_cols_; ++j) {
        reduced_costs_[j] = std_lp.c[j];
    }
    current_obj_ = 0.0;
    
    // Price out basic variables
    for (int r = 0; r < num_rows_; ++r) {
        int basic_col = basic_vars_[r];
        if (basic_col < num_cols_) {
            double c_B = reduced_costs_[basic_col];
            if (std::abs(c_B) > TOL) {
                for (int c = 0; c < num_cols_; ++c) {
                    reduced_costs_[c] -= c_B * tableau_[r][c];
                }
                current_obj_ -= c_B * rhs_[r];
            }
        }
    }
    
    TerminationStatus status = TerminationStatus::Unknown;
    while (step(status, false)) {}
    return status;
}

bool SimplexSolver::step(TerminationStatus& status, bool is_phase_1) {
    int max_cols = is_phase_1 ? (num_cols_ + num_artificial_) : num_cols_;
    
    // Bland's rule: Choose lowest index variable with negative reduced cost (minimization)
    int enter_col = -1;
    for (int c = 0; c < max_cols; ++c) {
        if (reduced_costs_[c] < -TOL) {
            enter_col = c;
            break;
        }
    }
    
    if (enter_col == -1) {
        status = TerminationStatus::Optimal;
        return false;
    }
    
    // Ratio test (Bland's rule: choose lowest index row in case of tie)
    int leave_row = -1;
    double min_ratio = 1e30;
    
    for (int r = 0; r < num_rows_; ++r) {
        double val = tableau_[r][enter_col];
        if (val > TOL) {
            double ratio = rhs_[r] / val;
            // Handle ties gracefully
            if (ratio < min_ratio - TOL) {
                min_ratio = ratio;
                leave_row = r;
            } else if (ratio < min_ratio + TOL) {
                if (leave_row != -1 && basic_vars_[r] < basic_vars_[leave_row]) {
                    leave_row = r;
                }
            }
        }
    }
    
    if (leave_row == -1) {
        status = TerminationStatus::Unbounded;
        return false;
    }
    
    int basic_vars_before = basic_vars_[leave_row];
    
    // Pivot
    double pivot_val = tableau_[leave_row][enter_col];
    for (int c = 0; c < max_cols; ++c) {
        tableau_[leave_row][c] /= pivot_val;
    }
    rhs_[leave_row] /= pivot_val;
    
    for (int r = 0; r < num_rows_; ++r) {
        if (r != leave_row) {
            double factor = tableau_[r][enter_col];
            if (std::abs(factor) > TOL) {
                for (int c = 0; c < max_cols; ++c) {
                    tableau_[r][c] -= factor * tableau_[leave_row][c];
                }
                rhs_[r] -= factor * rhs_[leave_row];
            }
        }
    }
    
    double reduced_cost_factor = reduced_costs_[enter_col];
    for (int c = 0; c < max_cols; ++c) {
        reduced_costs_[c] -= reduced_cost_factor * tableau_[leave_row][c];
    }
    current_obj_ -= reduced_cost_factor * rhs_[leave_row];
    
    basic_vars_[leave_row] = enter_col;
    stats_.iterations++;
    if (is_phase_1) stats_.phase1_iterations++;
    else stats_.phase2_iterations++;
    
    // Record iteration state
    IterationRecord record;
    record.iteration_number = stats_.iterations;
    record.is_phase_1 = is_phase_1;
    record.entering_variable = enter_col;
    record.leaving_variable = basic_vars_before;
    record.pivot_element = pivot_val;
    record.objective_value = -current_obj_;
    record.basic_variables = basic_vars_;
    record.reduced_costs = reduced_costs_;
    
    // Primal variables extract
    record.current_solution.assign(max_cols, 0.0);
    for (int r = 0; r < num_rows_; ++r) {
        if (basic_vars_[r] >= 0 && basic_vars_[r] < max_cols) {
            record.current_solution[basic_vars_[r]] = rhs_[r];
        }
    }
    
    iteration_history_.push_back(std::move(record));
    
    return true;
}

SolverResult SimplexSolver::extract_result(const StandardFormLP& std_lp, TerminationStatus status) {
    SolverResult res;
    res.status = status;
    res.objective_value = -current_obj_;
    res.primal_variables.assign(std_lp.num_vars, 0.0);
    for (int r = 0; r < num_rows_; ++r) {
        int bc = basic_vars_[r];
        if (bc < std_lp.num_vars) {
            res.primal_variables[bc] = rhs_[r];
        }
    }
    // Duals / reduced costs can be extracted from reduced_costs_ in phase 2.
    // For Phase 1 infeasible, we just return what we have.
    res.reduced_costs = std::vector<double>(reduced_costs_.begin(), reduced_costs_.begin() + std_lp.num_vars);
    // Dual variables extraction is more complex, typically negative of reduced costs of slacks.
    // We will leave dual variables empty for standard simple simplex unless requested.
    res.iteration_history = iteration_history_;
    return res;
}

} // namespace bharatopt
