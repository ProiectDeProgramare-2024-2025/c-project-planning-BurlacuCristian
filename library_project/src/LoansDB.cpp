#include "LoansDB.h"
#include <fstream>
#include <algorithm>

void LoansDB::load(const std::string& file) {
    std::ifstream fin(file);
    loans.clear();
    if (!fin) return;
    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        loans.push_back(Loan::deserialize(line));
    }
}

void LoansDB::save(const std::string& file) const {
    std::ofstream fout(file, std::ios::trunc);
    for (const auto& l : loans) {
        fout << l.serialize() << '\n';
    }
}

void LoansDB::addLoan(const Loan& l) {
    loans.push_back(l);
}

bool LoansDB::removeLoanByBook(const std::string& bookID) {
    auto it = std::remove_if(loans.begin(), loans.end(),
                             [&](const Loan& l){ return l.bookID == bookID; });
    if (it == loans.end()) return false;
    loans.erase(it, loans.end());
    return true;
}

bool LoansDB::renewLoan(const std::string& bookID, int extraDays) {
    for (auto& l : loans) {
        if (l.bookID == bookID) {
            l.dueDate = Date::addDays(l.dueDate, extraDays);
            return true;
        }
    }
    return false;
}

