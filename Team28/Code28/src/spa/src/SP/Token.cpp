#include "Token.h"
#include <string>

using namespace std;

Token::Token() {}

bool Token::isName() { return false; }
bool Token::isKeyword() { return false; }

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

bool Keyword::isKeyword() {
	return true;
}

bool Keyword::isName() {
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

Symbol::Symbol(string s) {
	this->value = s;
}

bool Symbol::isKeyword() {
	return false;
}

bool Symbol::isName() {
	return false;
}
