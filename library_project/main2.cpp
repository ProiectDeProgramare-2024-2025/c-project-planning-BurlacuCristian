#include <iostream>
#include <vector>
#include <stdexcept>
#include "Colors.h"
#include "Catalog.h"
#include "LoansDB.h"

constexpr const char* CATALOG_FILE = "catalog.txt";
constexpr const char* LOANS_FILE   = "loans.txt";

void printUsage() {
    using namespace Color;
    std::cout << YELLOW << "Patron commands:\n"
              << "  search_books <keyword>\n"
              << "  borrow_book <id>\n"
              << "  view_loans\n"
              << "  renew_book <id>\n"
              << "  return_book <id>\n" << RESET;
}

int main(int argc, char* argv[]) {
    using namespace Color;
    try {
        if (argc < 2) {
            printUsage();
            return 1;
        }

        Catalog catalog;
        LoansDB db;
        catalog.load(CATALOG_FILE);
        db.load(LOANS_FILE);

        std::string cmd = argv[1];

        if (cmd == "search_books") {
            if (argc != 3) throw std::runtime_error("search_books expects 1 parameter");
            auto res = catalog.search(argv[2]);
            for (const auto& b : res) {
                std::cout << CYAN << b.id << RESET << " | "
                          << b.title << " by " << b.author
                          << " [" << b.copies << " copies]\n";
            }
        } else if (cmd == "borrow_book") {
            if (argc != 3) throw std::runtime_error("borrow_book expects 1 parameter");
            auto res = catalog.search(argv[2]);
            if (res.empty()) throw std::runtime_error("Book not found");
            if (res[0].copies <= 0) throw std::runtime_error("No copies available");

            Date today = Date::today();
            Loan l;
            l.bookID = argv[2];
            l.borrowDate = today;
            l.dueDate = Date::addDays(today, 14);
            db.addLoan(l);

            catalog.modifyCopies(argv[2], res[0].copies - 1);
            db.save(LOANS_FILE);
            catalog.save(CATALOG_FILE);
            std::cout << GREEN << "Book borrowed. Due " << l.dueDate.toString() << "\n" << RESET;
        } else if (cmd == "view_loans") {
            for (const auto& l : db.getLoans()) {
                std::cout << CYAN << l.bookID << RESET << " borrowed "
                          << l.borrowDate.toString() << " due "
                          << l.dueDate.toString() << '\n';
            }
        } else if (cmd == "renew_book") {
            if (argc != 3) throw std::runtime_error("renew_book expects 1 parameter");
            if (!db.renewLoan(argv[2], 14)) {
                throw std::runtime_error("Loan not found");
            }
            db.save(LOANS_FILE);
            std::cout << GREEN << "Book renewed for 14 more days.\n" << RESET;
        } else if (cmd == "return_book") {
            if (argc != 3) throw std::runtime_error("return_book expects 1 parameter");
            if (!db.removeLoanByBook(argv[2])) {
                throw std::runtime_error("Loan not found");
            }
            auto res = catalog.search(argv[2]);
            int newCopies = res.empty() ? 1 : res[0].copies + 1;
            if (res.empty()) {
                catalog.addBook(Book{argv[2], "Unknown", "Unknown", newCopies});
            } else {
                catalog.modifyCopies(argv[2], newCopies);
            }
            db.save(LOANS_FILE);
            catalog.save(CATALOG_FILE);
            std::cout << GREEN << "Book returned.\n" << RESET;
        } else {
            std::cerr << RED << "Unknown command.\n" << RESET;
            printUsage();
            return 1;
        }
    } catch (const std::exception& ex) {
        std::cerr << Color::RED << "Error: " << ex.what() << Color::RESET << '\n';
        return 1;
    }
    return 0;
}

