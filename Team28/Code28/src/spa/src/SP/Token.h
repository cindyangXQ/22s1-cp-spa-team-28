#pragma once
#include <string>

using namespace std;

class Token {
public:
	string value;
	Token();
	Token(string s);
	bool equals(string other);
	virtual bool isName() = 0;
	virtual bool isKeyword() = 0;
	virtual bool isConstant() = 0;
};

class Keyword : public Token {
public:
	Keyword(string s);
	bool isName();
	bool isKeyword();
	bool isConstant();
};

class Operator : public Token {
public:
	Operator(string s);
	bool isName();
	bool isKeyword();
	bool isConstant();
};

class Symbol : public Token {
public:
	Symbol(string s);
	bool isName();
	bool isKeyword();
	bool isConstant();
};
