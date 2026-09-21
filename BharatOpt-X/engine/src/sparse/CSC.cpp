#include "bharatopt/sparse/CSC.hpp"
#include "bharatopt/sparse/COO.hpp"
#include <stdexcept>

namespace bharatopt {

CSC::CSC(int rows, int cols, std::vector<int> col_ptrs, std::vector<int> row_indices, std::vector<double> values)
    : rows_(rows), cols_(cols), col_ptrs_(std::move(col_ptrs)), row_indices_(std::move(row_indices)), values_(std::move(values)) {
    if (rows < 0 || cols < 0) {
        throw std::invalid_argument("Matrix dimensions must be non-negative");
    }
    if (col_ptrs_.size() != static_cast<size_t>(cols + 1)) {
        throw std::invalid_argument("col_ptrs must have size cols + 1");
    }
    if (row_indices_.size() != values_.size()) {
        throw std::invalid_argument("row_indices and values must have the same size");
    }
}

double CSC::density() const {
    double total = static_cast<double>(rows_) * static_cast<double>(cols_);
    return static_cast<double>(nnz()) / total;
}

COO CSC::to_coo() const {
    COO coo(rows_, cols_);
    for (int c = 0; c < cols_; ++c) {
        for (int i = col_ptrs_[c]; i < col_ptrs_[c + 1]; ++i) {
            coo.add_element(row_indices_[i], c, values_[i]);
        }
    }
    return coo;
}

CSC CSC::transpose() const {
    return to_coo().transpose().to_csc();
}

std::vector<double> CSC::spmv(const std::vector<double>& x) const {
    if (x.size() != static_cast<size_t>(cols_)) {
        throw std::invalid_argument("Vector dimension mismatch in SpMV");
    }
    std::vector<double> y(rows_, 0.0);
    for (int c = 0; c < cols_; ++c) {
        double val_x = x[c];
        for (int i = col_ptrs_[c]; i < col_ptrs_[c + 1]; ++i) {
            y[row_indices_[i]] += values_[i] * val_x;
        }
    }
    return y;
}

} // namespace bharatopt
