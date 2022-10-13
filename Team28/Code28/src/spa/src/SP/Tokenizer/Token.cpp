#include "Token.h"
#include <iostream>

Token::Token() {}

Token::Token(std::string s) { this->value = s; }

std::string Token::getValue() { return this->value; }

bool Token::equals(std::string other) {
    if (value.compare(other) == 0) {
        return true;
    } else {
        return false;
    }
}

Keyword::Keyword(std::string s) { this->value = s; }

Operator::Operator(std::string s) { this->value = s; }

Symbol::Symbol(std::string s) { this->value = s; }
