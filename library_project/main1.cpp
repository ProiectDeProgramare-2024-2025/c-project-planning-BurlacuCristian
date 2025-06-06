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
    std::cout << YELLOW << "Librarian commands:\n"
              << "  view_catalog\n"
              << "  add_book <id> <title> <author> <copies>\n"
              << "  delete_book <id>\n"
              << "  modify_copies <id> <newCount>\n"
              << "  view_loans\n"
              << "  record_return <id>\n" << RESET;
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

        if (cmd == "view_catalog") {
            for (const auto& b : catalog.getBooks()) {
                std::cout << CYAN << b.id << RESET << " | "
                          << b.title << " by " << b.author
                          << " [" << b.copies << " copies]\n";
            }
        } else if (cmd == "add_book") {
            if (argc != 6) throw std::runtime_error("add_book expects 4 parameters");
            catalog.addBook(Book{argv[2], argv[3], argv[4], std::stoi(argv[5])});
            catalog.save(CATALOG_FILE);
            std::cout << GREEN << "Book added.\n" << RESET;
        } else if (cmd == "delete_book") {
            if (argc != 3) throw std::runtime_error("delete_book expects 1 parameter");
            if (!catalog.deleteBook(argv[2])) throw std::runtime_error("Book ID not found");
            catalog.save(CATALOG_FILE);
            std::cout << GREEN << "Book deleted.\n" << RESET;
        } else if (cmd == "modify_copies") {
            if (argc != 4) throw std::runtime_error("modify_copies expects 2 parameters");
            if (!catalog.modifyCopies(argv[2], std::stoi(argv[3]))) {
                throw std::runtime_error("Book ID not found");
            }
            catalog.save(CATALOG_FILE);
            std::cout << GREEN << "Copies updated.\n" << RESET;
        } else if (cmd == "view_loans") {
            for (const auto& l : db.getLoans()) {
                std::cout << CYAN << l.bookID << RESET << " borrowed "
                          << l.borrowDate.toString() << " due "
                          << l.dueDate.toString() << '\n';
            }
        } else if (cmd == "record_return") {
            if (argc != 3) throw std::runtime_error("record_return expects 1 parameter");
            if (!db.removeLoanByBook(argv[2])) {
                throw std::runtime_error("Loan for this book not found");
            }
            // increase copies
            auto res = catalog.search(argv[2]);
            if (!res.empty()) {
                catalog.modifyCopies(argv[2], res[0].copies + 1);
            }
            db.save(LOANS_FILE);
            catalog.save(CATALOG_FILE);
            std::cout << GREEN << "Return recorded.\n" << RESET;
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

