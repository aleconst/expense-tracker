#pragma once
#include "Expense.h"

#include <vector>

void displayMenu();
void displayTotal(const std::vector<Expense>& expenses);
void displayTotalsByCategory(const std::vector<Expense>& expenses);