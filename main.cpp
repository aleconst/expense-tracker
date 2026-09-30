#include "Expense.h"
#include "ExpenseUtils.h"

#include <iostream>
#include <string>
#include <vector>
#include <cassert>

int main() {

    std::vector<Expense> expenses;

    expenses.push_back({1, 2500, "transport", "Uber"});
    expenses.push_back({2, 4200, "food", "Lunch"});
    expenses.push_back({3, 1800, "books", "Rent a book"});

    displayExpenses (expenses);

    std::cout << "Total: " << formatAmount (calculateTotal (expenses)) << " EUR\n";

    assert (calculateTotal (expenses) == 8500);
    assert (calculateTotal ({}) == 0);

    assert (formatAmount(2505) == "25.05");
    assert (formatAmount(2500) == "25.00");
    assert (formatAmount(75) == "0.75");
    assert (formatAmount(0) == "0.00");

    std::vector<Expense> filtered = filterByCategory (expenses, "food");

    assert (filtered.size() == 1);
    assert (filtered[0].id == 2);
    assert (filterByCategory (expenses, "health").size() == 0);

    displayExpenses (filtered);

    return 0;
}