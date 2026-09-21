# BharatOpt-X Architecture

## Overview
BharatOpt-X is a clean-room C++23 mathematical optimization engine.

## Core Modules
- **LP**: Primal/Dual Simplex, Interior Point Method
- **MILP**: Branch and Bound, Branch and Cut, Heuristics, GNN branching
- **QP**: Convex QP, Primal-Dual Interior Point, Active Set
- **ADMM**: Generic framework
- **Sparse Linear Algebra**: CSR, CSC, SpMV, SpMM
- **GPU backend**: CUDA C++ (optional)

## Tech Stack
- C++23
- CMake
- GoogleTest
