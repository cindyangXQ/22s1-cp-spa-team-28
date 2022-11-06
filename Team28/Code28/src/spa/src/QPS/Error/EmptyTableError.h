#pragma once

#include <stdexcept>

/*
 * Error class for the validation of query semantic.
 */
class EmptyTableError : public std::runtime_error {
private:
    bool isBoolean;

public:
    bool getIsBoolean() { return this->isBoolean; }
    EmptyTableError(bool isBoolean) : runtime_error("Table is empty") {
        this->isBoolean = isBoolean;
    }
};
