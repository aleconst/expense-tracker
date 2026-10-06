#pragma once
#include "Expense.h"

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

std::string formatAmount (std::int64_t cents);
void displayExpenses (const std::vector<Expense>& expenses);
std::int64_t calculateTotal (const std::vector<Expense>& expenses);
std::vector<Expense> filterByCategory (const std::vector<Expense>& expenses, const std::string& category);
bool addExpense (std::vector<Expense>& expenses, const Expense& exp);
bool removeExpense (std::vector<Expense>& expenses, const int& id);
std::unordered_map <std::string, std::int64_t> calculateTotalsByCategory (const std::vector <Expense>& expenses);
int generateNextId (const std::vector<Expense>& expenses);