#include "ExpenseStorage.h"
#include "Expense.h"
#include "ExpenseUtils.h"

#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

bool saveExpenses (const std::vector<Expense>& expenses, const std::string& file_path) {
    std::ofstream fout (file_path);

    if (!fout)
        return false;

    for (size_t index = 0; index < expenses.size(); index++) {
        fout << expenses[index].id << " " << expenses[index].amount_in_cents << " ";
        fout << std::quoted (expenses[index].category) << " " 
                << std::quoted (expenses[index].description) << "\n";
    }

    fout.close();
    return !fout.fail();
}

bool loadExpenses (std::vector<Expense>& expenses, const std::string& file_path) {
    std::ifstream fin (file_path);

    if (!fin)
        return false;

    std::vector<Expense> loaded_expenses;
    std::string message;

    while (std::getline (fin, message)) {
        Expense exp = {};

        std::istringstream row (message);

        if (! (row >> exp.id >> exp.amount_in_cents
                    >> std::quoted (exp.category) 
                    >> std::quoted (exp.description)))
            return false;

        row >> std::ws;

        if (row.eof() == false)
            return false;

        if (addExpense (loaded_expenses, exp) == false)
            return false;
    }

    if (fin.bad() == true || fin.eof() == false)
        return false;

    expenses = loaded_expenses;
    return true;
}