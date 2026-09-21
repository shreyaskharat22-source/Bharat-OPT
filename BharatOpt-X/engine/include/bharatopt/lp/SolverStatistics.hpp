#pragma once

namespace bharatopt {

struct SolverStatistics {
    int iterations = 0;
    int phase1_iterations = 0;
    int phase2_iterations = 0;
    
    int presolve_rows_eliminated = 0;
    int presolve_cols_eliminated = 0;
    
    double elapsed_time_sec = 0.0;
};

} // namespace bharatopt
