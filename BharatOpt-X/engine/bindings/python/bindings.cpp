#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "bharatopt/model/Model.hpp"
#include "bharatopt/lp/SimplexSolver.hpp"

namespace py = pybind11;
using namespace bharatopt;

PYBIND11_MODULE(bharatopt_py, m) {
    m.doc() = "BharatOpt-X Python Bindings";

    py::enum_<VariableType>(m, "VariableType")
        .value("Continuous", VariableType::Continuous)
        .value("Integer", VariableType::Integer)
        .value("Binary", VariableType::Binary)
        .export_values();

    py::enum_<ConstraintType>(m, "ConstraintType")
        .value("LessEq", ConstraintType::LessEq)
        .value("GreaterEq", ConstraintType::GreaterEq)
        .value("Equal", ConstraintType::Equal)
        .export_values();

    py::enum_<ObjectiveSense>(m, "ObjectiveSense")
        .value("Minimize", ObjectiveSense::Minimize)
        .value("Maximize", ObjectiveSense::Maximize)
        .export_values();

    py::class_<Bounds>(m, "Bounds")
        .def(py::init<>())
        .def(py::init<double, double>())
        .def_readwrite("lower", &Bounds::lower)
        .def_readwrite("upper", &Bounds::upper);

    py::class_<Objective>(m, "Objective")
        .def("set_sense", &Objective::set_sense)
        .def("set_offset", &Objective::set_offset)
        .def("set_coefficient", &Objective::set_coefficient);

    py::class_<Model>(m, "Model")
        .def(py::init<std::string>(), py::arg("name") = "BharatOptModel")
        .def("add_variable", &Model::add_variable)
        .def("add_constraint", &Model::add_constraint)
        .def("set_constraint_coefficient", &Model::set_constraint_coefficient)
        .def("objective", [](Model& mod) -> Objective& { return mod.objective(); }, py::return_value_policy::reference_internal);

    py::enum_<TerminationStatus>(m, "TerminationStatus")
        .value("Optimal", TerminationStatus::Optimal)
        .value("Infeasible", TerminationStatus::Infeasible)
        .value("Unbounded", TerminationStatus::Unbounded)
        .value("MaxIterations", TerminationStatus::MaxIterations)
        .value("NumericalError", TerminationStatus::NumericalError)
        .value("Presolved", TerminationStatus::Presolved)
        .value("Unknown", TerminationStatus::Unknown)
        .export_values();

    py::class_<IterationRecord>(m, "IterationRecord")
        .def_readonly("iteration_number", &IterationRecord::iteration_number)
        .def_readonly("is_phase_1", &IterationRecord::is_phase_1)
        .def_readonly("entering_variable", &IterationRecord::entering_variable)
        .def_readonly("leaving_variable", &IterationRecord::leaving_variable)
        .def_readonly("pivot_element", &IterationRecord::pivot_element)
        .def_readonly("objective_value", &IterationRecord::objective_value)
        .def_readonly("basic_variables", &IterationRecord::basic_variables)
        .def_readonly("reduced_costs", &IterationRecord::reduced_costs)
        .def_readonly("current_solution", &IterationRecord::current_solution);

    py::class_<SolverResult>(m, "SolverResult")
        .def_readonly("status", &SolverResult::status)
        .def_readonly("objective_value", &SolverResult::objective_value)
        .def_readonly("primal_variables", &SolverResult::primal_variables)
        .def_readonly("dual_variables", &SolverResult::dual_variables)
        .def_readonly("reduced_costs", &SolverResult::reduced_costs)
        .def_readonly("iteration_history", &SolverResult::iteration_history);

    py::class_<SimplexSolver>(m, "SimplexSolver")
        .def(py::init<>())
        .def("solve", &SimplexSolver::solve);
}
