#include "Expense.h"
#include "ExpenseUtils.h"

#include <iostream>
#include <cassert>

int main() {

    std::vector<Expense> expenses;

    expenses.push_back({1, 2500, "transport", "Uber"});
    expenses.push_back({2, 4200, "food", "Lunch"});
    expenses.push_back({3, 1800, "books", "Rent a book"});

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

    size_t size_before = expenses.size();
    bool added1 = addExpense (expenses, {4, 0, "food", ""});

    assert (added1 == false);
    assert (expenses.size() == size_before);

    bool added2 = addExpense (expenses, {4, 1500, "food", "Dinner"});

    assert (added2 == true);
    assert (expenses.size() == size_before + 1);
    assert (expenses.back().id == 4);
    assert (expenses.back().amount_in_cents == 1500);

    size_t size_before_duplicate = expenses.size();
    bool added3 = addExpense (expenses, {4, 3000, "food", ""});

    assert (added3 == false);
    assert (expenses.back().amount_in_cents == 1500);
    assert (expenses.size() == size_before_duplicate);

    std::cout << "All tests passed! \n";

    return 0;
}