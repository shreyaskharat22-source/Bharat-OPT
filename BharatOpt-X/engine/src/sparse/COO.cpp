#include "bharatopt/sparse/COO.hpp"
#include "bharatopt/sparse/CSR.hpp"
#include "bharatopt/sparse/CSC.hpp"
#include <stdexcept>
#include <numeric>

namespace bharatopt {

COO::COO(int rows, int cols) : rows_(rows), cols_(cols) {
    if (rows < 0 || cols < 0) {
        throw std::invalid_argument("Matrix dimensions must be non-negative");
    }
}

void COO::add_element(int row, int col, double value) {
    if (row < 0 || row >= rows_ || col < 0 || col >= cols_) {
        throw std::out_of_range("Row or column index out of range");
    }
    row_indices_.push_back(row);
    col_indices_.push_back(col);
    values_.push_back(value);
}

double COO::density() const {
    double total = static_cast<double>(rows_) * static_cast<double>(cols_);
    return static_cast<double>(nnz()) / total;
}

CSR COO::to_csr() const {
    std::vector<int> row_ptrs(rows_ + 1, 0);
    // Count occurrences of each row
    for (int r : row_indices_) {
        row_ptrs[r + 1]++;
    }
    // Prefix sum
    for (int i = 0; i < rows_; ++i) {
        row_ptrs[i + 1] += row_ptrs[i];
    }
    
    std::vector<int> csr_cols(nnz());
    std::vector<double> csr_vals(nnz());
    
    // We need a copy of row_ptrs to use as current offsets
    std::vector<int> current_ptrs = row_ptrs;
    for (size_t i = 0; i < nnz(); ++i) {
        int row = row_indices_[i];
        int dest = current_ptrs[row]++;
        csr_cols[dest] = col_indices_[i];
        csr_vals[dest] = values_[i];
    }
    
    return CSR(rows_, cols_, std::move(row_ptrs), std::move(csr_cols), std::move(csr_vals));
}

CSC COO::to_csc() const {
    std::vector<int> col_ptrs(cols_ + 1, 0);
    for (int c : col_indices_) {
        col_ptrs[c + 1]++;
    }
    for (int i = 0; i < cols_; ++i) {
        col_ptrs[i + 1] += col_ptrs[i];
    }
    
    std::vector<int> csc_rows(nnz());
    std::vector<double> csc_vals(nnz());
    
    std::vector<int> current_ptrs = col_ptrs;
    for (size_t i = 0; i < nnz(); ++i) {
        int col = col_indices_[i];
        int dest = current_ptrs[col]++;
        csc_rows[dest] = row_indices_[i];
        csc_vals[dest] = values_[i];
    }
    
    return CSC(rows_, cols_, std::move(col_ptrs), std::move(csc_rows), std::move(csc_vals));
}

COO COO::transpose() const {
    COO t(cols_, rows_);
    for (size_t i = 0; i < nnz(); ++i) {
        t.add_element(col_indices_[i], row_indices_[i], values_[i]);
    }
    return t;
}

std::vector<double> COO::spmv(const std::vector<double>& x) const {
    if (x.size() != static_cast<size_t>(cols_)) {
        throw std::invalid_argument("Vector dimension mismatch in SpMV");
    }
    std::vector<double> y(rows_, 0.0);
    for (size_t i = 0; i < nnz(); ++i) {
        y[row_indices_[i]] += values_[i] * x[col_indices_[i]];
    }
    return y;
}

} // namespace bharatopt
