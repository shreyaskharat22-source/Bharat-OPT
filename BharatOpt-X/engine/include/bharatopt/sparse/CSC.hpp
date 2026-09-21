#pragma once

#include <vector>

namespace bharatopt {

class COO;
class CSR;

class CSC {
public:
    CSC(int rows, int cols, std::vector<int> col_ptrs, std::vector<int> row_indices, std::vector<double> values);

    int rows() const { return rows_; }
    int cols() const { return cols_; }
    int nnz() const { return static_cast<int>(values_.size()); }

    double density() const;

    const std::vector<int>& col_ptrs() const { return col_ptrs_; }
    const std::vector<int>& row_indices() const { return row_indices_; }
    const std::vector<double>& values() const { return values_; }

    COO to_coo() const;
    CSC transpose() const;

    std::vector<double> spmv(const std::vector<double>& x) const;

private:
    int rows_;
    int cols_;
    std::vector<int> col_ptrs_;
    std::vector<int> row_indices_;
    std::vector<double> values_;
};

} // namespace bharatopt
