cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_FLAGS "-fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH GTest)

project(banking)

find_package(GTest CONFIG REQUIRED)

add_library(banking STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/banking/Account.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/banking/Transaction.cpp)

target_include_directories(banking PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/banking)

add_executable(tests
        ${CMAKE_CURRENT_SOURCE_DIR}/banking_test/tests.cpp)

target_link_libraries(tests banking GTest::gtest_main GTest::gmock_main)

enable_testing()
add_test(NAME unit_tests COMMAND tests)