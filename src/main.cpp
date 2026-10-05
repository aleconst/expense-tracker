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
            std::cout << "Enter expense ID: ";

            int removed_id;

            if (! (std::getline (std::cin, input_line)))
                break;

            valid = parseInteger (input_line, removed_id);

            if (valid == false) {
                std::cout << "Invalid ID. Please enter a whole number.\n";
                continue;
            }

            if (removeExpense (expenses, removed_id))
                std::cout << "Expense removed.\n";
            else
                std::cout << "Expense not found.\n";
        }
        else if (response == 4) {
            std::cout << "Enter category: ";

            std::string category;
            std::vector<Expense> filtered;
            
            if (!(std::getline (std::cin, category)))
                break;

            filtered = filterByCategory(expenses, category);

            if (filtered.empty() == true)
                std::cout << "No expenses found for this category.\n";
            else
                displayExpenses(filtered);
        }
        else if (response == 5) {
            Expense exp = {};

            std::cout << "Enter expense ID: ";
            if (! (std::getline (std::cin, input_line)))
                break;

            valid = parseInteger (input_line, exp.id);

            if (valid == false) {
                std::cout << "Invalid ID. Please enter a whole number.\n";
                continue;
            }
            
            std::cout << "Enter amount in cents: ";
            if (! (std::getline (std::cin, input_line)))
                break;

            valid = parseInteger (input_line, exp.amount_in_cents);

            if (valid == false) {
                std::cout << "Invalid amount. Please enter a whole number.\n";
                continue;
            }

            std::cout << "Enter category: ";
            if (! (std::getline (std::cin, exp.category)))
                break;

            std::cout << "Enter description: ";

            if (!std::getline (std::cin, exp.description))
                break;

            if(addExpense(expenses, exp) == true)
                std::cout << "Expense added.\n";
            else
                std::cout << "Expense rejected: ID must be positive and unique, amount must be positive, and category must not be empty.\n";
        }
        else if (response == 6) {
            bool saved = saveExpenses (expenses, "expenses.txt");

            if (!saved)
                std::cout << "Could not save expenses.\n";
            else
                std::cout << "Expenses saved.\n";
        }
        else if (response == 7) {
            bool loaded = loadExpenses (expenses, "expenses.txt");

            if (!loaded)
                std::cout << "Could not load expenses. Current data was preserved.\n";
            else
                std::cout << "Expenses loaded.\n";
        }
        else if (response == 8) {
            displayTotalsByCategory (expenses);
        }
        else
            std::cout << "Invalid option\n";
    }

    return 0;
}