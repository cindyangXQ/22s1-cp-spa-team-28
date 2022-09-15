#pragma once
#include <string>

class Token {
public:
	std::string value;
	Token();
	Token(std::string s);
	bool equals(std::string other);
	virtual bool equals(Token* other) { return false; }
	std::string getValue();
	virtual bool isName() { return false; };
	virtual bool isKeyword() { return false; };
	virtual bool isConstant() { return false; };
};

class Keyword : public Token {
public:
	Keyword(std::string s);
	bool isKeyword() { return true; };
	bool equals(Token* other) { return other->isKeyword() && other->value == this->value; }
};

class Operator : public Token {
public:
	Operator(std::string s);
	bool equals(Token* other) { return other->value == this->value; }
};

class Symbol : public Token {
public:
	Symbol(std::string s);
	bool equals(Token* other) { return other->value == this->value; }
};
