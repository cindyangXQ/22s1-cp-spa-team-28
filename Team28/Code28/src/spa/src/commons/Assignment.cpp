#include "Assignment.h"

Assignment::Assignment(int lineNo, std::string_view variable,
                       std::string_view expression) {
    this->lineNo = lineNo;
    this->variable = variable;
    this->expression = expression;
};

int Assignment::getLineNo() { return this->lineNo; };

std::string_view Assignment::getVariable() { return this->variable; };
std::string_view Assignment::getExpression() { return this->expression; };

std::string_view Assignment::toString() {
    std::string s = std::to_string(this->lineNo) + ", " + this->variable.data() + " = " +
           this->expression.data();
    return s;
}