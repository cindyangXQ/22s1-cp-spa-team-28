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
    explicit Assignment(int lineNo, std::string_view variable,
                        std::string_view expression);

    std::string_view toString();
    int getLineNo();
    std::string_view getVariable();
    std::string_view getExpression();

private:
    int lineNo;
    std::string_view variable;
    std::string_view expression;
};
