#include "Token.h"
#include <string>

using namespace std;

Token::Token() {}

Token::Token(string s) {
	this->value = s;
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

bool Keyword::isKeyword() {
	return true;
}

bool Keyword::isName() {
	return false;
}

bool Keyword::isConstant() {
	return false;
}

Operator::Operator(string s) {
	this->value = s;
}

bool Operator::isKeyword() {
	return false;
}

bool Operator::isName() {
	return false;
}

bool Operator::isConstant() {
	return false;
}

Symbol::Symbol(string s) {
	this->value = s;
}

bool Symbol::isKeyword() {
	return false;
}

bool Symbol::isName() {
	return false;
}

bool Symbol::isConstant() {
	return false;
}
