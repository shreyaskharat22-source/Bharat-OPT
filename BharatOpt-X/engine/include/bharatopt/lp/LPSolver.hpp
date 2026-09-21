#pragma once

#include "SolverResult.hpp"
#include "SolverStatistics.hpp"
#include "bharatopt/model/Model.hpp"

namespace bharatopt {

class LPSolver {
public:
    virtual ~LPSolver() = default;

    virtual SolverResult solve(const Model& model) = 0;

    const SolverStatistics& statistics() const { return stats_; }

protected:
    SolverStatistics stats_;
};

} // namespace bharatopt
