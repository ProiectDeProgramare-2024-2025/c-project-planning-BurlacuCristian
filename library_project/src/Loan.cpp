#include "Loan.h"
#include <sstream>
#include <stdexcept>

std::string Loan::serialize() const {
    std::ostringstream oss;
    oss << bookID << ' '
        << borrowDate.day << ' ' << borrowDate.month << ' ' << borrowDate.year << ' '
        << dueDate.day << ' ' << dueDate.month << ' ' << dueDate.year;
    return oss.str();
}

Loan Loan::deserialize(const std::string& line) {
    std::istringstream iss(line);
    Loan l;
    if (!(iss >> l.bookID
              >> l.borrowDate.day >> l.borrowDate.month >> l.borrowDate.year
              >> l.dueDate.day >> l.dueDate.month >> l.dueDate.year)) {
        throw std::runtime_error("Malformed loans line: " + line);
    }
    return l;
}

