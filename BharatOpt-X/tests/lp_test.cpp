#include <gtest/gtest.h>
#include "bharatopt/model/Model.hpp"
#include "bharatopt/lp/SimplexSolver.hpp"

using namespace bharatopt;

TEST(LPSolverTest, SmallLP) {
    // max 3x + 2y
    // s.t. 2x + y <= 18
    //      2x + 3y <= 42
    //      3x + y <= 24
    //      x, y >= 0
    // Expected optimal obj = 33 (x=3, y=12)

    Model m("SmallLP");
    m.objective().set_sense(ObjectiveSense::Maximize);

    int x = m.add_variable("x", VariableType::Continuous, Bounds(0.0, 1e30));
    int y = m.add_variable("y", VariableType::Continuous, Bounds(0.0, 1e30));

    m.objective().set_coefficient(x, 3.0);
    m.objective().set_coefficient(y, 2.0);

    int c1 = m.add_constraint("c1", ConstraintType::LessEq, 18.0);
    m.set_constraint_coefficient(c1, x, 2.0);
    m.set_constraint_coefficient(c1, y, 1.0);

    int c2 = m.add_constraint("c2", ConstraintType::LessEq, 42.0);
    m.set_constraint_coefficient(c2, x, 2.0);
    m.set_constraint_coefficient(c2, y, 3.0);

    int c3 = m.add_constraint("c3", ConstraintType::LessEq, 24.0);
    m.set_constraint_coefficient(c3, x, 3.0);
    m.set_constraint_coefficient(c3, y, 1.0);

    SimplexSolver solver;
    SolverResult res = solver.solve(m);

    EXPECT_EQ(res.status, TerminationStatus::Optimal);
    EXPECT_NEAR(res.objective_value, 33.0, 1e-6);
    EXPECT_NEAR(res.primal_variables[x], 3.0, 1e-6);
    EXPECT_NEAR(res.primal_variables[y], 12.0, 1e-6);
}

TEST(LPSolverTest, UnboundedLP) {
    // max x + y
    // s.t. x - y <= 1
    //      -x + y <= 1

    Model m("UnboundedLP");
    m.objective().set_sense(ObjectiveSense::Maximize);

    int x = m.add_variable("x", VariableType::Continuous, Bounds(0.0, 1e30));
    int y = m.add_variable("y", VariableType::Continuous, Bounds(0.0, 1e30));

    m.objective().set_coefficient(x, 1.0);
    m.objective().set_coefficient(y, 1.0);

    int c1 = m.add_constraint("c1", ConstraintType::LessEq, 1.0);
    m.set_constraint_coefficient(c1, x, 1.0);
    m.set_constraint_coefficient(c1, y, -1.0);

    int c2 = m.add_constraint("c2", ConstraintType::LessEq, 1.0);
    m.set_constraint_coefficient(c2, x, -1.0);
    m.set_constraint_coefficient(c2, y, 1.0);

    SimplexSolver solver;
    SolverResult res = solver.solve(m);

    EXPECT_EQ(res.status, TerminationStatus::Unbounded);
}

TEST(LPSolverTest, InfeasibleLP) {
    // min x + y
    // s.t. x + y <= 1
    //      x + y >= 2

    Model m("InfeasibleLP");
    m.objective().set_sense(ObjectiveSense::Minimize);

    int x = m.add_variable("x", VariableType::Continuous, Bounds(0.0, 1e30));
    int y = m.add_variable("y", VariableType::Continuous, Bounds(0.0, 1e30));

    m.objective().set_coefficient(x, 1.0);
    m.objective().set_coefficient(y, 1.0);

    int c1 = m.add_constraint("c1", ConstraintType::LessEq, 1.0);
    m.set_constraint_coefficient(c1, x, 1.0);
    m.set_constraint_coefficient(c1, y, 1.0);

    int c2 = m.add_constraint("c2", ConstraintType::GreaterEq, 2.0);
    m.set_constraint_coefficient(c2, x, 1.0);
    m.set_constraint_coefficient(c2, y, 1.0);

    SimplexSolver solver;
    SolverResult res = solver.solve(m);

    EXPECT_EQ(res.status, TerminationStatus::Infeasible);
}
