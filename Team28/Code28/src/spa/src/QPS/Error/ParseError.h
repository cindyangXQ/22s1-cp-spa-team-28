#pragma once

#include <stdexcept>

/*
 * Error class for the validation of query during parsing.
 */
class ParseError : public std::runtime_error {
public:
    ParseError(const std::string& message) : runtime_error(message){ }
};