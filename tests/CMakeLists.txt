cmake_minimum_required(VERSION "3.22")

project(MyLittleCity-tests)

find_package(GTest REQUIRED)

enable_testing()

add_executable(
    collider_test
    core/collider.cpp
)

target_link_libraries(
    collider_test
    GTest::gtest_main
    collider
)


include(GoogleTest)
gtest_discover_tests(collider_test)
