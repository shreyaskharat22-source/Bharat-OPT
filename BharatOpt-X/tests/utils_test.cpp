#include <gtest/gtest.h>
#include "bharatopt/utils/Numerics.hpp"
#include <limits>

using namespace bharatopt;

TEST(NumericsTest, Validations) {
    std::vector<double> v1 = {-1.5, 2.0, -10.0, 4.0};
    EXPECT_DOUBLE_EQ(Numerics::infinity_norm(v1), 10.0);

    std::vector<double> b = {10.0, 20.0};
    std::vector<double> Ax = {9.0, 21.0};
    std::vector<double> r = Numerics::compute_residual(b, Ax);
    EXPECT_DOUBLE_EQ(r[0], 1.0);
    EXPECT_DOUBLE_EQ(r[1], -1.0);

    EXPECT_TRUE(Numerics::is_zero(1e-10));
    EXPECT_FALSE(Numerics::is_zero(1e-5));

    std::vector<double> v_inf = {1.0, std::numeric_limits<double>::infinity()};
    std::vector<double> v_nan = {1.0, std::numeric_limits<double>::quiet_NaN()};
    EXPECT_TRUE(Numerics::is_finite(v1));
    EXPECT_FALSE(Numerics::is_finite(v_inf));
    EXPECT_FALSE(Numerics::is_finite(v_nan));
}
