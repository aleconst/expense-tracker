#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Expense {
    int id;
    int amount_in_bani;
    string category;
    string description;
};

void displayExpenses (const vector<Expense>& expenses) {
    for (size_t index = 0; index < expenses.size(); index++) {
        cout << expenses[index].id << " "
                << expenses[index].amount_in_bani << " "
                << expenses[index].category << " "
                << expenses[index].description << "\n";
    }
}

int main() {

    vector<Expense> expenses;

    expenses.push_back({1, 2500, "transport", "Uber"});
    expenses.push_back({2, 4200, "food", "Lunch"});
    expenses.push_back({3, 1800, "books", "Rent a book"});

    displayExpenses(expenses);

    return 0;
}