#pragma once

#include <vector>

namespace bharatopt {

class COO;
class CSC;

class CSR {
public:
    CSR(int rows, int cols, std::vector<int> row_ptrs, std::vector<int> col_indices, std::vector<double> values);

    int rows() const { return rows_; }
    int cols() const { return cols_; }
    int nnz() const { return static_cast<int>(values_.size()); }

    double density() const;

    const std::vector<int>& row_ptrs() const { return row_ptrs_; }
    const std::vector<int>& col_indices() const { return col_indices_; }
    const std::vector<double>& values() const { return values_; }

    COO to_coo() const;
    CSR transpose() const;

    std::vector<double> spmv(const std::vector<double>& x) const;

private:
    int rows_;
    int cols_;
    std::vector<int> row_ptrs_;
    std::vector<int> col_indices_;
    std::vector<double> values_;
};

} // namespace bharatopt
