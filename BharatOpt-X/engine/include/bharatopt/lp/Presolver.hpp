#pragma once

#include "bharatopt/model/Model.hpp"

namespace bharatopt {

class Presolver {
public:
    static void presolve(Model& model);
};

} // namespace bharatopt
