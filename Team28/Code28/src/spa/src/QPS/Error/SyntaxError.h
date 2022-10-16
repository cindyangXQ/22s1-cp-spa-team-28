#pragma once

#include <stdexcept>

/*
 * Error class for the validation of query during parsing.
 */
class SyntaxError : public std::runtime_error {
public:
    SyntaxError(const std::string &message) : runtime_error(message) {}
};
