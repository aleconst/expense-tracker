#include "ExpenseStorage.h"
#include "Expense.h"

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