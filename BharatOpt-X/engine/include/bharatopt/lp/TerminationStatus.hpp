#pragma once

namespace bharatopt {

enum class TerminationStatus {
    Optimal,
    Infeasible,
    Unbounded,
    MaxIterations,
    NumericalError,
    Presolved,
    Unknown
};

} // namespace bharatopt
