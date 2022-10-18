#include "Assignment.h"

Assignment::Assignment(int lineNo, std::string variable,
                       std::string expression) {
    this->lineNo = lineNo;
    this->variable = variable;
    this->expression = expression;
};

int Assignment::getLineNo() { return this->lineNo; };

std::string Assignment::getVariable() { return this->variable; };
std::string Assignment::getExpression() { return this->expression; };

std::string Assignment::toString() {
    return std::to_string(this->lineNo) + ", " + this->variable + " = " +
           this->expression;
}