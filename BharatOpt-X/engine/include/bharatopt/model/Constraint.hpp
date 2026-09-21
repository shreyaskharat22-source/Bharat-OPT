#pragma once

#include "Types.hpp"
#include <string>
#include <vector>

namespace bharatopt {

class Constraint {
public:
    Constraint(int id, std::string name, ConstraintType type, double rhs)
        : id_(id), name_(std::move(name)), type_(type), rhs_(rhs) {}

    int id() const { return id_; }
    const std::string& name() const { return name_; }
    ConstraintType type() const { return type_; }
    double rhs() const { return rhs_; }

    void set_coefficient(int var_id, double coeff) {
        // Simple dense vector for now or sparse representation.
        // We'll use a dense vector internally for simplicity of implementation
        if (var_id >= coeffs_.size()) {
            coeffs_.resize(var_id + 1, 0.0);
        }
        coeffs_[var_id] = coeff;
    }

    double coefficient(int var_id) const {
        if (var_id < coeffs_.size()) {
            return coeffs_[var_id];
        }
        return 0.0;
    }

    const std::vector<double>& coefficients() const { return coeffs_; }

private:
    int id_;
    std::string name_;
    ConstraintType type_;
    double rhs_;
    std::vector<double> coeffs_;
};

} // namespace bharatopt
