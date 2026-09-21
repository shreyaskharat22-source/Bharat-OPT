#pragma once

namespace bharatopt {

enum class VariableType {
    Continuous,
    Integer,
    Binary
};

enum class ConstraintType {
    LessEq,
    GreaterEq,
    Equal
};

enum class ObjectiveSense {
    Minimize,
    Maximize
};

} // namespace bharatopt
