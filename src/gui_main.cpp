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
#include <QHBoxLayout>
#include <QHeaderView>

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

void refreshCategoryTotalsTable (QTableWidget& table, const std::vector<Expense>& expenses) {
    table.setRowCount (0);
    auto totals = calculateTotalsByCategory (expenses);

    for (const auto& entry : totals) {
        auto row = table.rowCount ();
        table.insertRow (row);

        table.setItem (row, 0, new QTableWidgetItem (QString::fromStdString (entry.first)));
        table.setItem (row, 1, new QTableWidgetItem (QString::fromStdString (formatAmount (entry.second))));
    }
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    std::vector <Expense> expenses;
    std::string active_filter;

    QWidget window;
    window.setWindowTitle("Expense Tracker");
    window.resize(1000, 800);

    QVBoxLayout layout (&window);
    QLabel title ("Expense Tracker");
    layout.addWidget (&title);

    QLabel subtitle ("Track your daily expenses");
    layout.addWidget (&subtitle);

    QLabel amount_label("Amount (cents)");
    layout.addWidget(&amount_label);

    QSpinBox amount_input;
    amount_input.setRange (1, 100000000);
    amount_input.setSuffix (" cents");
    layout.addWidget (&amount_input);

    QLabel category_label("Category");
    layout.addWidget(&category_label);

    QLineEdit category_input;
    category_input.setPlaceholderText ("Category");
    layout.addWidget (&category_input);

    QLabel description_label("Description (optional)");
    layout.addWidget(&description_label);
    
    QLineEdit description_input;
    description_input.setPlaceholderText ("Expense description");
    layout.addWidget (&description_input);

    QPushButton add_button ("Add expense");
    layout.addWidget (&add_button);

    QHBoxLayout filter_layout;

    QLineEdit filter_input;
    filter_input.setPlaceholderText ("Filter by category");
    filter_layout.addWidget (&filter_input);

    QPushButton clear_button ("Clear filter");
    filter_layout.addWidget (&clear_button);

    QPushButton filter_button ("Apply filter");
    filter_layout.addWidget (&filter_button);
    layout.addLayout(&filter_layout);

    QTableWidget expenses_table;
    expenses_table.setColumnCount(4);
    expenses_table.setHorizontalHeaderLabels ({"ID", "Amount (EUR)", "Category", "Description"});
    expenses_table.horizontalHeader () -> setSectionResizeMode (QHeaderView::Stretch);
    layout.addWidget (&expenses_table);

    expenses_table.setEditTriggers (QAbstractItemView::NoEditTriggers);

    expenses_table.setSelectionBehavior(QAbstractItemView::SelectRows);
    expenses_table.setSelectionMode(QAbstractItemView::SingleSelection);

    QPushButton remove_button ("Remove selected expense");
    layout.addWidget (&remove_button);

    QLabel total_label ("Total: 0.00 EUR");
    layout.addWidget (&total_label);

    QLabel category_totals_label("Totals by category");
    layout.addWidget(&category_totals_label);

    QTableWidget category_totals_table;
    category_totals_table.setColumnCount (2);
    category_totals_table.setHorizontalHeaderLabels ({"Category", "Total (EUR)"});
    category_totals_table.horizontalHeader () -> setSectionResizeMode (QHeaderView::Stretch);
    layout.addWidget (&category_totals_table);

    category_totals_table.setEditTriggers (QAbstractItemView::NoEditTriggers);

    category_totals_table.setSelectionBehavior(QAbstractItemView::SelectRows);
    category_totals_table.setSelectionMode(QAbstractItemView::SingleSelection);

    QObject::connect (&add_button, &QPushButton::clicked, &subtitle, [&subtitle, 
                                                                        &amount_input, 
                                                                        &category_input, 
                                                                        &description_input, 
                                                                        &expenses,
                                                                        &expenses_table,
                                                                        &total_label,
                                                                        &active_filter,
                                                                        &category_totals_table] () {
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

                refreshCategoryTotalsTable (category_totals_table, expenses);

                description_input.clear ();
                amount_input.setValue (1);
                amount_input.setFocus ();
                amount_input.selectAll ();
            }
            else
                subtitle.setText("Could not add expense.");
        }
    });

    QObject::connect (&remove_button, &QPushButton::clicked, &subtitle, [&subtitle, 
                                                                            &expenses_table,
                                                                            &expenses,
                                                                            &total_label,
                                                                            &category_totals_table] () {
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

            refreshCategoryTotalsTable (category_totals_table, expenses);
        }
        else
            subtitle.setText ("Could not remove expense.");
    });

    QHBoxLayout storage_layout;
    QPushButton save_button ("Save expenses");
    storage_layout.addWidget (&save_button);

    QObject::connect (&save_button, &QPushButton::clicked, &subtitle, [&expenses,
                                                                            &subtitle] () {
        bool saved = saveExpenses (expenses, "expenses.txt");

        if (saved == true)
            subtitle.setText ("Expenses saved.");
        else
            subtitle.setText ("Could not save expenses.");
    });

    QPushButton load_button ("Load expenses");
    storage_layout.addWidget (&load_button);
    layout.addLayout(&storage_layout);

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

    QObject::connect (&clear_button, &QPushButton::clicked, &subtitle, [&filter_input,
                                                                            &active_filter,
                                                                            &subtitle,
                                                                            &expenses,
                                                                            &expenses_table] () {
        filter_input.clear ();
        active_filter.clear ();
        
        refreshFilteredTable (expenses_table, expenses, active_filter);

        subtitle.setText ("Showing all expenses.");
    });

    window.show();

    return app.exec();
}