#pragma once
#include <string>
#include "Date.h"

struct Loan {
    std::string bookID;
    Date borrowDate;
    Date dueDate;

    std::string serialize() const;
    static Loan deserialize(const std::string& line);
};

