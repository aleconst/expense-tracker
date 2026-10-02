#pragma once
#include "Expense.h"

#include <string>
#include <vector>

bool saveExpenses (const std::vector<Expense>& expenses, const std::string& file_path);