#pragma once

#include <string>

/*
 * A class encapsulating Assignment in SIMPLE.
 */
class Assignment {
public:
    /*
     * Explicit constructor for Assignment.
     */
    explicit Assignment(int lineNo, std::string variable,
                        std::string expression);

    int getLineNo();
    std::string getVariable();
    std::string getExpression();

private:
    int lineNo;
    std::string variable;
    std::string expression;
};
