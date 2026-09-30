#include "Expense.h"
#include "ExpenseUtils.h"

#include <iostream>
#include <string>
#include <vector>

std::string formatAmount (int cents) {
    std::string format;

    format += std::to_string (cents / 100);
    format.push_back ('.');

    if (cents % 100 < 10)
        format.push_back ('0');
    format += std::to_string (cents % 100);

    return format;
}

void displayExpenses (const std::vector<Expense>& expenses) {
    for (size_t index = 0; index < expenses.size(); index++) {
        std::cout << expenses[index].id << " "
                << formatAmount(expenses[index].amount_in_cents) << " EUR "
                << expenses[index].category << " "
                << expenses[index].description << "\n";
    }
}

int calculateTotal (const std::vector<Expense>& expenses) {
    int total = 0;

    for (size_t index = 0; index < expenses.size(); index++)
        total += expenses[index].amount_in_cents;

    return total;
}