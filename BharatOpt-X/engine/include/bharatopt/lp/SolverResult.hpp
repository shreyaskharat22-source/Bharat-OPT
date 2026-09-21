#pragma once

#include "TerminationStatus.hpp"
#include "IterationRecord.hpp"
#include <vector>

namespace bharatopt {

struct SolverResult {
    TerminationStatus status = TerminationStatus::Unknown;
    double objective_value = 0.0;
    std::vector<double> primal_variables;
    std::vector<double> dual_variables;
    std::vector<double> reduced_costs;
    std::vector<IterationRecord> iteration_history;
};

} // namespace bharatopt
