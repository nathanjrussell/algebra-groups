add_executable(algebra_groups_test cyclic_test.cpp)
target_link_libraries(algebra_groups_test PRIVATE algebra::groups GTest::gtest_main)
gtest_discover_tests(algebra_groups_test TEST_PREFIX "algebra_groups.")
