#include "Book.h"
#include <sstream>
#include <stdexcept>

Book::Book(std::string id_, std::string title_, std::string author_, int copies_)
    : id(std::move(id_)), title(std::move(title_)), author(std::move(author_)), copies(copies_) {}

std::string Book::serialize() const {
    std::ostringstream oss;
    oss << id << ' ' << title << ' ' << author << ' ' << copies;
    return oss.str();
}

Book Book::deserialize(const std::string& line) {
    std::istringstream iss(line);
    Book b;
    if (!(iss >> b.id >> b.title >> b.author >> b.copies)) {
        throw std::runtime_error("Malformed catalog line: " + line);
    }
    return b;
}

