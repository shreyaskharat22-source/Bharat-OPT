#pragma once

#include "Variable.hpp"
#include "Constraint.hpp"
#include "Objective.hpp"
#include <vector>
#include <memory>
#include <string>

namespace bharatopt {

class Model {
public:
    Model(std::string name = "BharatOptModel");

    const std::string& name() const { return name_; }

    int add_variable(std::string name, VariableType type, Bounds bounds);
    int add_constraint(std::string name, ConstraintType type, double rhs);

    void set_constraint_coefficient(int constr_id, int var_id, double coeff);

    const Variable& variable(int id) const;
    const Constraint& constraint(int id) const;

    size_t num_variables() const { return variables_.size(); }
    size_t num_constraints() const { return constraints_.size(); }

    Objective& objective() { return objective_; }
    const Objective& objective() const { return objective_; }

private:
    std::string name_;
    std::vector<Variable> variables_;
    std::vector<Constraint> constraints_;
    Objective objective_;
};

} // namespace bharatopt
