#pragma once

#include "bharatopt/model/Model.hpp"
#include "StandardFormLP.hpp"
#include "bharatopt/lp/SolverResult.hpp"

namespace bharatopt {

class StandardFormConverter {
public:
    static StandardFormLP convert(const Model& model);
    static SolverResult map_back(const StandardFormLP& std_lp, const SolverResult& std_result, const Model& original_model);
};

} // namespace bharatopt
