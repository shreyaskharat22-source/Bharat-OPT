#include "bharatopt/model/Model.hpp"
#include <stdexcept>

namespace bharatopt {

Model::Model(std::string name) : name_(std::move(name)) {}

int Model::add_variable(std::string name, VariableType type, Bounds bounds) {
    int id = static_cast<int>(variables_.size());
    variables_.emplace_back(id, std::move(name), type, bounds);
    return id;
}

int Model::add_constraint(std::string name, ConstraintType type, double rhs) {
    int id = static_cast<int>(constraints_.size());
    constraints_.emplace_back(id, std::move(name), type, rhs);
    return id;
}

void Model::set_constraint_coefficient(int constr_id, int var_id, double coeff) {
    if (constr_id < 0 || constr_id >= static_cast<int>(constraints_.size())) {
        throw std::out_of_range("Constraint ID out of range");
    }
    constraints_[constr_id].set_coefficient(var_id, coeff);
}

const Variable& Model::variable(int id) const {
    if (id < 0 || id >= static_cast<int>(variables_.size())) {
        throw std::out_of_range("Variable ID out of range");
    }
    return variables_[id];
}

const Constraint& Model::constraint(int id) const {
    if (id < 0 || id >= static_cast<int>(constraints_.size())) {
        throw std::out_of_range("Constraint ID out of range");
    }
    return constraints_[id];
}

} // namespace bharatopt
