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

std::vector<Expense> filterByCategory (const std::vector<Expense>& expenses, const std::string& category) {
    std::vector<Expense> filtered;

    for (size_t index = 0; index < expenses.size(); index++) {
        if (expenses[index].category == category)
            filtered.push_back (expenses[index]);
    }

    return filtered;
}

bool addExpense (std::vector<Expense>& expenses, const Expense& exp) {
    if (exp.id <= 0)
        return false;

    for (size_t index = 0; index < expenses.size(); index++)
        if (expenses[index].id == exp.id)
            return false;

    if (exp.amount_in_cents <= 0)
        return false;

    if (exp.category.size() == 0)
        return false;
    
    expenses.push_back (exp);

    return true;
}

bool removeExpense (std::vector<Expense>& expenses, const int& id) {
    for (size_t index = 0; index < expenses.size(); index++) {
        if (expenses[index].id == id)
        {
            expenses.erase (expenses.begin() + index);
            return true;
        }
    }

    return false;
}

std::unordered_map <std::string, int> calculateTotalsByCategory (const std::vector <Expense>& expenses) {
    std::unordered_map <std::string, int> totals;

    for (size_t index = 0; index < expenses.size(); index++) {
        totals[expenses[index].category] += expenses[index].amount_in_cents;
    }

    return totals;
}