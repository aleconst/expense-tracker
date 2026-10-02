#include "Expense.h"
#include "ExpenseUtils.h"
#include "ExpenseStorage.h"

#include <iostream>
#include <string>
#include <vector>
#include <limits>

int main() {

    std::vector<Expense> expenses;

    expenses.push_back({1, 2500, "transport", "Uber"});
    expenses.push_back({2, 4200, "food", "Lunch"});
    expenses.push_back({3, 1800, "books", "Rent a book"});

    while (true) {
        std::cout << "\n1. List expenses\n";
        std::cout << "2. Show total\n";
        std::cout << "3. Remove expense\n";
        std::cout << "4. Filter by category\n";
        std::cout << "5. Add expense\n";
        std::cout << "6. Save expenses\n";
        std::cout << "0. Exit\n\n";

        std::cout << "Choose an option: ";

        int response;

        if (!(std::cin >> response)) {
            if (std::cin.eof() == true)
                break;

            std::cin.clear ();
            std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        if (response == 0)
            break;
        else if (response == 1) {
            displayExpenses (expenses);
        }
        else if (response == 2) {
            std::cout << "Total: " << formatAmount (calculateTotal (expenses)) << " EUR\n";
        }
        else if (response == 3) {
            std::cout << "Enter expense ID: ";

            int removed_id;

            if (!(std::cin >> removed_id)) {
                if (std::cin.eof() == true)
                    break;

                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid ID. Please enter a number.\n";
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
            
            if (!(std::cin >> category))
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
            if (!(std::cin >> exp.id)) {
                if (std::cin.eof() == true)
                    break;

                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid ID. Please enter a number.\n";
                continue;
            }
            
            std::cout << "Enter amount in cents: ";
            if (!(std::cin >> exp.amount_in_cents)) {
                if (std::cin.eof() == true)
                    break;

                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid amount. Please enter a number.\n";
                continue;
            }

            std::cout << "Enter category: ";
            if (!(std::cin >> exp.category))
                break;

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Enter description: ";

            if (!std::getline (std::cin, exp.description))
                break;

            if(addExpense(expenses, exp) == true)
                std::cout << "Expense added.\n";
            else
                std::cout << "Expense rejected: ID must be positive and unique, amount must be positive, and category must not be empty.\n";
        }
        else if (response == 6) {
            bool saved;
            
            saved = saveExpenses (expenses, "expenses.txt");

            if (!saved)
                std::cout << "Could not save expenses.\n";
            else
                std::cout << "Expenses saved.\n";
        }
        else
            std::cout << "Invalid option\n";
    }

    return 0;
}