#include "Token.h"
#include <string>

using namespace std;

Token::Token() {}

Token::Token(string s) {
	this->value = s;
}

Constant::Constant(string s) {
	this->value = s;
}

Name::Name(string s) {
	this->value = s;
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
