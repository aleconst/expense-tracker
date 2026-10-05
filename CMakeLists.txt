cmake_minimum_required(VERSION 3.20)
project(ExpenseTracker LANGUAGES CXX)

add_executable(expense-tracker
    src/main.cpp
    src/ExpenseUtils.cpp
    src/ExpenseStorage.cpp
    src/InputUtils.cpp
    src/ExpenseMenu.cpp
)

target_include_directories(expense-tracker PRIVATE include)
target_compile_features(expense-tracker PRIVATE cxx_std_20)

add_executable(expense-tests
    tests/ExpenseTests.cpp
    src/ExpenseUtils.cpp
    src/ExpenseStorage.cpp
    src/InputUtils.cpp
)

target_include_directories(expense-tests PRIVATE include)
target_compile_features(expense-tests PRIVATE cxx_std_20)

enable_testing()
add_test(NAME expense-tests COMMAND expense-tests)