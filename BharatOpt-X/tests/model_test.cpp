#include <gtest/gtest.h>
#include "bharatopt/model/Model.hpp"

using namespace bharatopt;

TEST(ModelTest, CreateVariableAndConstraint) {
    Model m("TestModel");
    EXPECT_EQ(m.name(), "TestModel");

    int x = m.add_variable("x", VariableType::Continuous, Bounds(0.0, 10.0));
    int y = m.add_variable("y", VariableType::Integer, Bounds(0.0, 5.0));

    EXPECT_EQ(m.num_variables(), 2);
    EXPECT_EQ(m.variable(x).name(), "x");
    EXPECT_EQ(m.variable(x).type(), VariableType::Continuous);
    EXPECT_EQ(m.variable(y).bounds().upper, 5.0);

    int c = m.add_constraint("c1", ConstraintType::LessEq, 15.0);
    EXPECT_EQ(m.num_constraints(), 1);
    EXPECT_EQ(m.constraint(c).name(), "c1");
    EXPECT_EQ(m.constraint(c).rhs(), 15.0);
}
