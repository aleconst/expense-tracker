#pragma once
#include "Expense.h"

#include <iostream>
#include <string>
#include <vector>

std::string formatAmount (int cents);
void displayExpenses (const std::vector<Expense>& expenses);
int calculateTotal (const std::vector<Expense>& expenses);
std::vector<Expense> filterByCategory (const std::vector<Expense>& expenses, const std::string& category);
bool addExpense (std::vector<Expense>& expenses, const Expense& exp);
bool removeExpense (std::vector<Expense>& expenses, const int& id);