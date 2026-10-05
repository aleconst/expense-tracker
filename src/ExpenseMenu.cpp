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