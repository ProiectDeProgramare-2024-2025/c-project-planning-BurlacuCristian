// Date.cpp
#include "Date.h"
#include <sstream>
#include <iomanip>
#include <ctime>
#include <stdexcept>

std::string Date::toString() const {
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << day << '/'
        << std::setw(2) << std::setfill('0') << month << '/'
        << year;
    return oss.str();
}

Date Date::today() {
    std::time_t t = std::time(nullptr);
    std::tm local_tm;
    #if defined(_WIN32) || defined(_WIN64)
        // on Windows, use localtime_s
        localtime_s(&local_tm, &t);
    #else
        // on Unix-like, use localtime_r
        localtime_r(&t, &local_tm);
    #endif
    return Date{ local_tm.tm_mday, local_tm.tm_mon + 1, local_tm.tm_year + 1900 };
}

Date Date::fromTokens(int d, int m, int y) {
    return { d, m, y };
}

Date Date::fromString(const std::string& str) {
    int d, m, y;
    char sep1, sep2;
    std::stringstream ss(str);
    if (ss >> d >> sep1 >> m >> sep2 >> y) {
        return { d, m, y };
    }
    throw std::runtime_error("Invalid date string: " + str);
}

Date Date::addDays(const Date& base, int days) {
    // Build a tm from base date
    std::tm tm_date = {};
    tm_date.tm_mday  = base.day;
    tm_date.tm_mon   = base.month - 1;
    tm_date.tm_year  = base.year - 1900;
    tm_date.tm_isdst = -1;  // let mktime determine DST

    std::time_t t = std::mktime(&tm_date);
    if (t == -1) {
        throw std::runtime_error("Invalid base date for addDays");
    }
    // Add the given number of days (in seconds)
    t += static_cast<std::time_t>(days) * 24 * 60 * 60;

    std::tm new_tm;
    #if defined(_WIN32) || defined(_WIN64)
        localtime_s(&new_tm, &t);
    #else
        localtime_r(&t, &new_tm);
    #endif

    return Date{ new_tm.tm_mday, new_tm.tm_mon + 1, new_tm.tm_year + 1900 };
}
