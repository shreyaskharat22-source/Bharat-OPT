#pragma once

#include <vector>

namespace bharatopt {

struct IterationRecord {
    int iteration_number = 0;
    bool is_phase_1 = false;
    int entering_variable = -1; // -1 if none
    int leaving_variable = -1;  // -1 if none (row index)
    double pivot_element = 0.0;
    double objective_value = 0.0;
    
    std::vector<int> basic_variables;
    std::vector<double> reduced_costs;
    std::vector<double> current_solution; // Extract current primal values
};

} // namespace bharatopt
