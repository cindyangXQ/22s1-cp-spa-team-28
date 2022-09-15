#pragma once
#include <string>

using namespace std;

class Token {
public:
	string value;
	Token();
	Token(string s);
	bool equals(string other);
	virtual bool equals(Token* other) { return false; }
	string getValue();
	virtual bool isName() { return false; };
	virtual bool isKeyword() { return false; };
	virtual bool isConstant() { return false; };
};

class Keyword : public Token {
public:
	Keyword(string s);
	bool isKeyword() { return true; };
	bool equals(Token* other) { return other->isKeyword() && other->value == this->value; }
};

class Operator : public Token {
public:
	Operator(string s);
	bool equals(Token* other) { return other->value == this->value; }
};

class Symbol : public Token {
public:
	Symbol(string s);
	bool equals(Token* other) { return other->value == this->value; }
};
