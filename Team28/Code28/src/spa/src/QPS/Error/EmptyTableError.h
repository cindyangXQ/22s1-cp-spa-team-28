#pragma once

#include <stdexcept>

/*
 * Error class for the validation of query semantic.
 */
class EmptyTableError : public std::runtime_error {
public:
    bool isBoolean;
    EmptyTableError(const std::string &message) : runtime_error(message) {
        isBoolean = (message == "BOOLEAN");
    }
};
