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

    displayExpenses (expenses);

    std::cout << "Total: " << formatAmount (calculateTotal (expenses)) << " EUR\n";

    return 0;
}