#include "Token.h"
#include <string>
#include <iostream>

using namespace std;

Token::Token() {}

Token::Token(string s) {
	this->value = s;
}

string Token::getValue() {
	return this->value;
}

bool Token::equals(string other) {
	if (value.compare(other) == 0) {
		return true;
	}
	else {
		return false;
	}
}

Keyword::Keyword(string s) {
	this->value = s;
}

Operator::Operator(string s) {
	this->value = s;
}

Symbol::Symbol(string s) {
	this->value = s;
}
