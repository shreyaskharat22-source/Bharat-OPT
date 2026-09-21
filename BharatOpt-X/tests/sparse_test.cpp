#include <gtest/gtest.h>
#include "bharatopt/sparse/COO.hpp"
#include "bharatopt/sparse/CSR.hpp"
#include "bharatopt/sparse/CSC.hpp"

using namespace bharatopt;

TEST(SparseTest, ConversionsAndSpMV) {
    // 3x3 matrix
    // 1 0 2
    // 0 3 0
    // 4 5 0
    COO coo(3, 3);
    coo.add_element(0, 0, 1.0);
    coo.add_element(0, 2, 2.0);
    coo.add_element(1, 1, 3.0);
    coo.add_element(2, 0, 4.0);
    coo.add_element(2, 1, 5.0);

    EXPECT_EQ(coo.nnz(), 5);
    EXPECT_DOUBLE_EQ(coo.density(), 5.0 / 9.0);

    std::vector<double> x = {1.0, 2.0, 3.0};
    // y = A * x
    // row 0: 1*1 + 2*3 = 7
    // row 1: 3*2 = 6
    // row 2: 4*1 + 5*2 = 14
    std::vector<double> expected = {7.0, 6.0, 14.0};
    
    EXPECT_EQ(coo.spmv(x), expected);

    CSR csr = coo.to_csr();
    EXPECT_EQ(csr.nnz(), 5);
    EXPECT_EQ(csr.spmv(x), expected);

    CSC csc = coo.to_csc();
    EXPECT_EQ(csc.nnz(), 5);
    EXPECT_EQ(csc.spmv(x), expected);

    CSR t_csr = csr.transpose();
    EXPECT_EQ(t_csr.rows(), 3);
    EXPECT_EQ(t_csr.cols(), 3);
    
    // Transpose A^T * x
    // A^T =
    // 1 0 4
    // 0 3 5
    // 2 0 0
    // y = A^T * x
    // row 0: 1*1 + 4*3 = 13
    // row 1: 3*2 + 5*3 = 21
    // row 2: 2*1 = 2
    std::vector<double> t_expected = {13.0, 21.0, 2.0};
    EXPECT_EQ(t_csr.spmv(x), t_expected);
}
