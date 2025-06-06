#pragma once
#include <string>

class Book {
public:
    std::string id;
    std::string title;
    std::string author;
    int copies{0};

    Book() = default;
    Book(std::string id, std::string title, std::string author, int copies);

    std::string serialize() const;
    static Book deserialize(const std::string& line);
};

