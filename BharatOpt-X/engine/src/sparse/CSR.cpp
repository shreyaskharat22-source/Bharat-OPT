#include "bharatopt/sparse/CSR.hpp"
#include "bharatopt/sparse/COO.hpp"
#include <stdexcept>

namespace bharatopt {

CSR::CSR(int rows, int cols, std::vector<int> row_ptrs, std::vector<int> col_indices, std::vector<double> values)
    : rows_(rows), cols_(cols), row_ptrs_(std::move(row_ptrs)), col_indices_(std::move(col_indices)), values_(std::move(values)) {
    if (rows < 0 || cols < 0) {
        throw std::invalid_argument("Matrix dimensions must be non-negative");
    }
    if (row_ptrs_.size() != static_cast<size_t>(rows + 1)) {
        throw std::invalid_argument("row_ptrs must have size rows + 1");
    }
    if (col_indices_.size() != values_.size()) {
        throw std::invalid_argument("col_indices and values must have the same size");
    }
}

double CSR::density() const {
    double total = static_cast<double>(rows_) * static_cast<double>(cols_);
    return static_cast<double>(nnz()) / total;
}

COO CSR::to_coo() const {
    COO coo(rows_, cols_);
    for (int r = 0; r < rows_; ++r) {
        for (int i = row_ptrs_[r]; i < row_ptrs_[r + 1]; ++i) {
            coo.add_element(r, col_indices_[i], values_[i]);
        }
    }
    return coo;
}

CSR CSR::transpose() const {
    // A simple transpose logic: convert to COO, transpose, then to CSR (or equivalently CSC)
    // Here we just use the to_coo fallback for simplicity of the phase 1 implementation
    return to_coo().transpose().to_csr();
}

std::vector<double> CSR::spmv(const std::vector<double>& x) const {
    if (x.size() != static_cast<size_t>(cols_)) {
        throw std::invalid_argument("Vector dimension mismatch in SpMV");
    }
    std::vector<double> y(rows_, 0.0);
    for (int r = 0; r < rows_; ++r) {
        double sum = 0.0;
        for (int i = row_ptrs_[r]; i < row_ptrs_[r + 1]; ++i) {
            sum += values_[i] * x[col_indices_[i]];
        }
        y[r] = sum;
    }
    return y;
}

} // namespace bharatopt
