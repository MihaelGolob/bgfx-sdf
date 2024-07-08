add_subdirectory(lib)

add_executable(catch_test_run Vector2Test.cpp
        EdgeSegmentTest.cpp
        EquationSolverTests.cpp)
target_link_libraries(catch_test_run PRIVATE Catch2::Catch2WithMain)
target_link_libraries(catch_test_run PRIVATE bgfx-sdf)

include(Catch)
catch_discover_tests(catch_test_run)