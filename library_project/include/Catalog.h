#pragma once
#include <vector>
#include "Book.h"

class Catalog {
    std::vector<Book> books;
public:
    void load(const std::string& file);
    void save(const std::string& file) const;

    const std::vector<Book>& getBooks() const { return books; }

    void addBook(const Book& b);
    bool deleteBook(const std::string& id);
    bool modifyCopies(const std::string& id, int newCount);

    std::vector<Book> search(const std::string& keyword) const;
};

