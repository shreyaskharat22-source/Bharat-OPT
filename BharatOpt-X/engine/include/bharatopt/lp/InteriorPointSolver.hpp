#pragma once

#include "bharatopt/lp/LPSolver.hpp"
#include "bharatopt/lp/StandardFormLP.hpp"

namespace bharatopt {

class InteriorPointSolver : public LPSolver {
public:
    InteriorPointSolver() = default;

    SolverResult solve(const Model& model) override;
    
private:
    const double TOL = 1e-6;
    const int MAX_ITERS = 100;
};

} // namespace bharatopt
