cmake_minimum_required(VERSION 3.20)
project(ExpenseTracker LANGUAGES CXX)

add_library(expense_core STATIC
    src/ExpenseUtils.cpp
    src/ExpenseStorage.cpp
    src/InputUtils.cpp
)

target_include_directories(expense_core PUBLIC include)
target_compile_features(expense_core PUBLIC cxx_std_20)

add_executable(expense-tracker
    src/main.cpp
    src/ExpenseMenu.cpp
)

target_link_libraries(expense-tracker PRIVATE expense_core)

add_executable(expense-tests
    tests/ExpenseTests.cpp
)

target_link_libraries(expense-tests PRIVATE expense_core)

enable_testing()
add_test(NAME expense-tests COMMAND expense-tests)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(expense-gui
    src/gui_main.cpp
)

target_link_libraries(expense-gui PRIVATE expense_core Qt6::Widgets)