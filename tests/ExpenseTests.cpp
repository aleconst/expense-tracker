#include "Expense.h"
#include "ExpenseUtils.h"
#include "ExpenseStorage.h"
#include "InputUtils.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <unordered_map>
#include <cstdint>

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

    // Test storage: save and reload expenses

    bool result3 = saveExpenses (expenses, "test_expenses.txt");

    assert (result3 == true);

    std::vector<Expense> loaded;

    bool result4 = loadExpenses (loaded, "test_expenses.txt");

    assert (result4 == true);
    assert (loaded.size() == expenses.size());

    for (size_t index = 0; index < expenses.size(); index++) {
        assert (loaded[index].id == expenses[index].id);
        assert (loaded[index].amount_in_cents == expenses[index].amount_in_cents);
        assert (loaded[index].category == expenses[index].category);
        assert (loaded[index].description == expenses[index].description);
    }

    // Test storage: invalid file leaves existing expenses unchanged

    std::ofstream fout ("test_invalid_expenses.txt");
    Expense exp2 = {10, 1500, "food", "Valid expense"};

    assert (fout.is_open() == true);

    fout << exp2.id << " " << exp2.amount_in_cents << " "
                << std::quoted (exp2.category) << " " 
                << std::quoted (exp2.description) << "\n";

    fout << "invalid data";

    fout.close();
    assert (!fout.fail());

    std::vector<Expense> original_expenses = {{99, 500, "transport", "Original"}};
    std::vector<Expense> existing_expenses = original_expenses;

    bool  result5 = loadExpenses (original_expenses, "test_invalid_expenses.txt");

    assert (result5 == false);
    assert (original_expenses.size() == 1);
    assert (original_expenses[0].id == existing_expenses[0].id);
    assert (original_expenses[0].amount_in_cents == existing_expenses[0].amount_in_cents);
    assert (original_expenses[0].category == existing_expenses[0].category);
    assert (original_expenses[0].description == existing_expenses[0].description);

    // Test parseInteger: reject trailing text without changing the output

    int val = 99;
    bool result6 = parseInteger ("42abc", val);

    assert (result6 == false);
    assert (val == 99);

    // Test parseInteger: accept an integer with surrounding spaces

    int val2 = 99;
    bool result7 = parseInteger (" 42 ", val2);

    assert (result7 == true);
    assert (val2 == 42);

    // Test parseInteger: reject empty input without changing the output

    int val3 = 99;
    bool result8 = parseInteger ("", val3);

    assert (result8 == false);
    assert (val3 == 99);

    // Test category totals: sum repeated categories separately

    std::vector <Expense> total;
    std::vector <Expense> empty_exp;

    total.push_back ({10, 1200, "food", ""});
    total.push_back ({20, 800, "food", ""});
    total.push_back ({30, 500, "transport", ""});

    std::unordered_map <std::string, std::int64_t> mp;
    std::unordered_map <std::string, std::int64_t> empty_mp;

    mp = calculateTotalsByCategory (total);
    empty_mp = calculateTotalsByCategory (empty_exp);

    assert (mp.size() == 2);
    assert (mp["food"] == 2000);
    assert (mp["transport"] == 500);
    assert (empty_mp.empty() == true);

    // Test totals: accumulate and format amounts beyond the 32-bit integer limit

    std::vector<Expense> exp;
    std::int64_t sum = 0;

    exp.push_back ({5, 1500000000, "travel", ""});
    exp.push_back ({6, 1500000000, "travel", ""});

    assert (calculateTotal (exp) == 3000000000);
    
    std::unordered_map <std::string, std::int64_t> categories;
    categories = calculateTotalsByCategory (exp);

    for (const auto& entry : categories)
        if (entry.first == "travel")
            sum += entry.second;

    assert (formatAmount (sum) == "30000000.00");
    assert(sum == 3000000000);

    // Report successful test completion

    std::cout << "All tests passed! \n";

    return 0;
}