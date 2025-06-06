#pragma once
#include <vector>
#include "Loan.h"

class LoansDB {
    std::vector<Loan> loans;
public:
    void load(const std::string& file);
    void save(const std::string& file) const;

    const std::vector<Loan>& getLoans() const { return loans; }

    void addLoan(const Loan& l);
    bool removeLoanByBook(const std::string& bookID);
    bool renewLoan(const std::string& bookID, int extraDays);
};

