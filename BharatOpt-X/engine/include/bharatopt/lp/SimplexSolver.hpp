#pragma once

#include "bharatopt/lp/LPSolver.hpp"
#include "bharatopt/lp/StandardFormLP.hpp"
#include <vector>

namespace bharatopt {

class SimplexSolver : public LPSolver {
public:
    SimplexSolver() = default;

    SolverResult solve(const Model& model) override;

    const std::vector<std::vector<double>>& get_final_tableau() const { return tableau_; }
    const std::vector<double>& get_final_rhs() const { return rhs_; }
    const std::vector<int>& get_basic_variables() const { return basic_vars_; }

private:
    void build_tableau(const StandardFormLP& std_lp);
    TerminationStatus run_phase_1();
    TerminationStatus run_phase_2(const StandardFormLP& std_lp);
    
    // Core simplex step
    // Returns true if a pivot was performed, false if optimal/unbounded
    bool step(TerminationStatus& status, bool is_phase_1);
    
    // Extract result from tableau
    SolverResult extract_result(const StandardFormLP& std_lp, TerminationStatus status);

    // Tableau components
    int num_rows_ = 0;
    int num_cols_ = 0; // Does not include RHS
    int num_artificial_ = 0;
    
    std::vector<std::vector<double>> tableau_;
    std::vector<double> rhs_;
    std::vector<double> reduced_costs_;
    double current_obj_ = 0.0;
    
    std::vector<int> basic_vars_; // basic_vars_[i] is the variable basic in row i
    
    std::vector<IterationRecord> iteration_history_;
    
    // Tolerances
    const double TOL = 1e-9;
};

} // namespace bharatopt
