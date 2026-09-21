#pragma once

#include <limits>

namespace bharatopt {

struct Bounds {
    double lower = -std::numeric_limits<double>::infinity();
    double upper = std::numeric_limits<double>::infinity();

    Bounds() = default;
    Bounds(double l, double u) : lower(l), upper(u) {}
};

} // namespace bharatopt
