#pragma once

#include <string>

/*
 * Class encapsulating a SIMPLE constant value.
 */
class AssignExpression {
public:
    explicit AssignExpression(std::string expr, bool isExact);
    std::string getExpression();
    bool isExactExpression();

private:
    std::string expr;
    bool isExact;
};
