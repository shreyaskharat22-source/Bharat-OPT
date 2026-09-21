#pragma once

#include <vector>

namespace bharatopt {

class CSR;
class CSC;

class COO {
public:
    COO(int rows, int cols);

    void add_element(int row, int col, double value);

    int rows() const { return rows_; }
    int cols() const { return cols_; }
    int nnz() const { return static_cast<int>(values_.size()); }
    
    double density() const;

    const std::vector<int>& row_indices() const { return row_indices_; }
    const std::vector<int>& col_indices() const { return col_indices_; }
    const std::vector<double>& values() const { return values_; }

    CSR to_csr() const;
    CSC to_csc() const;
    COO transpose() const;

    std::vector<double> spmv(const std::vector<double>& x) const;

private:
    int rows_;
    int cols_;
    std::vector<int> row_indices_;
    std::vector<int> col_indices_;
    std::vector<double> values_;
};

} // namespace bharatopt
