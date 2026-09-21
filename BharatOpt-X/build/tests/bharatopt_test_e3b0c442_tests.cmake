add_test([=[ModelTest.CreateVariableAndConstraint]=]  [==[/Users/shreyas/Desktop/bharat OPT/BharatOpt-X/build/tests/bharatopt_test]==] [==[--gtest_filter=ModelTest.CreateVariableAndConstraint]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[ModelTest.CreateVariableAndConstraint]=]
  PROPERTIES
    
    DEF_SOURCE_LINE [==[/Users/shreyas/Desktop/bharat OPT/BharatOpt-X/tests/model_test.cpp:6]==]
    WORKING_DIRECTORY [==[/Users/shreyas/Desktop/bharat OPT/BharatOpt-X/build/tests]==]
    SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==]
    
)
add_test([=[SparseTest.ConversionsAndSpMV]=]  [==[/Users/shreyas/Desktop/bharat OPT/BharatOpt-X/build/tests/bharatopt_test]==] [==[--gtest_filter=SparseTest.ConversionsAndSpMV]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[SparseTest.ConversionsAndSpMV]=]
  PROPERTIES
    
    DEF_SOURCE_LINE [==[/Users/shreyas/Desktop/bharat OPT/BharatOpt-X/tests/sparse_test.cpp:8]==]
    WORKING_DIRECTORY [==[/Users/shreyas/Desktop/bharat OPT/BharatOpt-X/build/tests]==]
    SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==]
    
)
add_test([=[NumericsTest.Validations]=]  [==[/Users/shreyas/Desktop/bharat OPT/BharatOpt-X/build/tests/bharatopt_test]==] [==[--gtest_filter=NumericsTest.Validations]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[NumericsTest.Validations]=]
  PROPERTIES
    
    DEF_SOURCE_LINE [==[/Users/shreyas/Desktop/bharat OPT/BharatOpt-X/tests/utils_test.cpp:7]==]
    WORKING_DIRECTORY [==[/Users/shreyas/Desktop/bharat OPT/BharatOpt-X/build/tests]==]
    SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==]
    
)
add_test([=[LPSolverTest.SmallLP]=]  [==[/Users/shreyas/Desktop/bharat OPT/BharatOpt-X/build/tests/bharatopt_test]==] [==[--gtest_filter=LPSolverTest.SmallLP]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[LPSolverTest.SmallLP]=]
  PROPERTIES
    
    DEF_SOURCE_LINE [==[/Users/shreyas/Desktop/bharat OPT/BharatOpt-X/tests/lp_test.cpp:7]==]
    WORKING_DIRECTORY [==[/Users/shreyas/Desktop/bharat OPT/BharatOpt-X/build/tests]==]
    SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==]
    
)
add_test([=[LPSolverTest.UnboundedLP]=]  [==[/Users/shreyas/Desktop/bharat OPT/BharatOpt-X/build/tests/bharatopt_test]==] [==[--gtest_filter=LPSolverTest.UnboundedLP]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[LPSolverTest.UnboundedLP]=]
  PROPERTIES
    
    DEF_SOURCE_LINE [==[/Users/shreyas/Desktop/bharat OPT/BharatOpt-X/tests/lp_test.cpp:45]==]
    WORKING_DIRECTORY [==[/Users/shreyas/Desktop/bharat OPT/BharatOpt-X/build/tests]==]
    SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==]
    
)
add_test([=[LPSolverTest.InfeasibleLP]=]  [==[/Users/shreyas/Desktop/bharat OPT/BharatOpt-X/build/tests/bharatopt_test]==] [==[--gtest_filter=LPSolverTest.InfeasibleLP]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[LPSolverTest.InfeasibleLP]=]
  PROPERTIES
    
    DEF_SOURCE_LINE [==[/Users/shreyas/Desktop/bharat OPT/BharatOpt-X/tests/lp_test.cpp:73]==]
    WORKING_DIRECTORY [==[/Users/shreyas/Desktop/bharat OPT/BharatOpt-X/build/tests]==]
    SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==]
    
)
add_test([=[MILPTest.BinaryKnapsack]=]  [==[/Users/shreyas/Desktop/bharat OPT/BharatOpt-X/build/tests/bharatopt_test]==] [==[--gtest_filter=MILPTest.BinaryKnapsack]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[MILPTest.BinaryKnapsack]=]
  PROPERTIES
    
    DEF_SOURCE_LINE [==[/Users/shreyas/Desktop/bharat OPT/BharatOpt-X/tests/milp_test.cpp:7]==]
    WORKING_DIRECTORY [==[/Users/shreyas/Desktop/bharat OPT/BharatOpt-X/build/tests]==]
    SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==]
    
)
add_test([=[MILPTest.SimpleInteger]=]  [==[/Users/shreyas/Desktop/bharat OPT/BharatOpt-X/build/tests/bharatopt_test]==] [==[--gtest_filter=MILPTest.SimpleInteger]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[MILPTest.SimpleInteger]=]
  PROPERTIES
    
    DEF_SOURCE_LINE [==[/Users/shreyas/Desktop/bharat OPT/BharatOpt-X/tests/milp_test.cpp:41]==]
    WORKING_DIRECTORY [==[/Users/shreyas/Desktop/bharat OPT/BharatOpt-X/build/tests]==]
    SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==]
    
)
set(bharatopt_test_TESTS [==[ModelTest.CreateVariableAndConstraint]==] [==[SparseTest.ConversionsAndSpMV]==] [==[NumericsTest.Validations]==] [==[LPSolverTest.SmallLP]==] [==[LPSolverTest.UnboundedLP]==] [==[LPSolverTest.InfeasibleLP]==] [==[MILPTest.BinaryKnapsack]==] [==[MILPTest.SimpleInteger]==])
