add_library(bharatopt_engine)

target_include_directories(bharatopt_engine PUBLIC include)

target_sources(bharatopt_engine PRIVATE
    src/model/Model.cpp
    src/sparse/COO.cpp
    src/sparse/CSR.cpp
    src/sparse/CSC.cpp
    src/utils/Numerics.cpp
    src/lp/StandardFormConverter.cpp
    src/lp/Presolver.cpp
    src/lp/Scaler.cpp
    src/lp/SimplexSolver.cpp
    src/lp/DenseLinearAlgebra.cpp
    src/lp/RevisedSimplexSolver.cpp
    src/lp/InteriorPointSolver.cpp
    src/milp/MILPSolver.cpp
)

# Pybind11
include(FetchContent)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG        v2.11.1
)
FetchContent_MakeAvailable(pybind11)

pybind11_add_module(bharatopt_py bindings/python/bindings.cpp)
target_link_libraries(bharatopt_py PRIVATE bharatopt_engine)
