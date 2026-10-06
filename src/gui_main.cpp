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

    expenses_table.setEditTriggers (QAbstractItemView::NoEditTriggers);

    expenses_table.setSelectionBehavior(QAbstractItemView::SelectRows);
    expenses_table.setSelectionMode(QAbstractItemView::SingleSelection);

    QLabel total_label ("Total: 0.00 EUR");
    layout.addWidget (&total_label);

    QObject::connect (&add_button, &QPushButton::clicked, &subtitle, [&subtitle, 
                                                                        &amount_input, 
                                                                        &category_input, 
                                                                        &description_input, 
                                                                        &expenses,
                                                                        &expenses_table,
                                                                        &total_label] () {
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

            if (valid == true) {
                subtitle.setText("Expense added.");

                int row = expenses_table.rowCount();
                expenses_table.insertRow (row);

                expenses_table.setItem (
                    row, 0,
                    new QTableWidgetItem (QString::number(exp.id))
                );

                expenses_table.setItem (
                    row, 1,
                    new QTableWidgetItem (QString::fromStdString(formatAmount (exp.amount_in_cents)))
                );

                expenses_table.setItem (
                    row, 2,
                    new QTableWidgetItem (QString::fromStdString(exp.category))
                );

                expenses_table.setItem (
                    row, 3,
                    new QTableWidgetItem (QString::fromStdString(exp.description))
                );

                total_label.setText (
                    "Total: " +
                    QString::fromStdString (formatAmount (calculateTotal (expenses))) +
                    " EUR" 
                );
            }
            else
                subtitle.setText("Could not add expense.");
        }
    });

    QPushButton remove_button ("Remove selected expense");
    layout.addWidget (&remove_button);

    QObject::connect (&remove_button, &QPushButton::clicked, &subtitle, [&subtitle, 
                                                                            &expenses_table,
                                                                            &expenses,
                                                                            &total_label] () {
        int row = expenses_table.currentRow();

        if (row == -1) {
            subtitle.setText ("Please select an expense.");
            return;
        }
        else
            subtitle.setText ("Expense selected.");

        int id = expenses_table.item (row, 0)->text().toInt();
        bool removed = removeExpense (expenses, id);

        if (removed == true) {
            expenses_table.removeRow (row);
            subtitle.setText ("Expense removed.");

            total_label.setText (
                "Total: " +
                QString::fromStdString (formatAmount (calculateTotal (expenses))) +
                " EUR" 
            );
        }
        else
            subtitle.setText ("Could not remove expense.");
    });

    window.show();

    return app.exec();
}