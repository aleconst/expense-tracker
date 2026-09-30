#pragma once
#include <string>

struct Expense {
    int id;
    int amount_in_cents;
    std::string category;
    std::string description;
};