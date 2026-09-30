#include <iostream>
#include <string>

using namespace std;

struct Expense {
    int id;
    int amount_in_bani;
    string category;
    string description;
};

int main() {

    Expense expense{2, 2500, "transport", "Taxi"};

    cout << expense.amount_in_bani << " "
            << expense.description << "\n";

    return 0;
}