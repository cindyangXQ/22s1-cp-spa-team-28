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

Constant::Constant(string s) {
	super(s);
}

bool Constant::isKeyword() {
	return false;
}

bool Constant::isName() {
	return false;
}

Name::Name(string s) {
	super(s);
}

bool Name::isKeyword() {
	return false;
}

bool Name::isName() {
	return true;
}

Keyword::Keyword(string s) {
	super(s);
}

bool Keyword::isKeyword() {
	return true;
}

bool Keyword::isName() {
	return false;
}

Operator::Operator(string s) {
	super(s);
}

bool Operator::isKeyword() {
	return false;
}

bool Operator::isName() {
	return false;
}

Symbol::Symbol(string s) {
	super(s);
}

bool Symbol::isKeyword() {
	return false;
}

bool Symbol::isName() {
	return false;
}
