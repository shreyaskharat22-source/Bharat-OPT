#pragma once

#include "Types.hpp"
#include <vector>

namespace bharatopt {

class Objective {
public:
    Objective() : sense_(ObjectiveSense::Minimize), offset_(0.0) {}
    
    void set_sense(ObjectiveSense sense) { sense_ = sense; }
    ObjectiveSense sense() const { return sense_; }
    
    void set_offset(double offset) { offset_ = offset; }
    double offset() const { return offset_; }
    
    void set_coefficient(int var_id, double coeff) {
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

private:
    ObjectiveSense sense_;
    double offset_;
    std::vector<double> coeffs_;
};

} // namespace bharatopt
