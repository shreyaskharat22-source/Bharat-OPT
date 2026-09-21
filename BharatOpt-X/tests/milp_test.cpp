#include <gtest/gtest.h>
#include "bharatopt/model/Model.hpp"
#include "bharatopt/milp/MILPSolver.hpp"

using namespace bharatopt;

TEST(MILPTest, BinaryKnapsack) {
    // max 10x + 15y + 20z
    // s.t. 5x + 10y + 12z <= 20
    // x, y, z in {0, 1}
    // Optimal: x=1, z=1 (obj = 30) or y=0 (weight 17 <= 20). Wait, 5+12=17 <= 20.
    
    Model m("BinaryKnapsack");
    m.objective().set_sense(ObjectiveSense::Maximize);

    int x = m.add_variable("x", VariableType::Binary, Bounds(0.0, 1.0));
    int y = m.add_variable("y", VariableType::Binary, Bounds(0.0, 1.0));
    int z = m.add_variable("z", VariableType::Binary, Bounds(0.0, 1.0));

    m.objective().set_coefficient(x, 10.0);
    m.objective().set_coefficient(y, 15.0);
    m.objective().set_coefficient(z, 20.0);

    int c1 = m.add_constraint("weight", ConstraintType::LessEq, 20.0);
    m.set_constraint_coefficient(c1, x, 5.0);
    m.set_constraint_coefficient(c1, y, 10.0);
    m.set_constraint_coefficient(c1, z, 12.0);

    MILPSolver solver;
    SolverResult res = solver.solve(m);

    EXPECT_EQ(res.status, TerminationStatus::Optimal);
    EXPECT_NEAR(res.objective_value, 30.0, 1e-6);
    EXPECT_NEAR(res.primal_variables[x], 1.0, 1e-6);
    EXPECT_NEAR(res.primal_variables[y], 0.0, 1e-6);
    EXPECT_NEAR(res.primal_variables[z], 1.0, 1e-6);
    
    EXPECT_GT(solver.get_statistics().nodes_explored, 0);
}

TEST(MILPTest, SimpleInteger) {
    // max x + y
    // s.t. -x + y <= 1
    //      3x + 2y <= 12
    //      2x + 3y <= 12
    // x, y >= 0, integer
    // LP optimal: x=2.4, y=2.4 (obj 4.8)
    // IP optimal: x=2, y=2 (obj 4) or x=3, y=1 (obj 4) -> both are obj 4.
    
    Model m("SimpleInteger");
    m.objective().set_sense(ObjectiveSense::Maximize);

    int x = m.add_variable("x", VariableType::Integer, Bounds(0.0, 1e30));
    int y = m.add_variable("y", VariableType::Integer, Bounds(0.0, 1e30));

    m.objective().set_coefficient(x, 1.0);
    m.objective().set_coefficient(y, 1.0);

    int c1 = m.add_constraint("c1", ConstraintType::LessEq, 1.0);
    m.set_constraint_coefficient(c1, x, -1.0);
    m.set_constraint_coefficient(c1, y, 1.0);

    int c2 = m.add_constraint("c2", ConstraintType::LessEq, 12.0);
    m.set_constraint_coefficient(c2, x, 3.0);
    m.set_constraint_coefficient(c2, y, 2.0);
    
    int c3 = m.add_constraint("c3", ConstraintType::LessEq, 12.0);
    m.set_constraint_coefficient(c3, x, 2.0);
    m.set_constraint_coefficient(c3, y, 3.0);

    MILPSolver solver;
    SolverResult res = solver.solve(m);

    EXPECT_EQ(res.status, TerminationStatus::Optimal);
    EXPECT_NEAR(res.objective_value, 4.0, 1e-6);
    
    double vx = res.primal_variables[x];
    double vy = res.primal_variables[y];
    EXPECT_NEAR(vx - std::round(vx), 0.0, 1e-6);
    EXPECT_NEAR(vy - std::round(vy), 0.0, 1e-6);
}
