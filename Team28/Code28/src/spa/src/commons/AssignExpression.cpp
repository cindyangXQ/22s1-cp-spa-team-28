#include "AssignExpression.h"

AssignExpression::AssignExpression(std::string expr, bool isExact) {
    this->expr = expr;
    this->isExact = isExact;
};

std::string AssignExpression::getExpression() { return this->expr; };

bool AssignExpression::isExactExpression() { return this->isExact; };