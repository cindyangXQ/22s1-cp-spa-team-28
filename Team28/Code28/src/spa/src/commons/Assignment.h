#pragma once

#include <string>

#include "TableValue.h"

/*
 * A class encapsulating Assignment in SIMPLE.
 */
class Assignment : public TableValue {
public:
    /*
     * Explicit constructor for Assignment.
     */
    explicit Assignment(int lineNo, std::string variable,
                        std::string expression);

    std::string toString();
    int getLineNo();
    std::string getVariable();
    std::string getExpression();

private:
    int lineNo;
    std::string variable;
    std::string expression;
};
