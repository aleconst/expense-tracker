#include "ExpenseUtils.h"
#include "Expense.h"
#include "ExpenseStorage.h"

#include <vector>
#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QTableWidget>

void refreshExpensesTable (QTableWidget& table, const std::vector<Expense>& expenses) {
    table.setRowCount(0);

    for (const auto& exp : expenses) {
        int row = table.rowCount ();
        table.insertRow (row);

        table.setItem (row, 0, new QTableWidgetItem (QString::number (exp.id)));
        table.setItem (row, 1, new QTableWidgetItem (QString::fromStdString (formatAmount(exp.amount_in_cents))));
        table.setItem (row, 2, new QTableWidgetItem (QString::fromStdString (exp.category)));
        table.setItem (row, 3, new QTableWidgetItem (QString::fromStdString (exp.description)));
    }
}

void refreshFilteredTable (QTableWidget& table, const std::vector<Expense>& expenses, const std::string& filter) {
    if (filter.size() == 0)
        refreshExpensesTable (table, expenses);
    else {
        std::vector<Expense> filtered = filterByCategory (expenses, filter);
        refreshExpensesTable (table, filtered);
    }
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    std::vector <Expense> expenses;
    std::string active_filter;

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

    QLineEdit filter_input;
    filter_input.setPlaceholderText ("Filter by category");
    layout.addWidget (&filter_input);

    QPushButton filter_button ("Apply filter");
    layout.addWidget (&filter_button);

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
                                                                        &total_label,
                                                                        &active_filter] () {
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

                refreshFilteredTable (expenses_table, expenses, active_filter);

                total_label.setText (
                    "Total: " +
                    QString::fromStdString (formatAmount (calculateTotal (expenses))) +
                    " EUR" 
                );

                description_input.clear ();
                amount_input.setValue (1);
                amount_input.setFocus ();
                amount_input.selectAll ();
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

    QPushButton save_button ("Save expenses");
    layout.addWidget (&save_button);

    QObject::connect (&save_button, &QPushButton::clicked, &subtitle, [&expenses,
                                                                            &subtitle] () {
        bool saved = saveExpenses (expenses, "expenses.txt");

        if (saved == true)
            subtitle.setText ("Expenses saved.");
        else
            subtitle.setText ("Could not save expenses.");
    });

    QPushButton load_button ("Load expenses");
    layout.addWidget (&load_button);

    QObject::connect (&load_button, &QPushButton::clicked, &subtitle, [&expenses,
                                                                            &expenses_table,
                                                                            &total_label,
                                                                            &subtitle,
                                                                            &active_filter] () {
        bool loaded = loadExpenses  (expenses, "expenses.txt");

        if (loaded == false) {
            subtitle.setText ("Could not load expenses. Current data was preserved.");
            return;
        }
        else {
            refreshFilteredTable (expenses_table, expenses, active_filter);

            total_label.setText (
                "Total: " +
                QString::fromStdString (formatAmount (calculateTotal (expenses))) +
                " EUR" 
            );

            subtitle.setText ("Expenses loaded.");
        }
    });

    QObject::connect (&filter_button, &QPushButton::clicked, &subtitle, [&filter_input, 
                                                                            &expenses,
                                                                            &expenses_table,
                                                                            &subtitle,
                                                                            &active_filter] () {
        active_filter = filter_input.text().trimmed().toStdString();
        
        if (active_filter.size() == 0) {
            refreshExpensesTable (expenses_table, expenses);
            subtitle.setText ("Showing all expenses.");
        }
        else {
            std::vector<Expense> filtered = filterByCategory (expenses, active_filter);
            refreshExpensesTable (expenses_table, filtered);

            if (filtered.empty() == true)
                subtitle.setText ("No matching expenses.");
            else
                subtitle.setText ("Filter applied.");
        }
    });

    window.show();

    return app.exec();
}