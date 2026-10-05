#include "Expense.h"
#include "ExpenseUtils.h"
#include "ExpenseStorage.h"
#include "InputUtils.h"
#include "ExpenseMenu.h"

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <iomanip>

int main() {

    std::vector<Expense> expenses;

    expenses.push_back({1, 2500, "transport", "Uber"});
    expenses.push_back({2, 4200, "food", "Lunch"});
    expenses.push_back({3, 1800, "books", "Rent a book"});

    while (true) {
        displayMenu();

        int response;
        std::string input_line;

        if (! (std::getline (std::cin, input_line)))
            break;

        bool valid = parseInteger (input_line, response);

        if (valid == false) {
            std::cout << "Invalid input. Please enter a whole number.\n";
            continue;
        }

        if (response == 0)
            break;
        else if (response == 1) {
            displayExpenses (expenses);
        }
        else if (response == 2) {
            displayTotal(expenses);
        }
        else if (response == 3) {
            bool removed = handleRemoveExpense (expenses);

            if (removed == false)
                break;
        }
        else if (response == 4) {
            bool should_continue = handleFilterExpenses (expenses);
            
            if (should_continue == false)
                break;
        }
        else if (response == 5) {
            bool should_continue = handleAddExpense (expenses);

            if (should_continue == false)
                break;
        }
        else if (response == 6)
            handleSaveExpenses (expenses);
        else if (response == 7)
            handleLoadExpenses (expenses);
        else if (response == 8) 
            displayTotalsByCategory (expenses);
        else
            std::cout << "Invalid option\n";
    }

    return 0;
}