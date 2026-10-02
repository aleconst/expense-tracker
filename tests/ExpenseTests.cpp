#include "Expense.h"
#include "ExpenseUtils.h"

#include <iostream>
#include <cassert>

int main() {

    // Arrange sample expenses

    std::vector<Expense> expenses;

    expenses.push_back({1, 2500, "transport", "Uber"});
    expenses.push_back({2, 4200, "food", "Lunch"});
    expenses.push_back({3, 1800, "books", "Rent a book"});

    // Test calculateTotal: sample expenses and empty input

    assert (calculateTotal (expenses) == 8500);
    assert (calculateTotal ({}) == 0);

    // Test formatAmount: fractional cents, whole euros, and zero

    assert (formatAmount(2505) == "25.05");
    assert (formatAmount(2500) == "25.00");
    assert (formatAmount(75) == "0.75");
    assert (formatAmount(0) == "0.00");

    // Test filterByCategory: matching and missing categories

    std::vector<Expense> filtered = filterByCategory (expenses, "food");

    assert (filtered.size() == 1);
    assert (filtered[0].id == 2);
    assert (filterByCategory (expenses, "health").size() == 0);

    // Test addExpense: reject zero amount without changing size

    size_t size_before = expenses.size();
    bool added1 = addExpense (expenses, {4, 0, "food", ""});

    assert (added1 == false);
    assert (expenses.size() == size_before);

    // Test addExpense: append a valid expense

    bool added2 = addExpense (expenses, {4, 1500, "food", "Dinner"});

    assert (added2 == true);
    assert (expenses.size() == size_before + 1);
    assert (expenses.back().id == 4);
    assert (expenses.back().amount_in_cents == 1500);

    // Test addExpense: reject duplicate ID without replacing the existing expense

    size_t size_before_duplicate = expenses.size();
    bool added3 = addExpense (expenses, {4, 3000, "food", ""});

    assert (added3 == false);
    assert (expenses.back().amount_in_cents == 1500);
    assert (expenses.size() == size_before_duplicate);

    // Test removeExpense: remove the middle expense and preserve order

    std::vector<Expense> expenses_to_remove;

    expenses_to_remove.push_back ({10, 1500, "food", ""});
    expenses_to_remove.push_back ({20, 3000, "food", ""});
    expenses_to_remove.push_back ({30, 4500, "food", ""});

    bool result1 = removeExpense (expenses_to_remove, 20);

    assert (result1 == true);
    assert (expenses_to_remove.size() == 2);
    assert (expenses_to_remove[0].id == 10);
    assert (expenses_to_remove[1].id == 30);

    // Test removeExpense: missing ID leaves expenses unchanged

    bool result2 = removeExpense (expenses_to_remove, 99);

    assert (result2 == false);
    assert (expenses_to_remove.size() == 2);
    assert (expenses_to_remove[0].id == 10);
    assert (expenses_to_remove[1].id == 30);

    // Report successful test completion

    std::cout << "All tests passed! \n";

    return 0;
}