# Expense Tracker

A C++ expense tracker with a Qt Widgets desktop interface and a command-line interface. Expenses are stored locally in a text file.

## Authors

- Constantinescu Alexandru - Marius - [@Ale](https://github.com/aleconst)

## Desired result

- Track daily expenses with an amount, category, and optional description.
- Add, edit, and remove expenses through a desktop interface.
- Filter expenses by category.
- Display the overall total and totals for each category.
- Save expenses in a file and load them in a later session.
- Share application logic between the desktop interface, command-line interface, and tests.

## Technologies

- Language: C++20.
- Desktop interface: Qt 6 Widgets.
- Build tools: CMake and Ninja.
- Compiler used during development: GCC through MSYS2 UCRT64 on Windows.
- Testing: C++ assertions and CTest.
- Storage: local text files.

## Main components

### Core logic

- Expense: stores the ID, amount in cents, category, and description.
- ExpenseUtils: provides expense validation, addition, editing, removal, category filtering, total calculations, amount formatting, and ID generation.
- ExpenseStorage: saves and loads expenses. Loading validates records before replacing the current data.
- InputUtils: parses whole-number input and rejects invalid or out-of-range values.

Individual amounts are stored as integer cents. Totals use 64-bit integers, avoiding floating-point rounding when summing expenses.

### Desktop interface

- gui_main.cpp: creates the Qt Widgets interface and connects user actions to the core logic.
- Supports adding, editing, cancelling edits, and removing selected expenses.
- Displays expenses and category totals in tables.
- Supports category filtering and clearing the active filter.
- Requests confirmation before removal or loading.
- Updates tables and totals after successful operations.

Filtering affects the expense table. The overall total and category totals always include all expenses.

### Command-line interface

- main.cpp: starts the console application.
- ExpenseMenu: handles menu display and user interaction.
- Supports listing, adding, removing, filtering, calculating totals, saving, and loading expenses.

### Tests

- ExpenseTests.cpp: checks core operations, validation, formatting, integer parsing, ID generation, large totals, and file storage.
- Storage tests verify that invalid file contents leave existing expenses unchanged.

### Build configuration

- expense_core: Static library containing the expense-management, file-storage, and input-parsing functions.
- expense-tracker: command-line executable.
- expense-gui: desktop executable.
- expense-tests: test executable registered with CTest.

## Build and run

### Requirements

- CMake 3.20 or newer.
- A compiler supporting C++20.
- Ninja.
- Qt 6 with the Widgets module.

The following commands use PowerShell and assume an MSYS2 UCRT64 installation at C:/msys64/ucrt64, with its bin directory available in PATH. Qt and the compiler must use compatible toolchains.

The current CMake configuration requires Qt even when building only the console application or tests.

### Clone the repository

    git clone https://github.com/aleconst/expense-tracker.git
    cd expense-tracker

### Configure and build
    
    cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=C:/msys64/ucrt64
    cmake --build build

Adjust CMAKE_PREFIX_PATH if Qt is installed elsewhere.

### Run the desktop application
    
    .\build\expense-gui.exe

### Run the console application

    .\build\expense-tracker.exe

### Run tests

    ctest --test-dir build --output-on-failure

Use a Debug build for these tests, which rely on assert.

## Data storage and usage

- Enter amounts in cents: 1550 represents 15.50 EUR.
- Categories are required; descriptions are optional.
- Category filtering uses exact, case-sensitive matching.
- Save changes updates an edited expense in memory.
- Save expenses writes the current list to expenses.txt.
- Load expenses replaces the current list with the saved data.
- Saving is manual; closing the application does not automatically save changes.
- The file is located in the working directory from which the application is launched.

Each record contains an ID, an amount in cents, and quoted category and description fields:

    1 1550 "food" "Lunch with friends"
    2 2500 "transport" "Taxi"