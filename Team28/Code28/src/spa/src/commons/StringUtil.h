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
 * Returns the same string from a given string.
 */
inline std::string toString(const std::string &value) { return value; }
