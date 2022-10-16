#pragma once

#include <stdexcept>

// Define a toString method to return "SyntaxError" -> this will standardise
// your string name in QPS.cpp
/*
 * Error class for the validation of query during parsing.
 */
class SyntaxError : public std::runtime_error {
public:
    SyntaxError(const std::string &message) : runtime_error(message) {}
};
