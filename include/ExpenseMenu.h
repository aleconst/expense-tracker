#pragma once
#include "Expense.h"

#include <vector>

void displayMenu();
void displayTotal(const std::vector<Expense>& expenses);
void displayTotalsByCategory(const std::vector<Expense>& expenses);
bool handleFilterExpenses(const std::vector<Expense>& expenses);
bool handleRemoveExpense(std::vector<Expense>& expenses);
bool handleAddExpense(std::vector<Expense>& expenses);
void handleSaveExpenses(const std::vector<Expense>& expenses);
void handleLoadExpenses(std::vector<Expense>& expenses);