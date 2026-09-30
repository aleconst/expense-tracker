#pragma once
#include "Expense.h"

#include <iostream>
#include <string>
#include <vector>

std::string formatAmount (int cents);
void displayExpenses (const std::vector<Expense>& expenses);
int calculateTotal (const std::vector<Expense>& expenses);