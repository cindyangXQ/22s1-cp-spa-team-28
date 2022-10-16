#pragma once

#include <stdexcept>

/*
 * Error class for the validation of query semantic.
 */
class SemanticError : public std::runtime_error {
public:
    SemanticError(const std::string &message) : runtime_error(message) {}
};
