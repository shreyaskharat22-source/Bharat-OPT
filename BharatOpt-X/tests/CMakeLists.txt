add_executable(bharatopt_test
    model_test.cpp
    sparse_test.cpp
    utils_test.cpp
    lp_test.cpp
    milp_test.cpp
)

target_link_libraries(bharatopt_test
    PRIVATE
    bharatopt_engine
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(bharatopt_test)
