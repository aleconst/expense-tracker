#include "ExpenseUtils.h"
#include "Expense.h"

#include <vector>
#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QTableWidget>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    std::vector <Expense> expenses;

    QWidget window;
    window.setWindowTitle("Expense Tracker");
    window.resize(800, 500);

    QVBoxLayout layout (&window);
    QLabel title ("Expense Tracker");
    layout.addWidget (&title);

    QLabel subtitle ("Track your daily expenses");
    layout.addWidget (&subtitle);

    QSpinBox amount_input;
    amount_input.setRange (1, 100000000);
    amount_input.setSuffix (" cents");
    layout.addWidget (&amount_input);

    QLineEdit category_input;
    category_input.setPlaceholderText ("Category");
    layout.addWidget (&category_input);
    
    QLineEdit description_input;
    description_input.setPlaceholderText ("Expense description");
    layout.addWidget (&description_input);

    QPushButton add_button ("Add expense");
    layout.addWidget (&add_button);

    QTableWidget expenses_table;
    expenses_table.setColumnCount(4);
    expenses_table.setHorizontalHeaderLabels ({"ID", "Amount (EUR)", "Category", "Description"});
    layout.addWidget (&expenses_table);

    QObject::connect (&add_button, &QPushButton::clicked, &subtitle, [&subtitle, 
                                                                        &amount_input, 
                                                                        &category_input, 
                                                                        &description_input, 
                                                                        &expenses] () {
        if (category_input.text().trimmed().isEmpty() == true) {
            subtitle.setText ("Please enter a category.");
            return;
        }
        else {
            Expense exp = {};

            exp.id = generateNextId (expenses);
            exp.amount_in_cents = amount_input.value();
            exp.category = category_input.text().trimmed().toStdString();
            exp.description = description_input.text().toStdString();

            if (exp.id == 0) {
                subtitle.setText ("No more expense IDs available.");
                return;
            }

            bool valid = addExpense (expenses, exp);

            if (valid == true)
                subtitle.setText("Expense added.");
            else
                subtitle.setText("Could not add expense.");
        }
    });

    window.show();

    return app.exec();
}