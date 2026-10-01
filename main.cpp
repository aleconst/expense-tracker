#include "Expense.h"
#include "ExpenseUtils.h"

#include <iostream>
#include <string>
#include <vector>

int main() {

    std::vector<Expense> expenses;

    expenses.push_back({1, 2500, "transport", "Uber"});
    expenses.push_back({2, 4200, "food", "Lunch"});
    expenses.push_back({3, 1800, "books", "Rent a book"});

    while (true) {
        std::cout << "1. List expenses \n";
        std::cout << "2. Show total \n";
        std::cout << "0. Exit \n\n";

        std::cout << "Choose an option: \n";

        int response;
        std::cin >> response;

        if (response == 0)
            break;
        else if (response == 1) {
            displayExpenses (expenses);
            std::cout << "\n";
        }
        else if (response == 2) {
            std::cout << "Total: " << formatAmount (calculateTotal (expenses)) << " EUR \n\n";
        }
        else
            std::cout << "Invalid option \n\n";
    }

    return 0;
}