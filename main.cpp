#include <iostream>
#include <string>
#include <vector>
#include <cassert>

using namespace std;

struct Expense {
    int id;
    int amount_in_cents;
    string category;
    string description;
};

void displayExpenses (const vector<Expense>& expenses) {
    for (size_t index = 0; index < expenses.size(); index++) {
        cout << expenses[index].id << " "
                << expenses[index].amount_in_cents << " EUR cents "
                << expenses[index].category << " "
                << expenses[index].description << "\n";
    }
}

int calculateTotal (const vector<Expense>& expenses) {
    int total = 0;

    for (size_t index = 0; index < expenses.size(); index++)
        total += expenses[index].amount_in_cents;

    return total;
}

int main() {

    vector<Expense> expenses;

    expenses.push_back({1, 2500, "transport", "Uber"});
    expenses.push_back({2, 4200, "food", "Lunch"});
    expenses.push_back({3, 1800, "books", "Rent a book"});

    displayExpenses (expenses);

    assert (calculateTotal (expenses) == 8500);
    assert (calculateTotal ({}) == 0);

    cout << "Total (EUR cents): " << calculateTotal (expenses) << "\n";

    return 0;
}