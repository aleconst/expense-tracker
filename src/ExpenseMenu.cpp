#include "ExpenseMenu.h"
#include "Expense.h"
#include "InputUtils.h"
#include "ExpenseUtils.h"
#include "ExpenseStorage.h"

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

void displayMenu() {
    std::cout << "\n1. List expenses\n";
    std::cout << "2. Show total\n";
    std::cout << "3. Remove expense\n";
    std::cout << "4. Filter by category\n";
    std::cout << "5. Add expense\n";
    std::cout << "6. Save expenses\n";
    std::cout << "7. Load expenses\n";
    std::cout << "8. Show totals by category\n";
    std::cout << "0. Exit\n\n";

    std::cout << "Choose an option: ";
}

void displayTotal(const std::vector<Expense>& expenses) {
    std::cout << "Total: " << formatAmount (calculateTotal (expenses)) << " EUR\n";
}

void displayTotalsByCategory(const std::vector<Expense>& expenses) {
    std::unordered_map <std::string, int> mp;

    mp = calculateTotalsByCategory (expenses);

    if (mp.empty() == true)
        std::cout << "No expenses available.\n";

    for (const auto& entry : mp) {
        std::cout << std::quoted (entry.first) << ": " 
                        << formatAmount(entry.second) << " EUR\n";
    }
}

bool handleFilterExpenses(const std::vector<Expense>& expenses) {
    std::cout << "Enter category: ";

    std::string category;
    std::vector<Expense> filtered;
    
    if (!(std::getline (std::cin, category)))
        return false;

    filtered = filterByCategory(expenses, category);

    if (filtered.empty() == true)
        std::cout << "No expenses found for this category.\n";
    else
        displayExpenses(filtered);

    return true;
}

bool handleRemoveExpense(std::vector<Expense>& expenses) {
    std::cout << "Enter expense ID: ";

    int removed_id;
    std::string input_line;

    if (! (std::getline (std::cin, input_line)))
        return false;

    bool valid = parseInteger (input_line, removed_id);

    if (valid == false) {
        std::cout << "Invalid ID. Please enter a whole number.\n";
        return true;
    }

    if (removeExpense (expenses, removed_id))
        std::cout << "Expense removed.\n";
    else
        std::cout << "Expense not found.\n";

    return true;
}

bool handleAddExpense(std::vector<Expense>& expenses) {
    Expense exp = {};
    std::string input_line;

    std::cout << "Enter expense ID: ";
    if (! (std::getline (std::cin, input_line)))
        return false;

    bool valid = parseInteger (input_line, exp.id);

    if (valid == false) {
        std::cout << "Invalid ID. Please enter a whole number.\n";
        return true;
    }
    
    std::cout << "Enter amount in cents: ";
    if (! (std::getline (std::cin, input_line)))
        return false;

    valid = parseInteger (input_line, exp.amount_in_cents);

    if (valid == false) {
        std::cout << "Invalid amount. Please enter a whole number.\n";
        return true;
    }

    std::cout << "Enter category: ";
    if (! (std::getline (std::cin, exp.category)))
        return false;

    std::cout << "Enter description: ";

    if (!std::getline (std::cin, exp.description))
        return false;

    if(addExpense(expenses, exp) == true)
        std::cout << "Expense added.\n";
    else 
        std::cout << "Expense rejected: ID must be positive and unique, amount must be positive, and category must not be empty.\n";

    return true;
}

void handleSaveExpenses(const std::vector<Expense>& expenses) {
    bool saved = saveExpenses (expenses, "expenses.txt");

    if (!saved)
        std::cout << "Could not save expenses.\n";
    else
        std::cout << "Expenses saved.\n";
}

void handleLoadExpenses(std::vector<Expense>& expenses) {
    bool loaded = loadExpenses (expenses, "expenses.txt");

    if (!loaded)
        std::cout << "Could not load expenses. Current data was preserved.\n";
    else
        std::cout << "Expenses loaded.\n";
}