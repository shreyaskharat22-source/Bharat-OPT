#pragma once

#include "bharatopt/lp/LPSolver.hpp"
#include "bharatopt/lp/StandardFormLP.hpp"

namespace bharatopt {

class RevisedSimplexSolver : public LPSolver {
public:
    RevisedSimplexSolver() = default;

    SolverResult solve(const Model& model) override;

private:
    TerminationStatus run_phase_1(const StandardFormLP& std_lp);
    TerminationStatus run_phase_2(const StandardFormLP& std_lp);

    bool step(const StandardFormLP& std_lp, TerminationStatus& status, bool is_phase_1);
    
    std::vector<double> extract_column(const StandardFormLP& std_lp, int col) const;
    std::vector<double> build_dense_B() const;

    int num_rows_ = 0;
    int num_cols_ = 0;
    
    std::vector<int> basic_vars_;
    std::vector<double> current_costs_;
    double current_obj_ = 0.0;
    std::vector<double> primal_rhs_;
    
    const double TOL = 1e-9;
};

} // namespace bharatopt
