#pragma once

#include "bharatopt/lp/StandardFormLP.hpp"

namespace bharatopt {

class Scaler {
public:
    // Ruiz scaling or geometric mean scaling
    static void scale(StandardFormLP& lp);
};

} // namespace bharatopt
