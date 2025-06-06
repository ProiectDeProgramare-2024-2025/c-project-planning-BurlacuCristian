#include "Catalog.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <limits>
#include "Colors.h"

void Catalog::load(const std::string& file) {
    std::ifstream fin(file);
    books.clear();
    if (!fin) {
        // Missing file treated as empty catalog
        return;
    }
    int count;
    if (!(fin >> count)) {
        throw std::runtime_error("Failed to read catalog count");
    }
    fin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string line;
    for (int i = 0; i < count && std::getline(fin, line); ++i) {
        if (line.empty()) { --i; continue; }
        books.push_back(Book::deserialize(line));
    }
}

void Catalog::save(const std::string& file) const {
    std::ofstream fout(file, std::ios::trunc);
    fout << books.size() << '\n';
    for (const auto& b : books) {
        fout << b.serialize() << '\n';
    }
}

void Catalog::addBook(const Book& b) {
    auto it = std::find_if(books.begin(), books.end(),
                           [&](const Book& x){ return x.id == b.id; });
    if (it != books.end()) {
        throw std::runtime_error("Book with ID " + b.id + " already exists");
    }
    books.push_back(b);
}

bool Catalog::deleteBook(const std::string& id) {
    auto it = std::remove_if(books.begin(), books.end(),
                             [&](const Book& b){ return b.id == id; });
    if (it == books.end()) return false;
    books.erase(it, books.end());
    return true;
}

bool Catalog::modifyCopies(const std::string& id, int newCount) {
    for (auto& b : books) {
        if (b.id == id) {
            b.copies = newCount;
            return true;
        }
    }
    return false;
}

std::vector<Book> Catalog::search(const std::string& keyword) const {
    std::vector<Book> res;
    for (const auto& b : books) {
        auto contains = [&](const std::string& field){
            return field.find(keyword) != std::string::npos;
        };
        if (contains(b.id) || contains(b.title) || contains(b.author)) {
            res.push_back(b);
        }
    }
    return res;
}

