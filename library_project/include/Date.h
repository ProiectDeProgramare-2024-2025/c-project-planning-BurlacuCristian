#pragma once
#include <string>

struct Date {
    int day{1};
    int month{1};
    int year{1970};

    std::string toString() const;
    static Date today();
    static Date fromTokens(int d, int m, int y);
    static Date fromString(const std::string& str);
    static Date addDays(const Date& base, int days);
};

