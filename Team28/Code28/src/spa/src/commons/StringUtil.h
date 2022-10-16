#pragma once

#include <string>

/*
 * Returns the string form of a given number.
 * Boilerplate for default std::to_string method.
 */
template <typename T> std::string toString(const T &t) {
    return std::to_string(t);
}

/*
 * Returns the converted Type of a given string.
 * Supports string and int only.
 */
template <typename T> T convertToType(const std::string &str) {
    if constexpr (std::is_same_v<T, std::string>) {
        return str;
    }
    if constexpr (std::is_same_v<T, int>) {
        return std::stoi(str);
    }
}

/*
 * Returns the same string from a given string.
 */
inline std::string toString(const std::string &value) { return value; }
