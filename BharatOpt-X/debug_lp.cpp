#include "bharatopt/model/Model.hpp"
#include "bharatopt/lp/SimplexSolver.hpp"
#include "bharatopt/milp/MILPSolver.hpp"
#include <iostream>

using namespace bharatopt;

int main() {
    Model m("SimpleInteger");
    m.objective().set_sense(ObjectiveSense::Maximize);

    int x = m.add_variable("x", VariableType::Continuous, Bounds(0.0, 1e30));
    int y = m.add_variable("y", VariableType::Continuous, Bounds(0.0, 1e30));

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
    
    int c4 = m.add_constraint("bound_x", ConstraintType::LessEq, 3.0);
    m.set_constraint_coefficient(c4, x, 1.0);
    int c5 = m.add_constraint("bound_y", ConstraintType::LessEq, 1.0);
    m.set_constraint_coefficient(c5, y, 1.0);

    SimplexSolver solver;
    SolverResult res = solver.solve(m);

    std::cout << "Status: " << (int)res.status << std::endl;
    std::cout << "Obj: " << res.objective_value << std::endl;
    std::cout << "x: " << res.primal_variables[x] << std::endl;
    std::cout << "y: " << res.primal_variables[y] << std::endl;
    
    return 0;
}
